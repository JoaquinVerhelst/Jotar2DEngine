#include "SceneManager.h"
#include "Scene.h"
#include <stdexcept>


void Jotar::SceneManager::Start()
{
	m_scenes[m_CurrentSceneIndex]->Start();
}

void Jotar::SceneManager::Update()
{
	m_scenes[m_CurrentSceneIndex]->Update();
}

void Jotar::SceneManager::FixedUpdate()
{
	m_scenes[m_CurrentSceneIndex]->FixedUpdate();
}

void Jotar::SceneManager::LateUpdate()
{
	m_scenes[m_CurrentSceneIndex]->LateUpdate();
}

void Jotar::SceneManager::Render()
{
	m_scenes[m_CurrentSceneIndex]->Render();
}

void Jotar::SceneManager::CleanUpDestroyedObjects()
{
	for (const auto& scene : m_scenes)
	{
		scene->CleanUpDestroyedObjects();
	}
}

Jotar::Scene& Jotar::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	return *scene;
}

void Jotar::SceneManager::Destroy()
{
	for (auto& scene : m_scenes)
	{
		scene = nullptr;
	}
}

Jotar::Scene& Jotar::SceneManager::GetCurrentScene() const
{
	return *m_scenes[m_CurrentSceneIndex];
}

Jotar::Scene& Jotar::SceneManager::GetSceneByID(int SceneIndex) const
{
	return *m_scenes[SceneIndex];
}

Jotar::Scene& Jotar::SceneManager::GetSceneByName(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene>& scene) {
		return scene->GetName() == name;
		});

	if (it != m_scenes.end()) {
		return **it;
	}
	else {
		throw std::runtime_error("Scene not found: " + name);
	}

}

void Jotar::SceneManager::DestroyScene(Scene& scene)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&scene](const std::shared_ptr<Scene>& ptr)
		{
			return ptr.get() == &scene;
		});

	if (it != m_scenes.end())
	{
		m_scenes.erase(it);
	}
}


int Jotar::SceneManager::GetCurrentSceneID()
{
	return m_CurrentSceneIndex;
}

Jotar::Scene& Jotar::SceneManager::SetCurrentSceneByName(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene>& scene) {
		return scene->GetName() == name;
		});

	if (it != m_scenes.end()) {
		m_CurrentSceneIndex = static_cast<int>(std::distance(m_scenes.begin(), it));
		return **it;
	}
	else {
		throw std::runtime_error("Scene not found: " + name);
	}
}

void Jotar::SceneManager::SetCurrentSceneByScene(Scene& scene)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&scene](const std::shared_ptr<Scene>& ptr)
		{
			return ptr.get() == &scene;
		});

	if (it != m_scenes.end())
	{
		m_CurrentSceneIndex = static_cast<int>(std::distance(m_scenes.begin(), it));
	}
	else {
		throw std::runtime_error("Scene not found: " + scene.GetName());
	}
}
