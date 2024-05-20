#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include <vector>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>



namespace Jotar
{
	class Scene;
	class GameObject;
	class Font;

	class JsonLevelLoader final
	{

		struct GeneralLevelInfo
		{
			int Rows;
			int Columns;
			int CellSize;
			int ScreenWidth;
			int ScreenHeight;
			float UIPercent;
		};

	public:

		JsonLevelLoader() = default;
		~JsonLevelLoader() = default;

		bool InitGame();


		bool LoadLevelFromJson(Scene& scene, int level, bool isGameModeInit);
		bool LoadMenuFromJson(Scene& scene);

		void SetGameLevelsFilePath(std::string filePath);


		JsonLevelLoader(const JsonLevelLoader& other) = delete;
		JsonLevelLoader(JsonLevelLoader&& other) = delete;
		JsonLevelLoader& operator=(const JsonLevelLoader& other) = delete;
		JsonLevelLoader& operator=(JsonLevelLoader&& other) = delete;

	private:


		void RandomizeBreakableWalls(int rows, int columns, Scene& scene, int amount, std::vector<glm::ivec2>& spawnCells);
		std::shared_ptr<GameObject> CreateUnbreakableWall(Scene& scene);
		std::shared_ptr<GameObject> CreateBreakableWall(Scene& scene);

		bool CreateGameMode(Scene& scene, GeneralLevelInfo& levelInfo);
		void UpdateCamera(Scene& scene, GeneralLevelInfo& levelInfo);

		std::shared_ptr<GameObject> CreatePlayer(Scene& scene, const nlohmann::json& gameInfo, std::shared_ptr<GameObject> HUD, std::shared_ptr<Font> font, int cellSize, unsigned int playerIndex);
		void CreateBalloomPlayer(Scene& scene);
		void CreateEnemies(Scene& scene, const nlohmann::json& EnemyTypeInfo, GeneralLevelInfo& levelInfo);

		void PlaceEnemyRandomly(std::shared_ptr<GameObject> enemy, GeneralLevelInfo& levelInfo);

		std::string m_GameLevelsFilePath;

	};
}
