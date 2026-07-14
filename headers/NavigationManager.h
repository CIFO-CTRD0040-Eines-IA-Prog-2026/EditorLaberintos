#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

#include <cstdint>
#include "maze.h"

enum class Orientation : uint8_t
{
    Up,
    Down,
    Left,
    Right
};

class NavigationManager
{
public:
    NavigationManager(Maze& _oMaze);

    void Update(uint32_t _uNow);

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
    void TurnLeft();
    void TurnRight();
    void SetRandomFloorPosition();
    void LogAction(const char* _sAction) const;
};

#endif
