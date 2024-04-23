#include "SceneManager.h"
#include "Scene.h"



void Jotar::SceneManager::Start()
{
	for (auto& scene : m_scenes)
	{
		scene->Start();
	}
}

void Jotar::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void Jotar::SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void Jotar::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void Jotar::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
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

Jotar::Scene& Jotar::SceneManager::GetScene(int SceneIndex) const
{
	return *m_scenes[SceneIndex];
}
