#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

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

    unsigned int GetX() const { return m_uX; }
    unsigned int GetY() const { return m_uY; }
    Orientation GetOrientation() const { return m_eOrientation; }

private:
    Maze& m_oMaze;
    unsigned int m_uX;
    unsigned int m_uY;
    Orientation m_eOrientation;

    uint32_t m_uLastMoveTime;
    uint32_t m_uLastTurnTime;
    static constexpr uint32_t COOLDOWN_MS = 250;

    bool NavigateForward();
    bool NavigateBackward();
    bool StrafeLeft();
    bool StrafeRight();
    bool StrafeUp();
    bool StrafeDown();
    void TurnLeft();
    void TurnRight();
    void SetCenterFloorPosition();
    void LogAction(const char* _sAction) const;
};

#endif
