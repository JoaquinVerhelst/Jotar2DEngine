#pragma once
#include "SceneManager.h"

namespace Jotar
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		std::shared_ptr<GameObject> CreateGameObject(std::string objectName);
		void RemoveGameObjectFromRoot(GameObject* GO);
		//void AddGameObjectToRoot(GameObject* GO);
		void CleanUpDestroyedObjects();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;


	private: 
		explicit Scene(const std::string& name);


		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_pObjects{};

		static unsigned int m_idCounter; 
	};

}
