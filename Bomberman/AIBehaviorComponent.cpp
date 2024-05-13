#include "AIBehaviorComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"

#include <stdexcept>

#include "SDL.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "AIAnimationControllerComponent.h"
#include <iostream>

Jotar::AIBehaviorComponent::AIBehaviorComponent(GameObject* owner)
	:Component(owner)
	, m_pCurrentState{ nullptr }
	, m_IdleState{ std::make_unique<IdleAIState>(this) }
	, m_GoToTargetState{ std::make_unique<GoToTargetAIState>(this) }
	, m_ChaseTargetState{ std::make_unique<ChaseTargetAIState>(this) }
	, m_CalculateRandomPathState{ std::make_unique<CalculateRandomPathAIState>(this) }
	, m_OnDamageAIState{ std::make_unique<OnDamageAIState>(this) }
	, m_CalculatePathToPlayerState{ std::make_unique<CalculatePathToPlayerAIState>(this) }
	, m_IsPlayerSeen{false}
	, m_pAIAnimationControllerComponent{ nullptr }
	, m_IsDamaged{ false }
{
	m_pCurrentState = m_IdleState.get();
}

void Jotar::AIBehaviorComponent::Start()
{
	if (GetOwner()->HasComponent<AIAnimationControllerComponent>())
		m_pAIAnimationControllerComponent = GetOwner()->GetComponent<AIAnimationControllerComponent>();
}

void Jotar::AIBehaviorComponent::Update()
{
	if (m_pCurrentState == nullptr)
	{
		std::runtime_error("State is nullptr in AIBehaviorComponent");
		return;
	}

	auto newState = m_pCurrentState->OnHandle();
	if (newState == nullptr) return;

	m_pCurrentState->OnExit();
	m_pCurrentState = newState;
	m_pCurrentState->OnEnter();
}

void Jotar::AIBehaviorComponent::Render() const
{
	int size = 16;
	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

	const std::vector<glm::vec2>& path = GetChaseTargetState()->GetPath();
	for (const auto& point : path)
	{
		auto pos = point;

		auto camObj = SceneManager::GetInstance().GetScene(0).GetCamera();

		if (camObj != nullptr)
			pos += camObj->GetOffset();


		SDL_Rect rect = { static_cast<int>(pos.x), static_cast<int>(pos.y), size, size };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}

}

bool Jotar::AIBehaviorComponent::GetIsPlayerSeen() const
{
	return m_IsPlayerSeen;
}

void Jotar::AIBehaviorComponent::OnNotify(const AIPlayerSeenEvent& AiEvent)
{
	if (m_IsPlayerSeen) return;

	if (typeid(AiEvent) == typeid(AIPlayerSeenEvent))
	{
		const AIPlayerSeenEvent& playerSeenEvent = static_cast<const AIPlayerSeenEvent&>(AiEvent);
		std::cout << "PLAYER SEEN" << '\n';
		m_IsPlayerSeen = true;
		GetCalculatePathToPlayerState()->SetTarget(playerSeenEvent.GetTargetCollider());
		m_pCurrentState = GetCalculatePathToPlayerState();
	}

}

void Jotar::AIBehaviorComponent::OnNotify(const HealthEvent& healthEvent)
{
	if (m_IsDamaged) return;

	if (typeid(healthEvent) == typeid(DamageHealthEvent))
	{
		m_IsDamaged = true;
		m_pCurrentState = GetOnDamageAIState();
		m_pCurrentState->OnEnter();
	}
}


