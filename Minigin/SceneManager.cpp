#include "SceneManager.h"
#include "Scene.h"
#include <stdexcept>


void Jotar::SceneManager::Start()
{
	m_CurrentScene->Start();
}

void Jotar::SceneManager::Update()
{
	m_CurrentScene->Update();
}

void Jotar::SceneManager::FixedUpdate()
{
	m_CurrentScene->FixedUpdate();
}

void Jotar::SceneManager::LateUpdate()
{
	m_CurrentScene->LateUpdate();
}

void Jotar::SceneManager::Render()
{
	m_CurrentScene->Render();
}

void Jotar::SceneManager::CleanUpDestroyedObjects()
{
	for (const auto& scene : m_Scenes)
	{
		scene->CleanUpDestroyedObjects();
	}

	for (auto it = m_Scenes.begin(); it != m_Scenes.end(); )
	{
		if ((*it)->GetIsDestroyed())
		{
			it = m_Scenes.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Jotar::Scene& Jotar::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);


	if (m_CurrentScene == nullptr)
		m_CurrentScene = scene.get();


	return *scene;
}

void Jotar::SceneManager::Destroy()
{
	for (auto& scene : m_Scenes)
	{
		scene = nullptr;
	}
}

Jotar::Scene& Jotar::SceneManager::GetCurrentScene() const
{
	return *m_CurrentScene;
}

Jotar::Scene& Jotar::SceneManager::GetSceneByID(int SceneIndex) const
{
	return *m_Scenes[SceneIndex];
}

Jotar::Scene& Jotar::SceneManager::GetSceneByName(const std::string& name)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](const std::shared_ptr<Scene>& scene) {
		return scene->GetName() == name;
		});

	if (it != m_Scenes.end()) {
		return **it;
	}
	else {
		throw std::runtime_error("Scene not found: " + name);
	}

}

void Jotar::SceneManager::DestroyScene(Scene& scene)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&scene](const std::shared_ptr<Scene>& ptr)
		{
			return ptr.get() == &scene;
		});

	if (it != m_Scenes.end())
	{
		it->get()->Destroy();
	}
}


int Jotar::SceneManager::GetCurrentSceneID()
{
	return m_CurrentSceneIndex;
}

Jotar::Scene& Jotar::SceneManager::SetCurrentSceneByName(const std::string& name)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](const std::shared_ptr<Scene>& scene) {
		return scene->GetName() == name;
		});

	if (it != m_Scenes.end()) {
		m_CurrentScene = it->get();
		m_CurrentSceneIndex = static_cast<int>(std::distance(m_Scenes.begin(), it));
		return **it;
	}
	else {
		throw std::runtime_error("Scene not found: " + name);
	}
}

void Jotar::SceneManager::SetCurrentSceneByScene(Scene& scene)
{
	auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&scene](const std::shared_ptr<Scene>& ptr)
		{
			return ptr.get() == &scene;
		});

	if (it != m_Scenes.end())
	{
		m_CurrentScene = it->get();
		m_CurrentSceneIndex = static_cast<int>(std::distance(m_Scenes.begin(), it));
	}
	else {
		throw std::runtime_error("Scene not found: " + scene.GetName());
	}
}
