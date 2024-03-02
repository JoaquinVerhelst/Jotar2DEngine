#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <algorithm>

dae::GameObject::GameObject(Scene* pScene, const std::string& name)
	: m_pComponents{}
	, m_Name{name}
	, m_pScene{pScene}
	, m_IsDestroyed{ false }
{
	m_pTransform = AddComponent<TransformComponent>();
}

dae::GameObject::~GameObject()
{
	//if (!m_pChildren.empty())
	//{
	//	for (size_t i = 0; i < m_pChildren.size(); i++)
	//	{
	//		RemoveChild(m_pChildren[i]);
	//	}
	//	//m_pChildren.clear();
	//	//std::vector<std::shared_ptr<GameObject>>().swap(m_pChildren);
	//}


}

void dae::GameObject::Update()
{

	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->FixedUpdate();
	}


	for (const auto& pChild : m_pChildren)
	{
		pChild->FixedUpdate();
	}
}

void dae::GameObject::LateUpdate()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}


	for (const auto& pChild : m_pChildren)
	{
		pChild->LateUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Render();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->Render();
	}
}


void dae::GameObject::OnDestroy()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->OnDestroy();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->OnDestroy();
		
	}
}


void dae::GameObject::Destroy()
{
	m_IsDestroyed = true;

	for (const auto& child : m_pChildren)
	{
		child->Destroy();
	}
}


std::shared_ptr<dae::GameObject> dae::GameObject::CreateChildGameObject(const std::string& name)
{
	auto pGameObject{ std::make_shared<GameObject>(m_pScene, name) };

	//pGameObject->GetTransform()->SetPositionDirty();

	pGameObject->SetParent(shared_from_this(), false);

	return pGameObject;
}

void dae::GameObject::SetParent(std::shared_ptr<GameObject> newParent, bool keepWorldPosition)
{
	// choek if valid
	if (newParent == shared_from_this() || m_pParent == newParent)
		return;

	for (size_t i = 0; i < GetChildCount(); i++)
	{
		if (m_pChildren[i] == newParent)
			return;
	}


	if (newParent == nullptr)
		GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition() - newParent->GetTransform()->GetWorldPosition());
		GetTransform()->SetPositionDirty();
	}

	if (m_pParent)
	{
		m_pParent->RemoveChild(shared_from_this());
	}
	m_pParent = newParent;

	if (m_pParent)
	{
		m_pParent->m_pChildren.emplace_back(shared_from_this());
	}
}



std::shared_ptr<dae::GameObject> dae::GameObject::GetChildAt(unsigned int index) const
{
	if (index >= static_cast<int>(m_pChildren.size()))
	{
		throw std::runtime_error("Error: GameObject with the name: " + m_Name + "is getting a child out of index");
	}

	return m_pChildren[index];
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> childToRemove)
{

	if (!childToRemove)
		return;

	if (const auto child = std::ranges::find_if(m_pChildren, [childToRemove](const std::shared_ptr<GameObject>& check)
		{
			return check == childToRemove;
		});
		child != m_pChildren.end())
	{
		m_pChildren.erase(child);

		child->get()->m_pParent = nullptr;

		child->get()->GetTransform()->SetLocalPosition(child->get()->GetTransform()->GetWorldPosition());
	}
}

