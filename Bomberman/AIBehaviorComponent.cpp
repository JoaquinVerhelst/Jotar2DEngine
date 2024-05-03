#include "AIBehaviorComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"

#include <stdexcept>

#include "SDL.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

Jotar::AIBehaviorComponent::AIBehaviorComponent(GameObject* owner)
	:Component(owner)
	, m_pCurrentState{ nullptr }
	, m_IdleState{ std::make_unique<IdleAIState>(this) }
	, m_GoToTargetState{ std::make_unique<GoToTargetAIState>(this) }
	, m_ChaseTargetState{ std::make_unique<ChaseTargetAIState>(this) }
	, m_CalculatePathState{ std::make_unique<CalculateNextTargetAIState>(this) }
	, m_DeadState{ std::make_unique<DeadAIState>(this) }
	, m_IsPlayerSeen{false}
{
	m_pCurrentState = m_IdleState.get();
}

void Jotar::AIBehaviorComponent::Update()
{
	if (m_pCurrentState == nullptr)
	{
		std::runtime_error("State is nullptr -> Something is WRONG in this");
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


