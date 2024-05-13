#include "AIPerceptionComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"

Jotar::AIPerceptionComponent::AIPerceptionComponent(GameObject* owner, float viewDistance, std::vector<std::string> targetTags)
	:Component(owner)
	, m_pTransformComponent{nullptr}
	, m_pAIBehaviorComponent{ nullptr }
	, m_TargetTags{targetTags}
	, m_ViewDistance{ viewDistance }
{
	m_pSubject = std::make_unique<Subject<AIPlayerSeenEvent>>();
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

	ColliderComponent* collider = SceneManager::GetInstance().GetScene(0).GetCollisionManager().RaycastLookForCollider(pos, dir, m_ViewDistance, m_TargetTags);

	if (collider != nullptr)
	{
		m_pSubject->NotifyObservers(AIPlayerSeenEvent(collider));
	}
}

void Jotar::AIPerceptionComponent::FixedUpdate()
{
	CheckIfPotentialTargetIsSeen();
}


