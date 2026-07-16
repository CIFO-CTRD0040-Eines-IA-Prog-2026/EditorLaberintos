#include <cstdio>
#include <cmath>
#include <SDL3/SDL.h>
#include <imgui.h>
#include "NavigationManager.h"

static constexpr float PI = 3.14159265358979323846f;

static float OrientationToDegrees(Orientation _eOrient)
{
    switch (_eOrient)
    {
        case Orientation::Up:    return 0.0f;
        case Orientation::Right: return 90.0f;
        case Orientation::Down:  return 180.0f;
        case Orientation::Left:  return 270.0f;
    }
    return 0.0f;
}

NavigationManager::NavigationManager(Maze& _oMaze)
    : m_oMaze(_oMaze)
    , m_uX(0)
    , m_uY(0)
    , m_eOrientation(Orientation::Up)
    , m_uPrevX(0)
    , m_uPrevY(0)
    , m_ePrevOrientation(Orientation::Up)
    , m_uInterpEndTime(0)
{
    SetCenterFloorPosition();
}

void NavigationManager::FinishInterpolation()
{
    m_uInterpEndTime = 0;
}

bool NavigationManager::IsInterpolating() const
{
    return m_uInterpEndTime != 0 && SDL_GetTicks() < m_uInterpEndTime;
}

float NavigationManager::GetInterpX() const
{
    if (!IsInterpolating())
    {
        return static_cast<float>(m_uX) + 0.5f;
    }

    uint32_t uNow = SDL_GetTicks();
    uint32_t uStart = m_uInterpEndTime - INTERP_DURATION_MS;
    float fProgress = static_cast<float>(uNow - uStart) / static_cast<float>(INTERP_DURATION_MS);

    float fPrev = static_cast<float>(m_uPrevX) + 0.5f;
    float fTarget = static_cast<float>(m_uX) + 0.5f;
    return fPrev + (fTarget - fPrev) * fProgress;
}

float NavigationManager::GetInterpY() const
{
    if (!IsInterpolating())
    {
        return static_cast<float>(m_uY) + 0.5f;
    }

    uint32_t uNow = SDL_GetTicks();
    uint32_t uStart = m_uInterpEndTime - INTERP_DURATION_MS;
    float fProgress = static_cast<float>(uNow - uStart) / static_cast<float>(INTERP_DURATION_MS);

    float fPrev = static_cast<float>(m_uPrevY) + 0.5f;
    float fTarget = static_cast<float>(m_uY) + 0.5f;
    return fPrev + (fTarget - fPrev) * fProgress;
}

float NavigationManager::GetInterpAngle() const
{
    if (!IsInterpolating())
    {
        return OrientationToDegrees(m_eOrientation);
    }

    uint32_t uNow = SDL_GetTicks();
    uint32_t uStart = m_uInterpEndTime - INTERP_DURATION_MS;
    float fProgress = static_cast<float>(uNow - uStart) / static_cast<float>(INTERP_DURATION_MS);

    float fPrevAngle = OrientationToDegrees(m_ePrevOrientation);
    float fTargetAngle = OrientationToDegrees(m_eOrientation);

    float fDiff = fTargetAngle - fPrevAngle;
    while (fDiff > 180.0f) fDiff -= 360.0f;
    while (fDiff < -180.0f) fDiff += 360.0f;

    return fPrevAngle + fDiff * fProgress;
}

