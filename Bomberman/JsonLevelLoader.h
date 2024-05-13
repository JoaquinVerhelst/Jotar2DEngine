#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include <vector>
#include <glm/glm.hpp>

namespace Jotar
{
	class GameObject;
	class Scene;


	class JsonLevelLoader final : public Component
	{

	public:

		JsonLevelLoader(GameObject* owner, Scene& scene, const std::string& filePath);
		~JsonLevelLoader() = default;

		bool LoadLevelFromJson(Scene& scene, const std::string& filePath);

		//std::shared_ptr<GameObject> LoadPlayer(int index);

		JsonLevelLoader(const JsonLevelLoader& other) = delete;
		JsonLevelLoader(JsonLevelLoader&& other) = delete;
		JsonLevelLoader& operator=(const JsonLevelLoader& other) = delete;
		JsonLevelLoader& operator=(JsonLevelLoader&& other) = delete;

	private:

		void RandomizeBreakableWalls(int rows, int columns, Scene& scene, int amount, std::vector<glm::ivec2>& spawnCells);
		std::shared_ptr<GameObject> CreateUnbreakableWall(Scene& scene);
		std::shared_ptr<GameObject> CreateBreakableWall(Scene& scene);

		void CreateEnemies(Scene& scene);



		std::string m_JsonFilePath;
	};

}
