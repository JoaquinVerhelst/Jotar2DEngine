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


Jotar::AIPerceptionComponent::AIPerceptionComponent(GameObject* owner, float viewDistance, std::string targetTag)
	:Component(owner)
	, m_pTransformComponent{nullptr}
	, m_pAIBehaviorComponent{ nullptr }
	, m_TargetTag{targetTag}
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

	auto& collsiionManager = SceneManager::GetInstance().GetCurrentScene().GetCollisionManager();

	if (collsiionManager.RayCastIsColliderInRange(pos, dir, m_ViewDistance, m_TargetTag))
	{
		auto futureResult = SceneManager::GetInstance().GetCurrentScene().GetCollisionManager().RayCastCollisionAsync(pos, dir, m_ViewDistance, "Enemy");

		Jotar::ColliderComponent* closestCollider = futureResult.get();

		if (closestCollider == nullptr) return;


		std::cout << "Seen:" << closestCollider->GetOwner()->GetName() << '\n';

		for (size_t i = 0; i < m_TargetTag.size(); i++)
		{
			if (closestCollider->CompareTag(m_TargetTag))
			{
				std::cout << "PlayerSeen" << '\n';
				m_pSubject->NotifyObservers(AIPlayerSeenEvent(closestCollider));

			}
		}
	}
}

void Jotar::AIPerceptionComponent::FixedUpdate()
{

}

void Jotar::AIPerceptionComponent::Render() const
{
	// Perform raycast
	auto rayDir = m_pAIBehaviorComponent->GetGoToTargetState()->GetCurrentDirection();
	glm::ivec2 rayStart = static_cast<glm::ivec2>(m_pTransformComponent->GetWorldPosition());
	int rayDistance = static_cast<int>(m_ViewDistance);


	// Draw ray
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawLine(Renderer::GetInstance().GetSDLRenderer(), rayStart.x, rayStart.y, rayStart.x + rayDir.x * rayDistance, rayStart.y + rayDir.y * rayDistance);

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


