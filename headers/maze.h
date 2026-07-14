#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <cstdint>

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

    Maze(unsigned int _uWidth, unsigned int _uHeight);

    CellType GetCell(unsigned int _uX, unsigned int _uY) const;
    void SetCell(unsigned int _uX, unsigned int _uY, CellType _eType);
    bool IsInBounds(unsigned int _uX, unsigned int _uY) const;

    void Save(const char* _sFilePath) const;
    static Maze Load(const char* _sFilePath);
};

#endif
