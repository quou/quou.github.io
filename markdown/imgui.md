# Immediate-Mode Graphical User Interface
![screnshot](media/imgui.png)
![screnshot](media/imgui2.png)

I'm making this small GUI library to aid in the creation of [my game](openmv.html),
to use for displaying debug information like FPS, frame time and memory usage.

It's still a work-in-progress; As of writing it has the following features:
 - Windows that can be moved and resized.
 - Text labels.
 - Layout system using columns.
 - Text input.
 - Buttons.
 - Images and image buttons.
 - Ability to override the colour and font of specified elements.
 - Scrolling and clipping.

The API looks something like this (called every frame, since it is an immediate-mode
system):

```
if (ui_begin_window(ui, "Test Window", make_v2i(0, 0))) {
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
 - Dockable windows.
 - "Floating" elements (i.e. elements that are not confined to a window).
 - Text selection and multi-line text input.
 - Copy and paste.
 - Drop-down, menu bar, colour picker and list box elements.

## Links
 - [Parent Repository](github.com/veridisquot/openmv)
 - [Source](https://github.com/veridisquot/openmv/blob/master/core/src/imui.c) & [Header](https://github.com/veridisquot/openmv/blob/master/core/src/imui.h)
 - [Testing Sandbox](https://github.com/veridisquot/openmv/blob/master/util/imuitest/src/main.c)
