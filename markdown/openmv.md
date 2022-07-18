# OpenMV

OpenMV started out as an effort to create an open source platform game from the
ground up in C, using nothing but single header utilities and operating system
libraries. Since then, it has grown to become quite a capable box of tools
that would be useful to create a 2-D game, including:

 - Basic Rendering.
 - Resource management.
 - Code hot-reloading for game logic.
 - X11 and Windows support.
 - Physics.
 - Audio.
 - In-game GUI.
 - Debug IMGUI.
 - Save-game system.
 - Room system.
 - Resource Packing.
 - Tilemap system.
 - Dialogue system for NPCs.
 - Scripting language.
 - Post processing.
 - Command system.

## Links
 - [GitHub](https://github.com/quou/openmv).
 - [Write-up about the scripting language that I made for this engine](scrinterp.html).
 - [Page for the GUI that I made for this engine](imgui.html).

## Screenshots
![screenshot](https://raw.githubusercontent.com/quou/openmv/master/media/004.png)
![screenshot](https://raw.githubusercontent.com/quou/openmv/master/media/001.png)
![screenshot](https://raw.githubusercontent.com/quou/openmv/master/media/002.png)

## Implementation Details
### Levels
Levels are created using the Tiled level editor, using a custom exporter plug-in
made using Tiled's JavaScript API, exporting to a binary format that is fast and
easy to load. It supports most features of Tiled, including animated tiles.

### Rendering
The engine uses a simplistic batched 2-D renderer made with OpenGL. It pushes
vertices into a buffer and then draws them on command.

There is also support for post-processing, which allows for things like the CRT
effect that can be seen on the first screenshot above.

### Code Hot-reloading
One of the more non-trivial features that I implemented was code hot-reloading
of game logic, to allow for faster prototyping. This works by compiling the
project in three sub-projects: A "core" project that contains all of the basic
engine features like the renderer, a "logic" project that contains all of the
game logic and a "bootstrapper" project that loads the logic and core. The
logic and core projects are both built as shared objects and the bootstrapper
is an executable. "Logic" is where all of the game logic is written and this
is the code that can be reloaded. "Core"'s code cannot be reloaded at runtime,
but it can be accessed from "logic".

The first thing that the bootstrapper project does is make a copy of the "logic"
library. This is so that the logic shared object can be recompiled without the
bootstrapper and compiler fighting over permission to open a handle to it. Next,
it opens the copy of the logic shared object and loads the necessary functions
that it contains (including `on_update` and `on_init` functions, for example).
Every frame, it checks to see if the original "logic" shared object has been
updated by using the file's last modification time as reported by the operating
system, which would indicate that the logic has been recompile. If it has, the
program unloads the current "logic" library, updates the copy and reloads it.

One of the challenges with this is that global variables inside "logic" cannot
be changed, since they will simply be reset every reload. I worked around this
by having the bootstrapper maintain a heap allocation that "logic" can use to
store state.

I have found that this system works pretty well, and hasn't caused any major
performance bottlenecks. Of course, there is some overhead to constantly
making the syscall to get the modification time of the logic shared object,
which is why it only happens in debug mode and hot reloading is simply
disabled in release mode. The overhead could be reduced by only checking
for a modification every second or so, at the cost of a small delay between
recompiling the code and having it reload. The actual reloading doesn't
seem to cause a major frame drop, either, which was surprising.

### Resource Packing
All of the resources for the game, except shared objects are compiled into a
single package in release mode. This is to keep the files that will be
distributed clean without risk of a user accidentally deleting them.
