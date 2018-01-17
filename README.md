# TerminalSlides

An application to make presentations with your terminal

# Compile

If you have ncurses just make and you are ready to go.

# Usage

```./slides presentation_file```

This is the first version so the file is very simple:

* \# to create new slide.
* % to add text with this format ```x:y:J:text``` where x and y are the relative
	position of the size of the screen. J is the justification R, L or C.
* & for several lines, for example AsciiArt ```x:y:J:lineCount```. x and y are the
	relative position of the first line, J the justification.
	lineCount the number of lines to include


[![asciicast](https://asciinema.org/a/nbjNwyEtWhW6rz4oHpdvESOxd.png)](https://asciinema.org/a/nbjNwyEtWhW6rz4oHpdvESOxd)