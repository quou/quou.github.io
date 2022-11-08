# Vulkan-Style Pipeline Objects in OpenGL

![image](media/glpip.png)

*November 8, 2022*

Not so long ago, I added an OpenGL backend to my graphics framework, [Corrosion](corrosion.html).
My framework was designed initially as a fairly thin wrapper over the Vulkan API, with direct
abstractions over Vulkan graphics pipelines. This doesn't translate well to OpenGL's global state
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

## Pipeline State
The easiest part was the pipeline state. This essentially boils to an array of things that
get enabled on the pipeline begin and disabled on the pipeline end function calls. Combine
this with some flag checking for things like blending and depth testing, and we get quite
a robust and maintainable system.

```
/* On pipeline creation: */
if (flags & pipeline_flags_depth_test)
	vector_push(pipeline->to_enable, GL_DEPTH_TEST);
if (flags & pipeline_flags_dynamic_scissor)
	vector_push(pipeline->to_enable, GL_SCISSOR_TEST);

/* On pipeline begin: */
for (usize i = 0; i < vector_count(pipeline->to_enable); i++) {
	glEnable(pipeline->to_enable[i]);
}

/* Same thing for pipeline end, except calling glDisable instead of glEnable. */
```

## Descriptor Sets
OpenGL doesn't have descriptor sets, so they need to be emulated. In Corrosion, there are
three kinds of resources that can be bound in descriptors: Textures, storage buffers and
uniform buffers. The OpenGL backend doesn't support compute shaders, so I didn't have to
worry about supporting storage buffers.

The first step to supporting descriptor is in the shader compiler. The shader compilation
process looks like this:
 - Compile shaders to SPIR-V bytecode from GLSL.
 - Use SPIRV-Cross to cross-compile the SPIR-V bytecode into OpenGL GLSL (since the
   target version of OpenGL doesn't support directly loading SPIR-V). At this step,
   binding locations are re-mapped for uniform buffers such that locations are spaced out
   to allow for emulation of descriptor sets and binding locations are saved so that
   `glActiveTexture` can be used to bind textures to the correct location.
 - Write the SPIR-V bytecode, the OpenGL GLSL code as well as some meta-data for OpenGL
   (binding locations, etc.) to a unified file in the CSH format (A format I made
   specifically for this).

![image](media/glpip2.png)

The `bind_pipeline_descriptor_set` function takes a pointer to a Corrosion pipeline, a
null-terminated string for the name of the descriptor set and a target index where the
pipeline will be bound to. In the Vulkan backend, this translates into a hash table
lookup to get a handle to the descriptor from the name and a call to
`vkCmdBindDescriptorSets`. In OpenGL, it's a bit more involved: It iterates all of the
descriptors in the descriptor set, remaps the descriptor bind point to match the
target bind point for the set, then either calls `glActiveTexture` and `glBindTexture` for
a sampler descriptor or `glBindBufferBase` for a uniform buffer.

Binding remapping is done with the following formula:

```
location = set_index * 16 + binding
```

This has the effect of allowing sixteen descriptors per set, which is a reasonable limit
in my opinion. Since OpenGL doesn't allow texture bindings over GL_TEXTURE0 + k, where `k`
is the value of GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, which may be as low as 16, this won't
work in a lot of cases where more than one descriptor set is bound at once. To get around
this, the shader compiler generates an incremental bind location for samplers, then a
hash table lookup is used to convert the re-mapped location to get said generated location.

## Vertex Attributes
Vulkan has vertex attributes as part of the pipelines, and so does Corrosion (being a
thin-ish wrapper). OpenGL, however, uses a Vertex Array Object (VAO) to describe the
relationship between raw data and the vertex shader inputs. My first inclination was to
have a VAO attached to each pipeline that gets set up on pipeline creation (calling
`glEnableVertexAttribArray`, `glVertexAttribDivisor` and `glVertexAttribPointer` in
accordance to the configuration passed into `new_pipeline`). However, OpenGL requires a
vertex buffer to be bound when the VAO is configured, which I can't do in this case
due to how Corrosion separates vertex buffers from vertex attribute configurations;
Vertex buffers are separate objects to pipelines where vertex attributes are configured.

In OpenGL 4.3, the functions `glBindVertexBuffer` and `glVertexBindingDivisor` were
added, which allow vertex buffers to be separate from VAOs. However, I cannot use
these functions since I'm targeting OpenGL ES 3.0.

What I had to do instead was create a VAO as part of the pipeline, but instead of
configuring it on every binding, it's configured in the `bind_vertex_buffer` function,
which first binds the vertex buffer, then configures the VAO. This is unfortunate as it
makes caching the state of the VAO impossible, however setting up a VAO isn't that
performance intensive (there isn't any extra allocation that happens, since the VAO can
still be allocated at the beginning, it just can't be configured on pipeline creation).

## Conclusion
The OpenGL backend is now fairly stable and works well enough to run in a web browser
(compiled to web assembly with Emscripten). You can see a live demo of the GUI system
running through the OpenGL backend on the [page for corrosion](corrosion.html).
Desktop applications can seamlessly switch backends with no changes to the application
code (so long as they aren't using compute shaders!). While rather challenging to
implement, I consider this sub-project a success and worth the effort to have the
possibility of compiling anything I make with Corrosion for a web browser for ease
of distribution on sites such as itch.io.

There are still bugs and unimplemented features in the Emscripten platform implementation
(such as the file-system API), but they're unrelated to the OpenGL backend.
