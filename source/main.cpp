#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "maze.h"
#include "EditionInterface.h"
#include "NavigationManager.h"

static void ApplyStyle()
{
    ImGuiStyle& oStyle = ImGui::GetStyle();

    oStyle.Colors[ImGuiCol_WindowBg]  = ImVec4(52.0f / 255.0f, 58.0f / 255.0f, 64.0f / 255.0f, 1.00f);
    oStyle.Colors[ImGuiCol_Text]      = ImVec4(255.0f / 255.0f, 212.0f / 255.0f, 59.0f / 255.0f, 1.00f);
    oStyle.Colors[ImGuiCol_Button]    = ImVec4(134.0f / 255.0f, 142.0f / 255.0f, 150.0f / 255.0f, 1.00f);
    oStyle.Colors[ImGuiCol_FrameBg]   = ImVec4(52.0f / 255.0f, 58.0f / 255.0f, 64.0f / 255.0f, 1.00f);
    oStyle.Colors[ImGuiCol_TitleBg]   = ImVec4(134.0f / 255.0f, 142.0f / 255.0f, 150.0f / 255.0f, 1.00f);
    oStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(134.0f / 255.0f, 142.0f / 255.0f, 150.0f / 255.0f, 1.00f);
    oStyle.Colors[ImGuiCol_ChildBg]   = ImVec4(206.0f / 255.0f, 212.0f / 255.0f, 218.0f / 255.0f, 1.00f);
}

int main(int _iArgc, char* _sArgv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* pWindow = SDL_CreateWindow("Editor de Laberintos", 800, 600, 0);
    if (!pWindow)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, NULL);
    if (!pRenderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(pWindow, pRenderer);
    ImGui_ImplSDLRenderer3_Init(pRenderer);

    ApplyStyle();

    Maze oMaze(10, 10);
    NavigationManager oNav(oMaze);
    EditionInterface oEdition(pWindow, oMaze, oNav);

    SDL_Event oEvent;
    int iRunning = 1;
    while (iRunning)
    {
        while (SDL_PollEvent(&oEvent))
        {
            ImGui_ImplSDL3_ProcessEvent(&oEvent);
            if (oEvent.type == SDL_EVENT_QUIT)
            {
                iRunning = 0;
            }
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        oEdition.Update();
        oNav.Update(SDL_GetTicks());

        ImGui::Render();
        SDL_SetRenderDrawColor(pRenderer, 52, 58, 64, 255);
        SDL_RenderClear(pRenderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), pRenderer);
        SDL_RenderPresent(pRenderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}
