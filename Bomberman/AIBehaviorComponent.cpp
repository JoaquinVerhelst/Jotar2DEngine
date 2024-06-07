#include "AIBehaviorComponent.h"
#include "MovementComponent.h"
#include "GameObject.h"

#include <stdexcept>

#include "SDL.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "AIAnimationControllerComponent.h"
#include "PlayerDeathComponent.h"
#include "AIPerceptionComponent.h"



Jotar::AIBehaviorComponent::AIBehaviorComponent(GameObject* owner)
	:Component(owner)
	, m_pCurrentState{ nullptr }
	, m_pIdleState{ std::make_unique<IdleAIState>(this) }
	, m_pGoToTargetState{ std::make_unique<GoToTargetAIState>(this) }
	, m_pChaseTargetState{ std::make_unique<ChaseTargetAIState>(this, 2.0f) }
	, m_pCalculateRandomPathState{ std::make_unique<CalculateRandomPathAIState>(this, 3) }
	, m_pCalculatePathToPlayerState{ std::make_unique<CalculatePathToPlayerAIState>(this) }
	, m_pOnDamageState{ std::make_unique<OnDamageAIState>(this) }
	, m_IsPlayerSeen{false}
	, m_pAIAnimationControllerComponent{ nullptr }
	, m_pAIPerceptionComponent{ nullptr }
	, m_IsDamaged{ false }
{
	m_pCurrentState = m_pIdleState.get();
	m_pSubject = std::make_unique<Subject<AIDeathEvent>>();
}

Jotar::AIBehaviorComponent::~AIBehaviorComponent()
{
	m_pSubject->RemoveAllObservers();
}

void Jotar::AIBehaviorComponent::Start()
{
	if (GetOwner()->HasComponent<AIAnimationControllerComponent>())
		m_pAIAnimationControllerComponent = GetOwner()->GetComponent<AIAnimationControllerComponent>();

	if (GetOwner()->HasComponent<AIPerceptionComponent>())
		m_pAIPerceptionComponent = GetOwner()->GetComponent<AIPerceptionComponent>();

	
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

bool Jotar::AIBehaviorComponent::GetIsPlayerSeen() const
{
	return m_IsPlayerSeen;
}

void Jotar::AIBehaviorComponent::OnNotify(const AIEvents& AiEvent)
{
	if (!m_IsPlayerSeen)
	{
		if (typeid(AiEvent) == typeid(AIPlayerSeenEvent))
		{
			const AIPlayerSeenEvent& playerSeenEvent = static_cast<const AIPlayerSeenEvent&>(AiEvent);

			if (playerSeenEvent.GetTargetCollider()->GetOwner()->GetComponent<PlayerHealthComponent>()->GetIsDeath())
				return;

			m_IsPlayerSeen = true;
			GetCalculatePathToPlayerState()->SetTarget(playerSeenEvent.GetTargetCollider());
			m_pCurrentState = GetCalculatePathToPlayerState();

			if (m_pAIPerceptionComponent != nullptr)
				m_pAIPerceptionComponent->SetIsDisabled(true);
		}
	}
}

void Jotar::AIBehaviorComponent::OnNotify(const Event& healthEvent)
{

	if (m_IsDamaged) return;


	if (m_IsPlayerSeen)
	{
		if (typeid(healthEvent) == typeid(OnPlayerDeathEvent))
		{
			m_IsPlayerSeen = false;
			m_pCurrentState = GetCalculateRandomPathState();

			if (m_pAIPerceptionComponent != nullptr)
				m_pAIPerceptionComponent->SetIsDisabled(false);
		}

	}

	const DamageHealthEvent* damageEvent = dynamic_cast<const DamageHealthEvent*>(&healthEvent);
	if (damageEvent)
	{
		m_IsDamaged = true;
		m_pOnDamageState->Initialize(damageEvent->GetAttacker(), 2.0f);

		m_pCurrentState = m_pOnDamageState.get();
		m_pCurrentState->OnEnter();
	}
}


