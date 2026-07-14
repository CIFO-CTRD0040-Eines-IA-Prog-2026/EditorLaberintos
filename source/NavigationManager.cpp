#include <cstdio>
#include <random>
#include <vector>
#include <utility>
#include <SDL3/SDL.h>
#include <imgui.h>
#include "NavigationManager.h"

NavigationManager::NavigationManager(Maze& _oMaze)
    : m_oMaze(_oMaze)
    , m_uX(0)
    , m_uY(0)
    , m_eOrientation(Orientation::Up)
    , m_uLastMoveTime(0)
    , m_uLastTurnTime(0)
{
    SetRandomFloorPosition();
}

void NavigationManager::Update(uint32_t _uNow)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        m_uLastMoveTime = 0;
        m_uLastTurnTime = 0;
        return;
    }

    const bool* pKeys = SDL_GetKeyboardState(NULL);

    if (pKeys[SDL_SCANCODE_LEFT])
    {
        if (m_uLastTurnTime == 0 || _uNow - m_uLastTurnTime >= COOLDOWN_MS)
        {
            TurnLeft();
            m_uLastTurnTime = _uNow;
        }
    }
    else if (pKeys[SDL_SCANCODE_RIGHT])
    {
        if (m_uLastTurnTime == 0 || _uNow - m_uLastTurnTime >= COOLDOWN_MS)
        {
            TurnRight();
            m_uLastTurnTime = _uNow;
        }
    }
    else
    {
        m_uLastTurnTime = 0;
    }

    if (pKeys[SDL_SCANCODE_UP])
    {
        if (m_uLastMoveTime == 0 || _uNow - m_uLastMoveTime >= COOLDOWN_MS)
        {
            if (NavigateForward())
            {
                m_uLastMoveTime = _uNow;
            }
        }
    }
    else if (pKeys[SDL_SCANCODE_DOWN])
    {
        if (m_uLastMoveTime == 0 || _uNow - m_uLastMoveTime >= COOLDOWN_MS)
        {
            if (NavigateBackward())
            {
                m_uLastMoveTime = _uNow;
            }
        }
    }
    else
    {
        m_uLastMoveTime = 0;
    }
}

bool NavigationManager::NavigateForward()
{
    if (m_oMaze.GetCell(m_uX, m_uY) != CellType::Floor)
    {
        return false;
    }

    int iTargetX = static_cast<int>(m_uX);
    int iTargetY = static_cast<int>(m_uY);

    switch (m_eOrientation)
    {
        case Orientation::Up:    iTargetY -= 1; break;
        case Orientation::Down:  iTargetY += 1; break;
        case Orientation::Left:  iTargetX -= 1; break;
        case Orientation::Right: iTargetX += 1; break;
    }

    if (iTargetX < 0 || iTargetY < 0) return false;

    unsigned int uTargetX = static_cast<unsigned int>(iTargetX);
    unsigned int uTargetY = static_cast<unsigned int>(iTargetY);

    if (!m_oMaze.IsInBounds(uTargetX, uTargetY)) return false;
    if (m_oMaze.GetCell(uTargetX, uTargetY) != CellType::Floor) return false;

    m_uX = uTargetX;
    m_uY = uTargetY;
    LogAction("Move forward");
    return true;
}

bool NavigationManager::NavigateBackward()
{
    if (m_oMaze.GetCell(m_uX, m_uY) != CellType::Floor)
    {
        return false;
    }

    int iTargetX = static_cast<int>(m_uX);
    int iTargetY = static_cast<int>(m_uY);

    switch (m_eOrientation)
    {
        case Orientation::Up:    iTargetY += 1; break;
        case Orientation::Down:  iTargetY -= 1; break;
        case Orientation::Left:  iTargetX += 1; break;
        case Orientation::Right: iTargetX -= 1; break;
    }

    if (iTargetX < 0 || iTargetY < 0) return false;

    unsigned int uTargetX = static_cast<unsigned int>(iTargetX);
    unsigned int uTargetY = static_cast<unsigned int>(iTargetY);

    if (!m_oMaze.IsInBounds(uTargetX, uTargetY)) return false;
    if (m_oMaze.GetCell(uTargetX, uTargetY) != CellType::Floor) return false;

    m_uX = uTargetX;
    m_uY = uTargetY;
    LogAction("Move backward");
    return true;
}

void NavigationManager::TurnLeft()
{
    switch (m_eOrientation)
    {
        case Orientation::Up:    m_eOrientation = Orientation::Left;  break;
        case Orientation::Left:  m_eOrientation = Orientation::Down;  break;
        case Orientation::Down:  m_eOrientation = Orientation::Right; break;
        case Orientation::Right: m_eOrientation = Orientation::Up;    break;
    }
    LogAction("Turn left");
}

void NavigationManager::TurnRight()
{
    switch (m_eOrientation)
    {
        case Orientation::Up:    m_eOrientation = Orientation::Right; break;
        case Orientation::Right: m_eOrientation = Orientation::Down;  break;
        case Orientation::Down:  m_eOrientation = Orientation::Left;  break;
        case Orientation::Left:  m_eOrientation = Orientation::Up;    break;
    }
    LogAction("Turn right");
}

void NavigationManager::SetRandomFloorPosition()
{
    std::vector<std::pair<unsigned int, unsigned int>> oFloorCells;

    for (unsigned int uY = 0; uY < m_oMaze.m_uHeight; ++uY)
    {
        for (unsigned int uX = 0; uX < m_oMaze.m_uWidth; ++uX)
        {
            if (m_oMaze.GetCell(uX, uY) == CellType::Floor)
            {
                oFloorCells.push_back({uX, uY});
            }
        }
    }

    if (!oFloorCells.empty())
    {
        std::random_device oRd;
        std::mt19937 oGen(oRd());
        std::uniform_int_distribution<size_t> oDist(0, oFloorCells.size() - 1);
        auto [uX, uY] = oFloorCells[oDist(oGen)];
        m_uX = uX;
        m_uY = uY;
    }
}

void NavigationManager::LogAction(const char* _sAction) const
{
    const char* sOrientStr = "";
    switch (m_eOrientation)
    {
        case Orientation::Up:    sOrientStr = "Up";    break;
        case Orientation::Down:  sOrientStr = "Down";  break;
        case Orientation::Left:  sOrientStr = "Left";  break;
        case Orientation::Right: sOrientStr = "Right"; break;
    }
    printf("[NavigationManager] %s -> Position(%u, %u) Orientation(%s)\n",
           _sAction, m_uX, m_uY, sOrientStr);
}
