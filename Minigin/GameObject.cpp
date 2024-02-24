#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject(Scene* pScene, const std::string& name)
	: m_pComponents{}
	, m_Name{name}
	, m_pScene{pScene}
	, m_IsDestroyed{ false }
	, m_Transform{}
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{

	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->FixedUpdate();
	}
}

void dae::GameObject::LateUpdate()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Render();
	}

}


void dae::GameObject::OnDestroy()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->OnDestroy();
	}

	m_pComponents.clear();
}


void dae::GameObject::Destroy()
{
	m_IsDestroyed = true;
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
