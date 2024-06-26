#pragma once

#include "Component.h"
#include "Subject.h"
#include "AIEvents.h"
#include "TransformComponent.h"
#include "AIBehaviorComponent.h"


namespace Jotar
{
	class CollisionManager;


	class AIPerceptionComponent final : public Component
	{
	public:
		AIPerceptionComponent(GameObject* owner, float viewDistance, std::string targetTag);
		~AIPerceptionComponent();
		

		void Update() override;
		void Start() override;


		AIPerceptionComponent(const AIPerceptionComponent& other) = delete;
		AIPerceptionComponent(AIPerceptionComponent&& other) = delete;
		AIPerceptionComponent& operator=(const AIPerceptionComponent& other) = delete;
		AIPerceptionComponent& operator=(AIPerceptionComponent&& other) = delete;


		void AddObserver(Observer<AIEvents>* pObserver)
		{
			m_pSubject->AddObserver(pObserver);
		}
		void RemoveObserver(Observer<AIEvents>* pObserver)
		{
			m_pSubject->RemoveObserver(pObserver);
		}

		void CheckIfPotentialTargetIsSeen();

	private:
		TransformComponent* m_pTransformComponent;
		AIBehaviorComponent* m_pAIBehaviorComponent;
		CollisionManager* m_SceneCollisionManager;

		std::unique_ptr<Subject<AIEvents>> m_pSubject;

		std::string m_TargetTag;
		float m_ViewDistance;


		float m_TimeToCheck;
		float m_CheckTimer;
	};
}
