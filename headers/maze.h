#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <cstdint>
#include "Orientation.h"

enum class CellType : uint8_t
{
    Wall,
    Floor
};

struct Maze
{
    unsigned int m_uWidth;
    unsigned int m_uHeight;
    std::vector<CellType> m_oCells;

    unsigned int m_uNavX;
    unsigned int m_uNavY;
    Orientation m_eNavOrientation;

    Maze(unsigned int _uWidth, unsigned int _uHeight);

    CellType GetCell(unsigned int _uX, unsigned int _uY) const;
    void SetCell(unsigned int _uX, unsigned int _uY, CellType _eType);
    bool IsInBounds(unsigned int _uX, unsigned int _uY) const;

    void Save(const char* _sFilePath) const;
    static Maze Load(const char* _sFilePath);
};

#endif
