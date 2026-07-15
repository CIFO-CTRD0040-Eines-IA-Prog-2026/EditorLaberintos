# Plan — 006 Add navigation interpolation animation

The NavigationManager will include information necessary to perfom interpolation:
    - Previous position and orientation
    - Interpolation end time (in ticks)
The NavigationManager will compute the offset for both position and orientation according to the interpolation progress
NavigationManager repeat key cooldown must removed. Instead, a new navigation action can only be executed once the interpolation animation is done.
If the position of the navigator is set in the edition grid, the interpolation will finish immdiately
Loading a maze and modifying the maze (either its cells or dimensions) will also finish interpolation imediately
The interpolation will be reflected in both the edition grid and the 3d view