#pragma once
#include <string>
#include <memory>


namespace Jotar
{
	class GameObject;
	class Scene;


	class JsonLevelLoader final
	{

	public:

		JsonLevelLoader() = default;
		~JsonLevelLoader() = default;

		bool LoadLevelsFromJson(Scene& scene, const std::string& filePath);

		//std::shared_ptr<GameObject> LoadPlayer(int index);


	private:


		std::shared_ptr<GameObject> CreateUnbreakableWall(Scene& scene);

		std::string m_JsonFilePath;
	};

}
