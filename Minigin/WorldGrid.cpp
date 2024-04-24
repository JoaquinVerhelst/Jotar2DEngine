#include "WorldGrid.h"

#include "SDL.h"
#include "Renderer.h"
#include <stdexcept>


void Jotar::WorldGrid::Init(int rows, int columns, int size)
{
    m_Grid.reserve(rows * columns);
    m_GridSize = { rows, columns };
    m_CellSize = size;
    //m_GridSize.x = rows;
    //m_GridSize.y = columns;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            glm::ivec2 IDposition(r, c);

            glm::vec2 worldPosition(r * size, c * size);

            GridCell cell{ worldPosition, size, {r, c} };

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

std::unordered_map<glm::ivec2, Jotar::GridCell>& Jotar::WorldGrid::GetWorldGrid()
{
	return m_Grid;
}


const int Jotar::WorldGrid::GetCellSize() const
{
    return m_CellSize;
}

const glm::vec2& Jotar::WorldGrid::GetGridSize() const
{
    return m_GridSize;
}

const Jotar::GridCell& Jotar::WorldGrid::GetGridCellByID(const glm::ivec2& ID) const
{
    auto it = m_Grid.find(ID);

    if (it != m_Grid.end()) {
        return it->second;
    }

    throw std::runtime_error("const& GetGridCellByID() const: ID not found");
}

Jotar::GridCell& Jotar::WorldGrid::GetGridCellByID(const glm::ivec2& ID)
{
    auto it = m_Grid.find(ID);

    if (it != m_Grid.end()) {
        return it->second;
    }

    throw std::runtime_error("& GetGridCellByID(): ID not found");
}


const Jotar::GridCell& Jotar::WorldGrid::GetGridCellByPosition(const glm::vec2& position) const
{
    for (const auto& cellPair : m_Grid)
    {
        const GridCell& cell = cellPair.second;
        const glm::vec2& cellPos = cell.WorldPosition;

        if (position.x >= cellPos.x && position.x < cellPos.x + m_CellSize &&
            position.y >= cellPos.y && position.y < cellPos.y + m_CellSize)
        {
            return cellPair.second;
        }
    }

    throw std::runtime_error("const& GetGridCellByPosition() const: ID not found");
}

Jotar::GridCell& Jotar::WorldGrid::GetGridCellByPosition(const glm::vec2& position)
{
    for (auto& cellPair : m_Grid)
    {
        const GridCell& cell = cellPair.second;
        const glm::vec2& cellPos = cell.WorldPosition;

        if (position.x >= cellPos.x && position.x < cellPos.x + m_CellSize &&
            position.y >= cellPos.y && position.y < cellPos.y + m_CellSize)
        {
            return cellPair.second;
        }
    }

    throw std::runtime_error("const& GetGridCellByPosition() const: ID not found");
}


std::vector<Jotar::GridCell> Jotar::WorldGrid::GetConnectedCellsFromIndex(glm::ivec2 cellIndex) const
{
    std::vector<GridCell> connectedCells;

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;

            glm::ivec2 neighborIndex(cellIndex.x + i, cellIndex.y + j);

            // Check if the neighbor index is within the grid
            if (m_Grid.find(neighborIndex) != m_Grid.end())
            {
                // Check if the neighbor cell is connected to the current cell
                if (m_Grid.at(neighborIndex).IsConnected(cellIndex))
                {
                    // Add the connected neighbor cell to the list
                    connectedCells.push_back(m_Grid.at(neighborIndex));
                }
            }
        }
    }

    return connectedCells;
}
