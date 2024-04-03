#include "WorldGrid.h"




void Jotar::WorldGrid::Init(int rows, int columns, int size)
{
    m_Grid.reserve(rows * columns);


    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            glm::vec2 IDposition(r, c);

            glm::vec2 worldPosition(r * size, c * size);

            GridCell cell{ worldPosition };

            m_Grid[IDposition] = cell;
        }
    }
}

std::unordered_map<glm::vec2, Jotar::GridCell>& Jotar::WorldGrid::GetWorldGrid()
{
	return m_Grid;
}


const Jotar::GridCell Jotar::WorldGrid::GetGridCellByID(const glm::vec2& ID) const
{
    auto it = m_Grid.find(ID);

    if (it != m_Grid.end()) {
        return it->second;
    }

    return GridCell( glm::vec2( - 1, -1 ));
}


const Jotar::GridCell Jotar::WorldGrid::GetGridCellByPosition(const glm::vec2& position) const
{
    for (const auto& cellPair : m_Grid)
    {
        const GridCell& cell = cellPair.second;
        const glm::vec2& cellPos = cell.worldPosition;

        if (position.x >= cellPos.x && position.x < cellPos.x + m_CellSize &&
            position.y >= cellPos.y && position.y < cellPos.y + m_CellSize)
        {
            return cell;
        }
    }

    return GridCell(glm::vec2(-1, -1));
}