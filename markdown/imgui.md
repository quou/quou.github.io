# Immediate-Mode Graphical User Interface
![screnshot](media/imgui.png)
![screnshot](media/imgui2.png)
![screnshot](https://raw.githubusercontent.com/quou/openmv/master/media/003.png)

I made this GUI framework to aid the creation of [OpenMV](openmv.html). It's purpose
was for displaying debug information and some in-game GUI.

It's still a work-in-progress; As of writing it has the following features:
 - Windows that can be moved and resized.
 - Text labels.
 - Text wrapping.
 - Layout system using columns.
 - Text input.
 - Buttons.
 - Toggle switches.
 - Images and image buttons.
 - Ability to override the colour and font of specified elements.
 - Scrolling and clipping.
 - Dockable windows.
 - "Floating" elements (elements not contained within a window).

The API looks something like this (called every frame, since it is an immediate-mode
system):

```
if (ui_begin_window(ui, "Test Window", make_v2i(0, 0), null)) {
	ui_columns(ui, 2, 100);

	if (ui_button(ui, "Button")) {
		printf("Button has been pressed!\n");
	}

	ui_color(ui, make_color(0xff0000, 255));
	if (ui_button(ui, "Button 2")) {
		printf("Button 2 has been pressed!\n");
	}
	ui_reset_color(ui);

	ui_columns(ui, 1, 0);

	ui_end_window(ui);
}
```

The result of this being:

![screenshot](media/imgui3.png)

### Future Goals
 - Text selection and multi-line text input.
 - Copy and paste.
 - Drop-down, menu bar, colour picker and list box elements.

## Links
 - [Parent Repository](https://github.com/quou/openmv)
 - [Source](https://github.com/quou/openmv/blob/master/core/src/imui.c) & [Header](https://github.com/quou/openmv/blob/master/core/src/imui.h)
 - [Testing Sandbox](https://github.com/quou/openmv/blob/master/util/imuitest/src/main.c)

## Implementation Details
The GUI works by pushing different elements into a vector every frame. At the end of each frame,
said vector is iterated and each element is drawn. A faster method would be to simply draw elements
immediately as they are created, but this would complicate things like z sorting.

Layouts are handled using a point for the current "cursor position". The current cursor position
is where the next element will be drawn.

The active, hovered and hot elements are stored as pointers into the aforementioned vector and are set
upon interaction with a given element. For example, the `ui_button` function first pushes a new button
element into the element vector of the current window, then checks for interactions, setting the `hot`
pointer to the previously pushed element if the mouse is held over the button, or the `hovered` pointer
if the button is hovered. It then returns true if the mouse was released over the button (indicating a
button press) or false otherwise.
