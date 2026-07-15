# Plan — 002 Create maze edition interface

- The background of the interface window must be semi-transparent
- The interface conatins the following controls
    - Button `Abrir`, to restore a maze file stored in the disk. Include verification of discard the cuurent maze.
    - Button `Guardar`, to save the current maze to its file or a new one if it wasn't previously restored from disk. Include verification to overwite the selected file.
    - Button `Guardar como`, to save the current maze to a new file. Include verification to overwite the selected file.
    - Panel `Dimensiones`. Will contain the following fields as spinners (allowing text input):
        - `Ancho`: Width of the maze. In the range [1, 30]
        - `Alto`: Height of the maze. In the range [1, 30]
    - Panel `Celdas`: Conatins a 2D grid to edit their cells
        - The grid dimension must match the maze dimensions
        - The panel size must adapt to the grid size so it's displayed fully unless it overflows the containing window
        - The grid cells must be saparated by lines
        - The cells will have a fixed regular size
        - Left-clicking each cell will switch tehir type
        - Add scroll-bars if the grid does not fit in the current window
