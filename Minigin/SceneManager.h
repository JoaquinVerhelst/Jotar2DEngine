#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace Jotar
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render();
		void CleanUpDestroyedObjects();

		void Destroy();

		Scene& SetCurrentSceneByName(const std::string& name);
		void SetCurrentSceneByScene(Scene& scene);


		Scene& GetCurrentScene() const;
		Scene& GetSceneByID(int SceneIndex) const;
		Scene& GetSceneByName(const std::string& name);

		void DestroyScene(Scene& scene);


		int GetCurrentSceneID();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		int m_CurrentSceneIndex{};

	};
}
