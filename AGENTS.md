# AGENTS.md — ProyectoFinal
- Developed using Specification-Driven Development (SDD). Folder spec/ is the sole source of thruth.

## REGLES IMPORTANT
- Apply project skills (.agents/skills/).
- When asked, do not perform automatic actions. Wait for confirmation. 
- Work only inside this folder. 
- Contents in spanish, code & comments in english.



# AGENTS.md — ProyectoFinal

## Structure
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

- **SDL3** `SDL_Renderer` backend (no OpenGL/Vulkan — SDL3 renderer is the drawing target).
- **Dear ImGui 1.92.8** backends: `imgui_impl_sdl3` (platform) + `imgui_impl_sdlrenderer3` (renderer).

## Project structure

- `main.cpp` — single entry point, contains everything.
- All source files sit flat at the project root, no subdirectory source tree.
- No tests, no CI, no lint/typecheck pipeline.

## Toolchain

- MinGW-w64 via MSYS2 (`C:\msys64\ucrt64\bin\g++.exe`).
- C++23 standard (`c_cpp_properties.json`).
- When in need of doumentations or examples, check Context7
