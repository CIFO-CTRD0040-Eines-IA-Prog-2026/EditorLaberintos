#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

#include <cstdint>
#include "Orientation.h"
#include "Maze.h"

class NavigationManager
{
public:
    NavigationManager(Maze& _oMaze);

    void Update(uint32_t _uNow);
    void SetPosition(unsigned int _uX, unsigned int _uY);
    void SetOrientation(Orientation _eOrientation);
    void ResetPosition();
    void FinishInterpolation();

    unsigned int GetX() const { return m_uX; }
    unsigned int GetY() const { return m_uY; }
    Orientation GetOrientation() const { return m_eOrientation; }

    float GetInterpX() const;
    float GetInterpY() const;
    float GetInterpAngle() const;
    bool IsInterpolating() const;

private:
    Maze& m_oMaze;
    unsigned int m_uX;
    unsigned int m_uY;
    Orientation m_eOrientation;

    unsigned int m_uPrevX;
    unsigned int m_uPrevY;
    Orientation m_ePrevOrientation;
    uint32_t m_uInterpEndTime;

    static constexpr uint32_t INTERP_DURATION_MS = 200;

    bool NavigateForward();
    bool NavigateBackward();
    bool StrafeLeft();
    bool StrafeRight();
    void TurnLeft();
    void TurnRight();
    void SetCenterFloorPosition();
    void LogAction(const char* _sAction) const;
};

#endif
