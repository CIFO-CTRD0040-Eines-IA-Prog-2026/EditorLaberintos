# 002 — Create maze edition interface

Isolated in a new EditionInterface class.
Floating ImGUI window aligned to the right side of the main window. It should auto-size to cover the whole vertical area and 30% of the horizontal area.
The window must be movable, resizeable and fodable.
Will allow to save and restore mazes from disk (as .lab files) and edit them.
Use standard file selection dialog to handle file selection.
The manipulated maze object must be owned by the parent so it can be passed to other systems.
By default the manipulated has a size of 10x10 and all cells are floor.
The currently open maze filename will be displayed next to the application name in the tittle bar separated by a dash. If none has been opened, display "NEW".

- Contents
    - Buttons to open and save maze file
    - Edit dimensions
    - Edit the state of each cell
