#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"

#include <algorithm>

Jotar::GameObject::GameObject(Scene* pScene, const std::string& name, bool isMovingWithCamera)
	: m_pComponents{}
	, m_Name{name}
	, m_pScene{pScene}
	, m_IsDestroyed{ false }
	, m_pParent{nullptr}
	, m_pChildren{}
	, m_DestroyOnLoad{ true }
{
	m_pTransform = AddComponent<TransformComponent>(isMovingWithCamera);
}

Jotar::GameObject::~GameObject()
{
	m_pChildren.clear();
}

void Jotar::GameObject::Start()
{
	for (const auto& pComponent : m_pComponents)
	{
		if (!pComponent->IsDisabled())
			pComponent->Start();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->Start();
	}
}

void Jotar::GameObject::Reset()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Reset();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->Reset();
	}
}

void Jotar::GameObject::Update()
{

	for (const auto& pComponent : m_pComponents)
	{
		if (!pComponent->IsDisabled())
			pComponent->Update();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void Jotar::GameObject::FixedUpdate()
{
	for (const auto& pComponent : m_pComponents)
	{
		if (!pComponent->IsDisabled())
			pComponent->FixedUpdate();
	}


	for (const auto& pChild : m_pChildren)
	{
		pChild->FixedUpdate();
	}
}

void Jotar::GameObject::LateUpdate()
{
	for (const auto& pComponent : m_pComponents)
	{
		if (!pComponent->IsDisabled())
			pComponent->LateUpdate();
	}


	for (const auto& pChild : m_pChildren)
	{
		pChild->LateUpdate();
	}
}

void Jotar::GameObject::Render() const
{
	for (const auto& pComponent : m_pComponents)
	{
		if (!pComponent->IsDisabled())	
			pComponent->Render();
	}

	for (const auto& pChild : m_pChildren)
	{
		pChild->Render();
	}
}


void Jotar::GameObject::OnDestroy()
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


void Jotar::GameObject::Destroy()
{
	m_IsDestroyed = true;

	for (const auto& child : m_pChildren)
	{
		child->Destroy();
	}
}


std::shared_ptr<Jotar::GameObject> Jotar::GameObject::CreateChildGameObject(const std::string& name, bool keepWorldPosition, bool isMovingWithCamera)
{
	auto pGameObject{ std::make_shared<GameObject>(m_pScene, name, isMovingWithCamera) };

	//pGameObject->GetTransform()->SetPositionDirty();

	pGameObject->SetParent(this, keepWorldPosition);

	return pGameObject;
}

void Jotar::GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
{
	// choek if valid
	if (newParent == this|| m_pParent == newParent)
		return;

	for (size_t i = 0; i < GetChildCount(); i++)
	{
		if (m_pChildren[i].get() == newParent)
			return;
	}

	if (m_pParent)
	{
		GetTransform()->SetPosition(GetTransform()->GetWorldPosition());

		m_pParent->RemoveChild(shared_from_this());
	}

	GetTransform()->UpdateWorldPosition();


	m_pParent = newParent;

	if (m_pParent)
	{
		m_pParent->m_pChildren.emplace_back(shared_from_this());
	}


	if (newParent == nullptr)
	{
		GetTransform()->SetPosition(GetTransform()->GetWorldPosition());
		m_pScene->Add(shared_from_this());
	}
	else
	{
		if (keepWorldPosition)
			GetTransform()->SetPosition(GetTransform()->GetLocalPosition() - newParent->GetTransform()->GetWorldPosition());
		GetTransform()->SetPositionDirty();


		if (!m_pParent)
		{
			m_pScene->RemoveGameObjectFromRoot(this);
		}
	}


}



std::shared_ptr<Jotar::GameObject> Jotar::GameObject::GetChildAt(unsigned int index) const
{
	if (index >= static_cast<int>(m_pChildren.size()))
	{
		throw std::runtime_error("Error: GameObject with the name: " + m_Name + "is getting a child out of index");
	}

	return m_pChildren[index];
}

void Jotar::GameObject::RemoveChild(std::shared_ptr<GameObject> childToRemove)
{

	if (!childToRemove)
		return;

	if (const auto child = std::ranges::find_if(m_pChildren, [childToRemove](const std::shared_ptr<GameObject>& check)
		{
			return check == childToRemove;
		});
		child != m_pChildren.end())
	{
		std::shared_ptr<GameObject> removedChild = std::move(*child);
		m_pChildren.erase(child);

		removedChild->m_pParent = nullptr;
		removedChild->GetTransform()->SetPosition(removedChild->GetTransform()->GetWorldPosition());
	}
}

