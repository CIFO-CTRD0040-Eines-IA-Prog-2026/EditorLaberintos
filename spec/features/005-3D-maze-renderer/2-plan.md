# Plan — 005 Create a 3D view of the maze

Renders using SDL3 in the background a 3D view of the maze using ray-casting:
    - Every cell is 5 x 5 x 5 meters in size
    - The camera is at the center of the window, 2.5 meters over the floor
    - The camera has perspective and a 90º field of view
    - The axes of the coordinate system have the following meaning:
        - X: Right
        - Y: Up
        - Z: Front
    - The view is updated on every frame according to the current V-sync refresh rate 
    - First draw the maze background:
        - The upper half of the window as the ceil color (blue)
        - The lower half of the window as the floor color (brown)
    - Then for each position in the horizontal axis of the window, find the closest distance to a maze wall:
        - Use the 2D grid to detect the collision (only test the cells traversed by the ray)
        - Consider a plane 2 meters wide right in front of the camera fitting the whole window width
        - Use a ray of infinite length with a direction from the camera to a point in that plane:
            - X component: interpolate along the window position where -1 is the left-most position, 0 the center and 1 the right-most
            - Y component: the complementary of X so the vector is normalized
    - Draw a vertical line (centered on the window) with a height consistent with the computed distance. The color will depend of the side of the wall hit:
        - Up: #F0F0F0
        - Right: #808080
        - Down: #404040
        - Down: #202020
    - If the navigator is inside a wall, display the message: "Posición inválida. Debe especificarse una celda suelo"
    - The edges of the maze must be drawn as walls