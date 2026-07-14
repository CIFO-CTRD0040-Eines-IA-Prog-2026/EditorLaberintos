# Plan — 001 Create structure to contain maze definition

- File: `Maze.h`
- Contains struct Maze conatining the following folders:
    - Width: Width of the maze
    - Height: Height of the maze
    - Current cell: Coordinates of the current cells of the navigator
    - Current orientation: Current orientation of the navigator
    - Cells: Vector of enum CellType (can be Wall or Floor) representing a 2D matrix in row-major order
- An out of bounds cell corrdinate is considered a wall
