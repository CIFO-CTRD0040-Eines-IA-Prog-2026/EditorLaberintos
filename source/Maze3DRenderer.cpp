#include <cmath>
#include <imgui.h>
#include "Maze3DRenderer.h"

static const SDL_Color COLOR_CEIL  = { 70, 130, 220, 255 };
static const SDL_Color COLOR_FLOOR = { 139, 90, 43, 255 };

static const float PI = 3.14159265358979323846f;

static const SDL_Color COLOR_WALL_UP    = { 240, 240, 240, 255 };
static const SDL_Color COLOR_WALL_RIGHT = { 128, 128, 128, 255 };
static const SDL_Color COLOR_WALL_DOWN  = { 64, 64, 64, 255 };
static const SDL_Color COLOR_WALL_LEFT  = { 32, 32, 32, 255 };

Maze3DRenderer::Maze3DRenderer(SDL_Renderer* _pRenderer, NavigationManager& _oNav, Maze& _oMaze)
    : m_pRenderer(_pRenderer)
    , m_oNav(_oNav)
    , m_oMaze(_oMaze)
{
}

bool Maze3DRenderer::IsValidPosition() const
{
    return m_oMaze.GetCell(m_oNav.GetX(), m_oNav.GetY()) == CellType::Floor;
}

void Maze3DRenderer::RenderImGui()
{
    if (!IsValidPosition())
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.8f);
        ImGui::Begin("Error", nullptr,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize);
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                           "Posicion invalida. Debe especificarse una celda suelo");
        ImGui::End();
    }
}

void Maze3DRenderer::RenderBackground()
{
    if (!IsValidPosition())
    {
        SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(m_pRenderer);
        return;
    }

    int iWidth = 0;
    int iHeight = 0;
    if (!SDL_GetCurrentRenderOutputSize(m_pRenderer, &iWidth, &iHeight) ||
        iWidth <= 0 || iHeight <= 0)
    {
        return;
    }

    DrawView(iWidth, iHeight);
}

void Maze3DRenderer::DrawView(int _iWidth, int _iHeight)
{
    DrawBackground(_iWidth, _iHeight);
    CastRays(_iWidth, _iHeight);
}

void Maze3DRenderer::DrawBackground(int _iWidth, int _iHeight)
{
    int iMidY = _iHeight / 2;

    SDL_FRect oCeilRect = { 0.0f, 0.0f, static_cast<float>(_iWidth), static_cast<float>(iMidY) };
    SDL_SetRenderDrawColor(m_pRenderer, COLOR_CEIL.r, COLOR_CEIL.g, COLOR_CEIL.b, COLOR_CEIL.a);
    SDL_RenderFillRect(m_pRenderer, &oCeilRect);

    SDL_FRect oFloorRect = { 0.0f, static_cast<float>(iMidY), static_cast<float>(_iWidth), static_cast<float>(_iHeight - iMidY) };
    SDL_SetRenderDrawColor(m_pRenderer, COLOR_FLOOR.r, COLOR_FLOOR.g, COLOR_FLOOR.b, COLOR_FLOOR.a);
    SDL_RenderFillRect(m_pRenderer, &oFloorRect);
}

void Maze3DRenderer::CastRays(int _iWidth, int _iHeight)
{
    float fPosX = m_oNav.GetInterpX();
    float fPosY = m_oNav.GetInterpY();
    float fAngle = m_oNav.GetInterpAngle();
    float fRad = fAngle * PI / 180.0f;

    float fDirX = std::sin(fRad);
    float fDirY = -std::cos(fRad);
    float fPlaneX = std::cos(fRad);
    float fPlaneY = std::sin(fRad);

    for (int iX = 0; iX < _iWidth; ++iX)
    {
        float fCameraX = 2.0f * static_cast<float>(iX) / static_cast<float>(_iWidth) - 1.0f;

        float fRayDirX = fDirX + fPlaneX * fCameraX;
        float fRayDirY = fDirY + fPlaneY * fCameraX;

        int iMapX = static_cast<int>(fPosX);
        int iMapY = static_cast<int>(fPosY);

        float fDeltaDistX = (fRayDirX == 0.0f) ? 1e30f : std::abs(1.0f / fRayDirX);
        float fDeltaDistY = (fRayDirY == 0.0f) ? 1e30f : std::abs(1.0f / fRayDirY);

        int iStepX = 0;
        int iStepY = 0;
        float fSideDistX = 0.0f;
        float fSideDistY = 0.0f;

        if (fRayDirX < 0.0f)
        {
            iStepX = -1;
            fSideDistX = (fPosX - static_cast<float>(iMapX)) * fDeltaDistX;
        }
        else
        {
            iStepX = 1;
            fSideDistX = (static_cast<float>(iMapX) + 1.0f - fPosX) * fDeltaDistX;
        }

        if (fRayDirY < 0.0f)
        {
            iStepY = -1;
            fSideDistY = (fPosY - static_cast<float>(iMapY)) * fDeltaDistY;
        }
        else
        {
            iStepY = 1;
            fSideDistY = (static_cast<float>(iMapY) + 1.0f - fPosY) * fDeltaDistY;
        }

        bool bHit = false;
        int iSide = 0;

        while (!bHit)
        {
            if (fSideDistX < fSideDistY)
            {
                fSideDistX += fDeltaDistX;
                iMapX += iStepX;
                iSide = 0;
            }
            else
            {
                fSideDistY += fDeltaDistY;
                iMapY += iStepY;
                iSide = 1;
            }

            if (m_oMaze.GetCell(static_cast<unsigned int>(iMapX), static_cast<unsigned int>(iMapY)) == CellType::Wall)
            {
                bHit = true;
            }
        }

        float fPerpDist = 0.0f;
        if (iSide == 0)
        {
            fPerpDist = (static_cast<float>(iMapX) - fPosX + (1.0f - static_cast<float>(iStepX)) / 2.0f) / fRayDirX;
        }
        else
        {
            fPerpDist = (static_cast<float>(iMapY) - fPosY + (1.0f - static_cast<float>(iStepY)) / 2.0f) / fRayDirY;
        }

        if (fPerpDist < 0.01f)
        {
            fPerpDist = 0.01f;
        }

        int iLineHeight = static_cast<int>(static_cast<float>(_iHeight) / fPerpDist);
        int iDrawStart = -iLineHeight / 2 + _iHeight / 2;
        if (iDrawStart < 0) iDrawStart = 0;
        int iDrawEnd = iLineHeight / 2 + _iHeight / 2;
        if (iDrawEnd >= _iHeight) iDrawEnd = _iHeight - 1;

        SDL_Color oWallColor;
        if (iSide == 0)
        {
            oWallColor = (iStepX > 0) ? COLOR_WALL_LEFT : COLOR_WALL_RIGHT;
        }
        else
        {
            oWallColor = (iStepY > 0) ? COLOR_WALL_UP : COLOR_WALL_DOWN;
        }

        SDL_SetRenderDrawColor(m_pRenderer, oWallColor.r, oWallColor.g, oWallColor.b, oWallColor.a);
        SDL_RenderLine(m_pRenderer,
                       static_cast<float>(iX) + 0.5f, static_cast<float>(iDrawStart),
                       static_cast<float>(iX) + 0.5f, static_cast<float>(iDrawEnd));
    }
}
