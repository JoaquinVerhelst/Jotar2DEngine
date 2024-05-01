#pragma once
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Camera.h"

namespace Jotar
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<GameObject> CreateGameObject(std::string objectName);
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void RemoveGameObjectFromRoot(GameObject* GO);
		void CleanUpDestroyedObjects();

		CollisionManager& GetCollisionManager();

		void SetCamera(Camera* cameraObj);
		Camera* GetCamera() const;

	private: 
		explicit Scene(const std::string& name);


		std::string m_Name;
		CollisionManager m_CollisionManager;
		std::vector < std::shared_ptr<GameObject>> m_pObjects{};

		Camera* m_pCameraObject;


		static unsigned int m_idCounter; 
	};

}
