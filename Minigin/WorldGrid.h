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
		GridCell() = default;
		GridCell(glm::vec2 worldPos)
		{
			worldPosition = worldPos;
		}

		glm::vec2 worldPosition;
	};




	class WorldGrid : public Singleton<WorldGrid>
	{
	public:

		WorldGrid() = default;
		~WorldGrid() = default;

		void Init(int rows, int columns, int size);

		WorldGrid(const WorldGrid& other) = delete;
		WorldGrid(WorldGrid&& other) = delete;
		WorldGrid& operator=(const WorldGrid& other) = delete;
		WorldGrid& operator=(WorldGrid&& other) = delete;



		std::unordered_map<glm::vec2, GridCell>& GetWorldGrid();

		const GridCell GetGridCellByID(const glm::vec2& ID) const;
		const GridCell GetGridCellByPosition(const glm::vec2& position) const;

	private:

		std::unordered_map<glm::vec2, GridCell> m_Grid;

		int m_CellSize;



	};
}
