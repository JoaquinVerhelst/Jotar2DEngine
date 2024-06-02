#pragma once
#include "SceneManager.h"
#include "CollisionManager.h"
#include "CameraComponent.h"

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

		std::shared_ptr<GameObject> CreateGameObject(std::string objectName, bool isMovingWithCamera = true);
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		// every Object in scene will be deleted
		void MarkAllForDestroy();

		// every Object except the "dont Destroy On load" Obj in scene will be deleted
		void MarkSceneForDestroy();

		// Transfers the Dont destroy on load obj to the next scene
		void HandleDontDestroyOnLoadObjects(Scene& nextScene);


		void Start();
		void Reset();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		std::shared_ptr<GameObject> GetObjectByName(const std::string& name);

		void RemoveGameObjectFromRoot(GameObject* GO);
		void CleanUpDestroyedObjects();

		CollisionManager& GetCollisionManager();

		void AddCamera(CameraComponent* cameraObj);
		CameraComponent* GetCamera(int cameraIndex) const;
		std::vector<CameraComponent*> GetCameras() const;

		const std::string& GetName() const;

		int GetSceneID() const;

	private: 
		explicit Scene(const std::string& name);


		std::string m_Name;
		CollisionManager m_CollisionManager;
		std::vector<std::shared_ptr<GameObject>> m_pObjects{};

		std::vector<CameraComponent*> m_pCameras;


		static unsigned int m_idCounter; 
	};

}
