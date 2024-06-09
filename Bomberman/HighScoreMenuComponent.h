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
		HighScoreMenuComponent(GameObject* owner, const std::shared_ptr<Font>& font, glm::ivec2 windowSize, float baseOffset = -100.f, float betweenOffset = 50.f);
		~HighScoreMenuComponent() = default;

		void Start() override;

		void UpdateHighScoreList();
		JsonHighScoreLoaderComponent* GetHighScoreLoader() const;

		HighScoreMenuComponent(const HighScoreMenuComponent& other) = delete;
		HighScoreMenuComponent(HighScoreMenuComponent&& other) = delete;
		HighScoreMenuComponent& operator=(const HighScoreMenuComponent& other) = delete;
		HighScoreMenuComponent& operator=(HighScoreMenuComponent&& other) = delete;

	private:

		void CreateScoreTextGameObject(std::string gameMode, std::string scoreText, glm::vec2 position);

		JsonHighScoreLoaderComponent* m_pJsonHighScoreLoader;

		std::vector<GameObject*> m_ScoreTextObjects;
		std::shared_ptr<Font> m_Font;

		float m_BaseOffset;
		float m_BetweenOffset;

		glm::ivec2 m_WindowSize;
	};
}