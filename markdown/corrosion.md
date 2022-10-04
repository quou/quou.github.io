# Corrosion

Corrosion is a graphics library with backends for OpenGL ES and Vulkan.

It has the following features:
 - Backends for OpenGL ES and Vulkan 1.2. The Vulkan backend uses the new dynamic rendering features.
 - Custom-made IMGUI, a spiritual successor to the one that I made for [OpenMV](openmv.html).
 - Generic data structures, including vector and hash table implementations that make heavy use
   of macros.
 - Generic and extendable resource manager with Quake PAK reader/writer.
 - Hand-made windowing and input for Windows and X11.
 - Utilities for such things as frustum culling.
 - API abstraction that wraps Vulkan pipelines and emulates them in OpenGL.
 - Compute shaders (Vulkan only).
 - Memory allocator that checks for memory leaks.

## Links
 - [Codeberg](https://codeberg.org/quou/corrosion)
 - [GitHub](https://github.com/quou/corrosion)

## Demo Screenshots
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