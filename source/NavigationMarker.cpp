#include <cmath>
#include "NavigationMarker.h"

static constexpr float PI = 3.14159265358979323846f;

NavigationMarker::NavigationMarker(NavigationManager& _oNav)
    : m_oNav(_oNav)
{
}

void NavigationMarker::Render(ImDrawList* _pDrawList, float _fOriginX, float _fOriginY, float _fCellSize, float _fLineWidth) const
{
    float fInterpX = m_oNav.GetInterpX();
    float fInterpY = m_oNav.GetInterpY();
    float fAngle = m_oNav.GetInterpAngle();

    float fCenterX = _fOriginX + _fLineWidth + fInterpX * (_fCellSize + _fLineWidth);
    float fCenterY = _fOriginY + _fLineWidth + fInterpY * (_fCellSize + _fLineWidth);

    float fRad = fAngle * PI / 180.0f;
    float fDirX = std::sin(fRad);
    float fDirY = -std::cos(fRad);
    float fPerpX = std::cos(fRad);
    float fPerpY = std::sin(fRad);

    float fHeight = _fCellSize * 0.75f;
    float fWidth  = _fCellSize * 0.40f;

    ImVec2 oP1(fCenterX + fDirX * fHeight / 2.0f, fCenterY + fDirY * fHeight / 2.0f);
    ImVec2 oP2(fCenterX - fDirX * fHeight / 2.0f - fPerpX * fWidth / 2.0f,
               fCenterY - fDirY * fHeight / 2.0f - fPerpY * fWidth / 2.0f);
    ImVec2 oP3(fCenterX - fDirX * fHeight / 2.0f + fPerpX * fWidth / 2.0f,
               fCenterY - fDirY * fHeight / 2.0f + fPerpY * fWidth / 2.0f);

    _pDrawList->AddTriangleFilled(oP1, oP2, oP3, IM_COL32(0, 0, 0, 255));
}
