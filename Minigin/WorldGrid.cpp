#include "WorldGrid.h"

#include "SDL.h"
#include "Renderer.h"

void Jotar::WorldGrid::Init(int rows, int columns, int size)
{
    m_Grid.reserve(rows * columns);

    m_CellSize = size;
    //m_GridSize.x = rows;
    //m_GridSize.y = columns;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            glm::vec2 IDposition(r, c);

            glm::vec2 worldPosition(r * size, c * size);

            GridCell cell{ worldPosition, size };

            m_Grid[IDposition] = cell;
        }
    }
}

void Jotar::WorldGrid::Render() const
{
    int index = 0;

    for (const auto& cellPair : m_Grid)
    {
        const GridCell& cell = cellPair.second;
        const glm::vec2& cellPos = cell.WorldPosition;

        SDL_Rect rect = { static_cast<int>(cellPos.x) ,static_cast<int>( cellPos.y), m_CellSize, m_CellSize};
        if (index % 2)
            SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255);

        SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);

        ++index;
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

    return GridCell( glm::vec2( - 1, -1 ), 0);
}


const Jotar::GridCell Jotar::WorldGrid::GetGridCellByPosition(const glm::vec2& position) const
{
    for (const auto& cellPair : m_Grid)
    {
        const GridCell& cell = cellPair.second;
        const glm::vec2& cellPos = cell.WorldPosition;

        if (position.x >= cellPos.x && position.x < cellPos.x + m_CellSize &&
            position.y >= cellPos.y && position.y < cellPos.y + m_CellSize)
        {
            return cell;
        }
    }

    return GridCell(glm::vec2(-1, -1), 0);
}