#pragma once

#include "Component.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Font.h"
#include "JsonHighScoreLoaderComponent.h"

namespace Jotar
{
	class GameObject;

	class HighScoreMenuComponent : public Component
	{
	public:
		HighScoreMenuComponent(GameObject* owner, const std::shared_ptr<Font>& font, glm::ivec2 windowSize);
		~HighScoreMenuComponent() = default;

		void Update() override;
		void Start() override;

		void UpdateHighScoreList();

		HighScoreMenuComponent(const HighScoreMenuComponent& other) = delete;
		HighScoreMenuComponent(HighScoreMenuComponent&& other) = delete;
		HighScoreMenuComponent& operator=(const HighScoreMenuComponent& other) = delete;
		HighScoreMenuComponent& operator=(HighScoreMenuComponent&& other) = delete;

	private:


		JsonHighScoreLoaderComponent* m_pJsonHighScoreLoader;

		std::vector<GameObject*> m_ScoreTextObjects;
		std::shared_ptr<Font> m_Font;

		glm::ivec2 m_WindowSize;
	};
}