void NavigationManager::Update(uint32_t _uNow)
{
    if (m_uInterpEndTime != 0 && _uNow >= m_uInterpEndTime)
    {
        m_uInterpEndTime = 0;
    }

    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    if (IsInterpolating())
    {
        return;
    }

    const bool* pKeys = SDL_GetKeyboardState(NULL);
    bool bShift = pKeys[SDL_SCANCODE_LSHIFT] || pKeys[SDL_SCANCODE_RSHIFT];

    if (bShift)
    {
        if (pKeys[SDL_SCANCODE_LEFT])
        {
            StrafeLeft();
        }
        else if (pKeys[SDL_SCANCODE_RIGHT])
        {
            StrafeRight();
        }

        if (pKeys[SDL_SCANCODE_UP])
        {
            NavigateForward();
        }
        else if (pKeys[SDL_SCANCODE_DOWN])
        {
            NavigateBackward();
        }
    }
    else
    {
        if (pKeys[SDL_SCANCODE_LEFT])
        {
            TurnLeft();
        }
        else if (pKeys[SDL_SCANCODE_RIGHT])
        {
            TurnRight();
        }

        if (pKeys[SDL_SCANCODE_UP])
        {
            NavigateForward();
        }
        else if (pKeys[SDL_SCANCODE_DOWN])
        {
            NavigateBackward();
        }
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

    m_uPrevX = m_uX;
    m_uPrevY = m_uY;
    m_ePrevOrientation = m_eOrientation;
    m_uX = uTargetX;
    m_uY = uTargetY;
    m_uInterpEndTime = SDL_GetTicks() + INTERP_DURATION_MS;
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

    m_uPrevX = m_uX;
    m_uPrevY = m_uY;
    m_ePrevOrientation = m_eOrientation;
    m_uX = uTargetX;
    m_uY = uTargetY;
    m_uInterpEndTime = SDL_GetTicks() + INTERP_DURATION_MS;
    return true;
}

bool NavigationManager::StrafeLeft()
{
    if (m_oMaze.GetCell(m_uX, m_uY) != CellType::Floor)
    {
        return false;
    }

    int iTargetX = static_cast<int>(m_uX);
    int iTargetY = static_cast<int>(m_uY);

    switch (m_eOrientation)
    {
        case Orientation::Up:    iTargetX -= 1; break;
        case Orientation::Down:  iTargetX += 1; break;
        case Orientation::Left:  iTargetY += 1; break;
        case Orientation::Right: iTargetY -= 1; break;
    }

    if (iTargetX < 0 || iTargetY < 0) return false;

    unsigned int uTargetX = static_cast<unsigned int>(iTargetX);
    unsigned int uTargetY = static_cast<unsigned int>(iTargetY);

    if (!m_oMaze.IsInBounds(uTargetX, uTargetY)) return false;
    if (m_oMaze.GetCell(uTargetX, uTargetY) != CellType::Floor) return false;

    m_uPrevX = m_uX;
    m_uPrevY = m_uY;
    m_ePrevOrientation = m_eOrientation;
    m_uX = uTargetX;
    m_uY = uTargetY;
    m_uInterpEndTime = SDL_GetTicks() + INTERP_DURATION_MS;
    return true;
}

bool NavigationManager::StrafeRight()
{
    if (m_oMaze.GetCell(m_uX, m_uY) != CellType::Floor)
    {
        return false;
    }

    int iTargetX = static_cast<int>(m_uX);
    int iTargetY = static_cast<int>(m_uY);

    switch (m_eOrientation)
    {
        case Orientation::Up:    iTargetX += 1; break;
        case Orientation::Down:  iTargetX -= 1; break;
        case Orientation::Left:  iTargetY -= 1; break;
        case Orientation::Right: iTargetY += 1; break;
    }

    if (iTargetX < 0 || iTargetY < 0) return false;

    unsigned int uTargetX = static_cast<unsigned int>(iTargetX);
    unsigned int uTargetY = static_cast<unsigned int>(iTargetY);

    if (!m_oMaze.IsInBounds(uTargetX, uTargetY)) return false;
    if (m_oMaze.GetCell(uTargetX, uTargetY) != CellType::Floor) return false;

    m_uPrevX = m_uX;
    m_uPrevY = m_uY;
    m_ePrevOrientation = m_eOrientation;
    m_uX = uTargetX;
    m_uY = uTargetY;
    m_uInterpEndTime = SDL_GetTicks() + INTERP_DURATION_MS;
    return true;
}

void NavigationManager::TurnLeft()
{
    m_ePrevOrientation = m_eOrientation;
    m_uPrevX = m_uX;
    m_uPrevY = m_uY;

    switch (m_eOrientation)
    {
        case Orientation::Up:    m_eOrientation = Orientation::Left;  break;
        case Orientation::Left:  m_eOrientation = Orientation::Down;  break;
        case Orientation::Down:  m_eOrientation = Orientation::Right; break;
        case Orientation::Right: m_eOrientation = Orientation::Up;    break;
    }

    m_uInterpEndTime = SDL_GetTicks() + INTERP_DURATION_MS;
}

void NavigationManager::TurnRight()
{
    m_ePrevOrientation = m_eOrientation;
    m_uPrevX = m_uX;
    m_uPrevY = m_uY;

    switch (m_eOrientation)
    {
        case Orientation::Up:    m_eOrientation = Orientation::Right; break;
        case Orientation::Right: m_eOrientation = Orientation::Down;  break;
        case Orientation::Down:  m_eOrientation = Orientation::Left;  break;
        case Orientation::Left:  m_eOrientation = Orientation::Up;    break;
    }

    m_uInterpEndTime = SDL_GetTicks() + INTERP_DURATION_MS;
}

void NavigationManager::SetPosition(unsigned int _uX, unsigned int _uY)
{
    if (m_oMaze.IsInBounds(_uX, _uY) && m_oMaze.GetCell(_uX, _uY) == CellType::Floor)
    {
        FinishInterpolation();
        m_uX = _uX;
        m_uY = _uY;
    }
}

void NavigationManager::SetOrientation(Orientation _eOrientation)
{
    m_eOrientation = _eOrientation;
}

void NavigationManager::ResetPosition()
{
    FinishInterpolation();
    SetCenterFloorPosition();
}

void NavigationManager::SetCenterFloorPosition()
{
    m_uX = m_oMaze.m_uWidth / 2;
    m_uY = m_oMaze.m_uHeight / 2;
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
