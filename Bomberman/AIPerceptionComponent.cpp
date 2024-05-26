#include "AIPerceptionComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include "WorldTimeManager.h"


// todo remove
#include "SDL.h"
#include "Renderer.h"
#include <iostream>


Jotar::AIPerceptionComponent::AIPerceptionComponent(GameObject* owner, float viewDistance, std::vector<std::string> targetTags)
	:Component(owner)
	, m_pTransformComponent{nullptr}
	, m_pAIBehaviorComponent{ nullptr }
	, m_TargetTags{targetTags}
	, m_ViewDistance{ viewDistance }
	, m_TimeToCheck{0.2f}
	, m_CheckTimer{0}
{
	m_pSubject = std::make_unique<Subject<AIPlayerSeenEvent>>();
}

Jotar::AIPerceptionComponent::~AIPerceptionComponent()
{
	m_pSubject->RemoveAllObservers();
}

void Jotar::AIPerceptionComponent::Start()
{
	m_pTransformComponent = GetOwner()->GetComponent<TransformComponent>();
	m_pAIBehaviorComponent = GetOwner()->GetComponent<AIBehaviorComponent>();
}

void Jotar::AIPerceptionComponent::CheckIfPotentialTargetIsSeen()
{
	auto dir = m_pAIBehaviorComponent->GetGoToTargetState()->GetCurrentDirection();
	auto pos = m_pTransformComponent->GetWorldPosition();

	ColliderComponent* collider = SceneManager::GetInstance().GetCurrentScene().GetCollisionManager().RaycastLookForCollider(pos, dir, m_ViewDistance, m_TargetTags);

	if (collider == nullptr) return;

	for (size_t i = 0; i < m_TargetTags.size(); i++)
	{
		if (collider->CompareTag(m_TargetTags[i]))
		{

			std::cout << "PlayerSeen" << '\n';
			m_pSubject->NotifyObservers(AIPlayerSeenEvent(collider));
			
		}
	}
}

void Jotar::AIPerceptionComponent::FixedUpdate()
{

}

void Jotar::AIPerceptionComponent::Update()
{	
	m_CheckTimer += WorldTimeManager::GetInstance().GetDeltaTime();

	if (m_CheckTimer >= m_TimeToCheck)
	{
		CheckIfPotentialTargetIsSeen();
		m_CheckTimer = 0;

	}

}


