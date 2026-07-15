#ifndef MAZE3DRENDERER_H
#define MAZE3DRENDERER_H

#include <SDL3/SDL_render.h>
#include "NavigationManager.h"
#include "Maze.h"

class Maze3DRenderer
{
public:
    Maze3DRenderer(SDL_Renderer* _pRenderer, NavigationManager& _oNav, Maze& _oMaze);

    void RenderImGui();
    void RenderBackground();

private:
    SDL_Renderer* m_pRenderer;
    NavigationManager& m_oNav;
    Maze& m_oMaze;

    bool IsValidPosition() const;
    void DrawView(int _iWidth, int _iHeight);
    void DrawBackground(int _iWidth, int _iHeight);
    void CastRays(int _iWidth, int _iHeight);
};

#endif
