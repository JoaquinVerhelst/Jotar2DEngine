#include "AIPerceptionComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include "WorldTimeManager.h"



Jotar::AIPerceptionComponent::AIPerceptionComponent(GameObject* owner, float viewDistance, std::string targetTag)
	:Component(owner)
	, m_pTransformComponent{nullptr}
	, m_pAIBehaviorComponent{ nullptr }
	, m_TargetTag{targetTag}
	, m_ViewDistance{ viewDistance }
	, m_TimeToCheck{0.2f}
	, m_CheckTimer{0}
	, m_SceneCollisionManager{nullptr}
{
	m_pSubject = std::make_unique<Subject<AIEvents>>();
}

Jotar::AIPerceptionComponent::~AIPerceptionComponent()
{
	m_pSubject->RemoveAllObservers();
}

void Jotar::AIPerceptionComponent::Start()
{
	m_pTransformComponent = GetOwner()->GetComponent<TransformComponent>();
	m_pAIBehaviorComponent = GetOwner()->GetComponent<AIBehaviorComponent>();
	m_SceneCollisionManager = &SceneManager::GetInstance().GetCurrentScene().GetCollisionManager();
}

void Jotar::AIPerceptionComponent::CheckIfPotentialTargetIsSeen()
{
	auto dir = m_pAIBehaviorComponent->GetGoToTargetState()->GetCurrentDirection();
	auto pos = m_pTransformComponent->GetWorldPosition();

	if (m_SceneCollisionManager->RayCastIsColliderInRange(pos, dir, m_ViewDistance, m_TargetTag))
	{
		auto futureResult = m_SceneCollisionManager->RayCastCollisionAsync(pos, dir, m_ViewDistance, "Enemy");

		Jotar::ColliderComponent* closestCollider = futureResult.get();

		if (closestCollider == nullptr) return;

		for (size_t i = 0; i < m_TargetTag.size(); i++)
		{
			if (closestCollider->CompareTag(m_TargetTag))
			{
				m_pSubject->NotifyObservers(AIPlayerSeenEvent(closestCollider));
			}
		}
	}
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


