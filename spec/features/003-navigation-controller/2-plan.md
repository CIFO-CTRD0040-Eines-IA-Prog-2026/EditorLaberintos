# Plan — 003 Create a navigation controller

The navigator has a position (the coordinates of a floor cell in the current maze) and an orientation (can be only left, up, right or down)
By default, the navigator is in the center of the maze
When restoring a saved maze, the position and orientation is also restored
If the navigator is in a wall cell or outside of the maze, navigation is fobidden
It navigate to the neighboring cells using the following keyboard control scheme:
    - Left and right arrow keys to turn 90º left or right
    - Up and down arrow keys to go to move to the next and previous cells in the current orientation
    - If shift is held, the arrow keys move the navigator directly in their corresponding direction (relative to the current direction) without changing orientation.
It's only possible to navigate around floor type cells
The controls will be acceted immediately on key press. While they key is held, the corresponding action will be repetaed with a cooldown of 0.25 seconds. The cooldown must be handled using time stamps instead of delta time.