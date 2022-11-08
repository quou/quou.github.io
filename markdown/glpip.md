# Vulkan-Style Pipeline Objects in OpenGL

*November 8, 2022*

Not so long ago, I added an OpenGL backend to my graphics framework, [Corrosion](corrosion.html).
My framework was designed initially as a fairly thin wrapper over the Vulkan API, with direct
abstractions over Vulkan graphcis pipelines. This doesn't translate well to OpenGL's global state
machine and OpenGL's way of sending data to shaders, so a system that emulates Vulkan pipelines
in OpenGL is necessary. Since I wanted to support Emscripten (which was the whole idea behind
adding an OpenGL backend), I couldn't use many of the more modern OpenGL features that would
make this more trivial (such as being able to directly specify sampler bind points in the
shader).

## Not Supporting Compute Shaders
I chose not to support compute shaders in the OpenGL backend. My reasoning behind this is that
almost any GPU that supports compute shaders will also support Vulkan, so the client application
will be able to use the Vulkan backend instead. WebGL also doesn't support compute shaders,
so if it's a web app it can't use compute shaders anyway. Omitting them from the OpenGL backend
makes the framework easier to maintain, so I think it's worth it in this case.

## The API
The final API looks something like this:

```
struct pipeline* pipeline = video.new_pipeline(
    flags,
    shader,
    target_framebuffer,
    vertex_attributes,
    descriptor_sets
);

/* Every frame: */
video.begin_pipeline(pipeline);
    /* Submit draw commands */
video.end_pipeline(pipeline);
```

## Pipeline state
The easiest part was the pipeline state. This essentially boils to an array of things that
get enabled on the pipeline begin and disabled on the pipeline end function calls. Combine
this with some flag checking for things like blending and depth testing, and we get quite
a robust and maintainable system.

## Descriptor sets
OpenGL doesn't have descriptor sets, so they need to be emulated. In Corrosion, there are
three kinds of resources that can be bound in descriptors: Textures, storage buffers and
uniform buffers. The OpenGL backend doesn't support compute shaders, so I didn't have to
worry about supporting storage buffers.

The first step to supporting descriptor is in the shader compiler. The shader compilation
process looks like this:
 - Compile shaders to SPIR-V bytecode from GLSL.
 - Use SPIRV-Cross to cross-compile the SPIR-V bytecode into OpenGL GLSL (since the
   target version of OpenGL doesn't support directly loading SPIR-V, that's a 4.6 feature
   and I'm targeting version 3.3 on desktop or ES 3.0 on the web). At this step,
   binding locations are re-mapped for uniform buffers such that locations are spaced out
   to alloow for emulation of descriptor sets and binding locations are saved so that
   `glActiveTexture` can be used to bind textures to the correct location.
 - Write the SPIR-V bytecode, the OpenGL GLSL code as well as some meta-data for OpenGL
   (binding locations, etc.) to a unified file in the CSH format (A format I made
   specifically for this).

