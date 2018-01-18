# TerminalSlides

An application to make presentations with your terminal

# Compile

If you have ncurses just make and you are ready to go.

# Usage

```./slides presentation_file```

This is the first version so the file is very simple:

* \#[W|B] to create new slide, W for white background B for black.
* % to add text with this format ```x:y:J:color:text``` where x and y are the relative
	position of the size of the screen. J is the justification R, L or C and color
	for select the color 1 Red, 2 Green, 3 Blue and 4 Black or white depend of the background.
* & for several lines, for example AsciiArt. ```x:y:J:color:lineCount``` x and y are the
	relative position of the first line, J the justification, color the color and
	lineCount the number of lines to include.

# Compatibilities

You need a compatible terminal emulator, like iTerm, in order to see colors.


[![asciicast](https://asciinema.org/a/5iTTmyqjpH7A4FBLg66ZnvbkA.png)](https://asciinema.org/a/5iTTmyqjpH7A4FBLg66ZnvbkA)