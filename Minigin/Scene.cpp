#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include <iostream>


using namespace Jotar;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_CollisionManager()

{}



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

void Jotar::Scene::MarkAllForDestroy()
{
	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->Destroy();
	}
}

void Jotar::Scene::MarkSceneForDestroy()
{
	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		if (m_pObjects[i]->IsDestroyOnLoad())
			m_pObjects[i]->Destroy();
	}
}

void Jotar::Scene::HandleDontDestroyOnLoadObjects(Scene& nextScene)
{
	std::vector<std::shared_ptr<GameObject>> objectsForNextScene{};

	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		if (!m_pObjects[i]->IsDestroyOnLoad())
		{
			objectsForNextScene.emplace_back(m_pObjects[i]);
		}
	}

	for (size_t i = 0; i < objectsForNextScene.size(); i++)
	{
		nextScene.Add(std::move(objectsForNextScene[i]));
	}


	MarkSceneForDestroy();
}

void Jotar::Scene::Start()
{
	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->Start();
	}
}

void Jotar::Scene::Reset()
{
	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->Reset();
	}

	m_pCameras.clear();
	m_CollisionManager.Reset();
}

void Scene::Update()
{	
	

	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->Update();
	}
}

void Jotar::Scene::FixedUpdate()
{
	m_CollisionManager.FixedUpdate();

	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->FixedUpdate();
	}	
	
	


}

void Jotar::Scene::LateUpdate()
{
	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->LateUpdate();
	}
}

void Scene::Render() const
{
	for (size_t i = 0; i < m_pObjects.size(); ++i)
	{
		m_pObjects[i]->Render();
	}
}

std::shared_ptr<GameObject> Jotar::Scene::GetObjectByName(const std::string& name) 
{
	auto it = std::find_if(m_pObjects.begin(), m_pObjects.end(),
		[&name](const std::shared_ptr<GameObject>& obj) {
			return obj->GetName() == name;
		});

	if (it != m_pObjects.end()) {
		return *it;
	}

	return nullptr;
}

std::shared_ptr<GameObject> Jotar::Scene::CreateGameObject(std::string objectName, bool isMovingWithCamera)
{

	auto gameObject = std::make_shared<Jotar::GameObject>(this, objectName, isMovingWithCamera);

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

CollisionManager& Jotar::Scene::GetCollisionManager()
{
	return m_CollisionManager;
}

void Jotar::Scene::AddCamera(CameraComponent* cameraObj)
{
	m_pCameras.emplace_back(cameraObj);
}

CameraComponent* Jotar::Scene::GetCamera(int cameraIndex) const
{
	return m_pCameras[cameraIndex];
}

std::vector<CameraComponent*> Jotar::Scene::GetCameras() const
{
	return m_pCameras;
}

const std::string& Jotar::Scene::GetName() const
{
	return m_Name;
}

int Jotar::Scene::GetSceneID() const
{
	return m_idCounter;
}

