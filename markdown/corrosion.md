# Corrosion

Corrosion is a graphics library with backends for OpenGL and Vulkan.

It has the following features:
 - Backends for OpenGL 3.3 and Vulkan 1.2. The Vulkan backend uses the new dynamic rendering features.
 - Custom-made IMGUI, a spiritual successor to the one that I made for [OpenMV](openmv.html).
 - Generic data structures, including vector and hash table implementations that make heavy use
   of macros.
 - Generic and extendable resource manager with Quake PAK reader/writer.
 - Hand-made windowing and input for Windows and X11.
 - Utilities for such things as frustum culling.
 - API abstraction that wraps Vulkan pipelines and emulates them in OpenGL.
 - Compute shaders (Vulkan only).
 - Memory allocator that checks for memory leaks.
 - Function to check for API support and choose the best available one.
 - Support for Emscripten.

## Links
 - [Codeberg](https://codeberg.org/quou/corrosion)
 - [GitHub](https://github.com/quou/corrosion)

## Emscripten
Here is an interactive demo of the IMGUI running on Emscripten (If it isn't displaying then it
probably means your browser doesn't support WebGL 2.0):

<iframe src="content/corrosion/ui.html" width="800" height="600" style="padding:none;border:none;" scrolling="no"></iframe>

There are some bugs in the Emscripten backend that need fixing and some unimplemented features
(such as the filesystem API), but it's functional for the most part.

## TinyMMO
This is an early-stage multiplayer game that I'm building to somewhat battle-test this framework.
![Screenshot](https://codeberg.org/quou/tinymmo/raw/branch/master/media/screenshot.png)

[Here](https://codeberg.org/quou/tinymmo) is the Codeberg repository for it.

## Demo Screenshots
I have written a few demos using Corrosion to test out the features:

<figure>
	<img src="https://codeberg.org/quou/corrosion/raw/branch/master/media/demos/sand.png" alt="Compute Shader Sand Simulation">
	<figcaption>Compute Shader Sand Simulation</figcaption>
</figure>

<figure>
	<img src="https://codeberg.org/quou/corrosion/raw/branch/master/media/demos/voxel.png" alt="Voxel Raytracer">
	<figcaption>Voxel Raytracer</figcaption>
</figure>

<figure>
	<img src="https://codeberg.org/quou/corrosion/raw/branch/master/media/demos/3d.png" alt="3D Renderer">
	<figcaption>Simple 3D Renderer</figcaption>
</figure>

<figure>
	<img src="https://codeberg.org/quou/corrosion/raw/branch/master/media/demos/volume.png" alt="Volumetric Rendering">
	<figcaption>Volumetric Rendering</figcaption>
</figure>

<figure>
	<img src="media/imgui4.png" alt="Volumetric Rendering">
	<figcaption>Immediate Mode GUI</figcaption>
</figure>
