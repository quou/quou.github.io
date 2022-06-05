# Vulkan Renderer

I created this project to learn the Vulkan graphics API during my time at AIE for my
Complex Games Systems project. It's written in C++ and includes a number of features,
both graphics related and more general features:

 - API abstraction.
 - Bloom.
 - Percentage-Closer Soft Shadows.
 - Deferred shading.
 - Resource packing.
 - Modular architecture.
 - 2-D rendering and atlasing system.
 - Basic Immediate-mode GUI.

## PCSS
One of my focuses of this project was to implement soft shadows. I used quite a few
resources to aid me in this project, including [this project](https://github.com/pboechat/PCSS)
and [this paper](https://developer.download.nvidia.com/shaderlibrary/docs/shadow_PCSS.pdf).
PCSS is a technique that causes the shadows to become softer the further away the
blocker is from the surface that it's casting a shadow onto, just like how is seen
in real life. In the first screenshot below, its particularly visible on the shadow
of the tall cube.

It works by doing an initial "blocker search" to gain an average of all of the depth
values in a specified area in terms of the surface, using a poisson disk (not unlike a
PCF implementation). Then, it uses this formula (described in the paper) to estimate the
size of the penumbra:
`p = (receiver - blocker) * lightsize / blocker`, where `blocker` is the average gained
from the blocker search. This estimation is then used to calculate a radius to sample
the depth map using PCF. PCF is probably not the best approach to use for this kind of
soft shadowing, as it requires a lot of samples (the screenshots below use 64 per-pixel)
to avoid banding effects. Variance shadow mapping would probably yield much better results
and is something I'd like to research more into in the near future.

## Links
 - [GitHub](https://github.com/quou/vkr)

## Screenshots
![screenshot](media/vkr_screenshot_001.png)
