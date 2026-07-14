#ifndef NAVIGATIONMARKER_H
#define NAVIGATIONMARKER_H

#include <imgui.h>
#include "NavigationManager.h"

class NavigationMarker
{
public:
    NavigationMarker(NavigationManager& _oNav);

    void Render(ImDrawList* _pDrawList, float _fOriginX, float _fOriginY, float _fCellSize, float _fLineWidth) const;

private:
    NavigationManager& m_oNav;
};

#endif
