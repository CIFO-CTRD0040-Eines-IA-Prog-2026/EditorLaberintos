#include "NavigationMarker.h"

NavigationMarker::NavigationMarker(NavigationManager& _oNav)
    : m_oNav(_oNav)
{
}

void NavigationMarker::Render(ImDrawList* _pDrawList, float _fOriginX, float _fOriginY, float _fCellSize, float _fLineWidth) const
{
    unsigned int uNavX = m_oNav.GetX();
    unsigned int uNavY = m_oNav.GetY();
    Orientation eOrientation = m_oNav.GetOrientation();

    float fCenterX = _fOriginX + _fLineWidth + uNavX * (_fCellSize + _fLineWidth) + _fCellSize / 2.0f;
    float fCenterY = _fOriginY + _fLineWidth + uNavY * (_fCellSize + _fLineWidth) + _fCellSize / 2.0f;

    float fHeight = _fCellSize * 0.75f;
    float fWidth  = _fCellSize * 0.40f;

    ImVec2 oP1, oP2, oP3;

    switch (eOrientation)
    {
        case Orientation::Up:
            oP1 = ImVec2(fCenterX, fCenterY - fHeight / 2.0f);
            oP2 = ImVec2(fCenterX - fWidth / 2.0f, fCenterY + fHeight / 2.0f);
            oP3 = ImVec2(fCenterX + fWidth / 2.0f, fCenterY + fHeight / 2.0f);
            break;
        case Orientation::Down:
            oP1 = ImVec2(fCenterX, fCenterY + fHeight / 2.0f);
            oP2 = ImVec2(fCenterX - fWidth / 2.0f, fCenterY - fHeight / 2.0f);
            oP3 = ImVec2(fCenterX + fWidth / 2.0f, fCenterY - fHeight / 2.0f);
            break;
        case Orientation::Left:
            oP1 = ImVec2(fCenterX - fHeight / 2.0f, fCenterY);
            oP2 = ImVec2(fCenterX + fHeight / 2.0f, fCenterY - fWidth / 2.0f);
            oP3 = ImVec2(fCenterX + fHeight / 2.0f, fCenterY + fWidth / 2.0f);
            break;
        case Orientation::Right:
            oP1 = ImVec2(fCenterX + fHeight / 2.0f, fCenterY);
            oP2 = ImVec2(fCenterX - fHeight / 2.0f, fCenterY - fWidth / 2.0f);
            oP3 = ImVec2(fCenterX - fHeight / 2.0f, fCenterY + fWidth / 2.0f);
            break;
    }

    _pDrawList->AddTriangleFilled(oP1, oP2, oP3, IM_COL32(0, 0, 0, 255));
}
