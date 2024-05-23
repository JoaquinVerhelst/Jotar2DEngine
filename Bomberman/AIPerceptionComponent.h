#pragma once

#include "Component.h"
#include "Subject.h"
#include "AIEvents.h"
#include "TransformComponent.h"
#include "AIBehaviorComponent.h"


namespace Jotar
{
	class AIPerceptionComponent final : public Component
	{
	public:
		AIPerceptionComponent(GameObject* owner, float viewDistance, std::vector<std::string> targetTags);
		~AIPerceptionComponent();
		
		void FixedUpdate() override;
		void Update() override;
		void Start() override;


		AIPerceptionComponent(const AIPerceptionComponent& other) = delete;
		AIPerceptionComponent(AIPerceptionComponent&& other) = delete;
		AIPerceptionComponent& operator=(const AIPerceptionComponent& other) = delete;
		AIPerceptionComponent& operator=(AIPerceptionComponent&& other) = delete;


		void AddObserver(Observer<AIPlayerSeenEvent>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<AIPlayerSeenEvent>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}

		void CheckIfPotentialTargetIsSeen();

	private:
		TransformComponent* m_pTransformComponent;
		AIBehaviorComponent* m_pAIBehaviorComponent;

		std::unique_ptr<Subject<AIPlayerSeenEvent>> m_pSubject;

		std::vector<std::string> m_TargetTags;
		float m_ViewDistance;


		float m_TimeToCheck;
		float m_CheckTimer;
	};
}
