#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Subject.h"
#include "EventAddScore.h"

namespace Jotar
{
	class GameObject;

	class ScoreComponent final : public Component, public Subject<EventAddScore>
	{
	public:

		ScoreComponent(GameObject* owner);
		~ScoreComponent();

		virtual void Start() override;

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		int GetScore();
		void AddScore(int score);

	private:

		int m_CurrentScore;
	};
}
