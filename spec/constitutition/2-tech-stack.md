# Tech Stack

- Documentation
Check updated documentation through Context7 MCP to generate SDL3 and ImGUI code

- Conventions
Code and comments in english, content in spanish.

-----------------------------

## Structure
- The entry point is at `main.cpp`
- The project has the following structure:
    - Folder "external": Contains files releated to external libraries.
    - Folder "out":
        - Folder "release" contains the release build output
        - Folder "debug" contains the release build output
    - Folder "source": Contains all *.cpp files
    - Folder "headers": Contains all *.h files

## Runtime

- `SDL3.dll` must be in the same directory as `main.exe` (copy from `external/SDL3-3.4.12/x86_64-w64-mingw32/bin/`).

## Linker flags

- Use `-lSDL3`

## Framework

- **SDL3** `SDL_Renderer` backend (no OpenGL/Vulkan — SDL3 renderer is the drawing target). Stored in folder `external/SDL3-3.4.12`
- **Dear ImGui 1.92.8** backends: `imgui_impl_sdl3` (platform) + `imgui_impl_sdlrenderer3` (renderer). Stored in folder `external/imgui-1.92.8`

## Project structure

- `main.cpp` — single entry point. Isolate new components in clases stored in independent files.

## Toolchain

- MinGW-w64 via MSYS2 (`C:\msys64\ucrt64\bin\g++.exe`).
- C++23 standard (`c_cpp_properties.json`).
- When in need of doumentations or examples, check Context7


-----------------------------