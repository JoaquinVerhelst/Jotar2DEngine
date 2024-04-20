#pragma once

#include <glm/glm.hpp>
#include <memory>
//#include <vector>
#include <unordered_map>
#include "Singleton.h"



namespace std {
	template <>
	struct hash<glm::vec2> {
		size_t operator()(const glm::vec2& v) const {
			// Combine the hash values of the x and y components
			// You can use any suitable hash function here
			return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
		}
	};

	template <>
	struct equal_to<glm::vec2> {
		bool operator()(const glm::vec2& v1, const glm::vec2& v2) const {
			// Compare the x and y components for equality
			return v1.x == v2.x && v1.y == v2.y;
		}
	};
}

namespace Jotar
{

	struct GridCell
	{
		GridCell()
			:WorldPosition{}, CellSize{}, CenterCellPosition{}, Index{} {}

		GridCell(glm::vec2 worldPos, int size, glm::ivec2 index)
			:WorldPosition{ worldPos }, CellSize{ size }, CenterCellPosition{}, Index{ index }
		{
			CenterCellPosition = glm::vec2(worldPos.x + CellSize / 2, worldPos.y + CellSize / 2);
		}

		glm::vec2 WorldPosition;
		int CellSize;
		glm::vec2 CenterCellPosition;
		glm::ivec2 Index;

		enum class WallType
		{
			None,
			Destroyable,
			Undestroyable

		};

		WallType HasWall = WallType::None;
	};




	class WorldGrid : public Singleton<WorldGrid>
	{
	public:

		WorldGrid() = default;
		~WorldGrid() = default;

		void Init(int rows, int columns, int size);
		void Render() const;

		WorldGrid(const WorldGrid& other) = delete;
		WorldGrid(WorldGrid&& other) = delete;
		WorldGrid& operator=(const WorldGrid& other) = delete;
		WorldGrid& operator=(WorldGrid&& other) = delete;

		std::unordered_map<glm::vec2, GridCell>& GetWorldGrid();

		const int GetCellSize() const;
		const glm::vec2& GetGridSize() const;
		const GridCell& GetGridCellByID(const glm::vec2& ID) const;
		GridCell& GetGridCellByID(const glm::vec2& ID);
		const GridCell& GetGridCellByPosition(const glm::vec2& position) const;

	private:

		std::unordered_map<glm::vec2, GridCell> m_Grid;
		glm::vec2 m_GridSize;
		int m_CellSize;
	};
}
