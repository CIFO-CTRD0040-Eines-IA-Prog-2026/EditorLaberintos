#include <stdexcept>
#include <fstream>
#include "Maze.h"

Maze::Maze(unsigned int _uWidth, unsigned int _uHeight)
    : m_uWidth(_uWidth)
    , m_uHeight(_uHeight)
    , m_oCells(static_cast<size_t>(_uWidth) * static_cast<size_t>(_uHeight), CellType::Wall)
    , m_uNavX(0)
    , m_uNavY(0)
    , m_eNavOrientation(Orientation::Up)
{
}

CellType Maze::GetCell(unsigned int _uX, unsigned int _uY) const
{
    if (!IsInBounds(_uX, _uY))
    {
        return CellType::Wall;
    }
    return m_oCells[static_cast<size_t>(_uY) * static_cast<size_t>(m_uWidth) + static_cast<size_t>(_uX)];
}

void Maze::SetCell(unsigned int _uX, unsigned int _uY, CellType _eType)
{
    if (!IsInBounds(_uX, _uY))
    {
        return;
    }
    m_oCells[static_cast<size_t>(_uY) * static_cast<size_t>(m_uWidth) + static_cast<size_t>(_uX)] = _eType;
}

bool Maze::IsInBounds(unsigned int _uX, unsigned int _uY) const
{
    return _uX < m_uWidth && _uY < m_uHeight;
}

void Maze::Save(const char* _sFilePath) const
{
    std::ofstream oFile(_sFilePath, std::ios::binary);
    if (!oFile)
    {
        throw std::runtime_error("Failed to open file for writing");
    }

    uint32_t uWidth = static_cast<uint32_t>(m_uWidth);
    uint32_t uHeight = static_cast<uint32_t>(m_uHeight);

    oFile.write(reinterpret_cast<const char*>(&uWidth), sizeof(uWidth));
    oFile.write(reinterpret_cast<const char*>(&uHeight), sizeof(uHeight));
    oFile.write(reinterpret_cast<const char*>(m_oCells.data()),
                static_cast<std::streamsize>(m_oCells.size() * sizeof(CellType)));

    uint32_t uNavX = static_cast<uint32_t>(m_uNavX);
    uint32_t uNavY = static_cast<uint32_t>(m_uNavY);
    uint8_t uNavOrientation = static_cast<uint8_t>(m_eNavOrientation);

    oFile.write(reinterpret_cast<const char*>(&uNavX), sizeof(uNavX));
    oFile.write(reinterpret_cast<const char*>(&uNavY), sizeof(uNavY));
    oFile.write(reinterpret_cast<const char*>(&uNavOrientation), sizeof(uNavOrientation));

    if (!oFile)
    {
        throw std::runtime_error("Failed to write maze data to file");
    }
}

Maze Maze::Load(const char* _sFilePath)
{
    std::ifstream oFile(_sFilePath, std::ios::binary);
    if (!oFile)
    {
        throw std::runtime_error("Failed to open file for reading");
    }

    uint32_t uWidth = 0;
    uint32_t uHeight = 0;

    oFile.read(reinterpret_cast<char*>(&uWidth), sizeof(uWidth));
    oFile.read(reinterpret_cast<char*>(&uHeight), sizeof(uHeight));

    if (!oFile)
    {
        throw std::runtime_error("Failed to read maze dimensions from file");
    }

    Maze oMaze(static_cast<unsigned int>(uWidth), static_cast<unsigned int>(uHeight));
    oFile.read(reinterpret_cast<char*>(oMaze.m_oCells.data()),
               static_cast<std::streamsize>(oMaze.m_oCells.size() * sizeof(CellType)));

    if (!oFile)
    {
        throw std::runtime_error("Failed to read maze cells from file");
    }

    uint32_t uNavX = 0;
    uint32_t uNavY = 0;
    uint8_t uNavOrientation = 0;

    oFile.read(reinterpret_cast<char*>(&uNavX), sizeof(uNavX));
    oFile.read(reinterpret_cast<char*>(&uNavY), sizeof(uNavY));
    oFile.read(reinterpret_cast<char*>(&uNavOrientation), sizeof(uNavOrientation));

    if (oFile)
    {
        oMaze.m_uNavX = static_cast<unsigned int>(uNavX);
        oMaze.m_uNavY = static_cast<unsigned int>(uNavY);
        oMaze.m_eNavOrientation = static_cast<Orientation>(uNavOrientation);
    }

    return oMaze;
}
