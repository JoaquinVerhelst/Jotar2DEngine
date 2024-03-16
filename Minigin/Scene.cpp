#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace Jotar;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}



Scene::~Scene() = default;



void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_pObjects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), object), m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
}

void Jotar::Scene::FixedUpdate()
{
	for (auto& object : m_pObjects)
	{
		object->FixedUpdate();
	}
}

void Jotar::Scene::LateUpdate()
{

	for (auto& object : m_pObjects)
	{
		object->LateUpdate();
	}

}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

std::shared_ptr<GameObject> Jotar::Scene::CreateGameObject(std::string objectName)
{

	auto gameObject = std::make_shared<Jotar::GameObject>(this, objectName);

	Add(gameObject);

	return gameObject;
}

void Jotar::Scene::RemoveGameObjectFromRoot(GameObject* object)
{
	auto foundObj = std::find_if(m_pObjects.begin(), m_pObjects.end(), [&](const auto& p) { return p.get() == object; });

	if (foundObj != m_pObjects.end())
	{
		m_pObjects.erase(foundObj);
	}
}


//void Jotar::Scene::AddGameObjectToRoot(GameObject* GO)
//{
//}
//



void Jotar::Scene::CleanUpDestroyedObjects()
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i]->IsDestroyed())
		{
			m_pObjects[i]->OnDestroy();
			Remove(m_pObjects[i]);
		}
	}

}

