#include "AIPerceptionComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include <glm/glm.hpp>

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
	, m_TimeToCheck{0}
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
	CheckIfPotentialTargetIsSeen();
}

void Jotar::AIPerceptionComponent::Update()
{	
	
	auto dir = m_pAIBehaviorComponent->GetGoToTargetState()->GetCurrentDirection();
	auto pos = m_pTransformComponent->GetWorldPosition();

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

	// Calculate the end position of the ray
	glm::vec2 endPos = pos + static_cast<glm::vec2>(dir) * m_ViewDistance;
	endPos += pos;
	endPos *= m_ViewDistance;

	// Set the draw color for the ray (e.g., red)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	// Draw the line
	SDL_RenderDrawLine(renderer, static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y));

	// Reset the draw color to white (or whatever your default is)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

}


