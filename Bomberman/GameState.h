#pragma once
#include <string>
#include <glm/glm.hpp>



struct SDL_Color;

namespace Jotar
{
	class GameManager;

	class GameState
	{
	public:
		GameState(const glm::ivec4& color);

		virtual ~GameState() = default;

		GameState(GameState&& other) = delete;
		GameState(const GameState& other) = delete;
		const GameState& operator=(GameState&& other) = delete;
		const GameState& operator=(const GameState& other) = delete;

		virtual void OnEnter(GameManager* ) {}
		virtual void OnExit(GameManager* ) {}

	protected:
		glm::ivec4 m_BackgroundColor;
	};


	class MainMenuState : public GameState 
	{
	public:
		MainMenuState(const glm::ivec4& color)
			:GameState(color) {}

		void OnEnter(GameManager* gameManager) override;
		void OnExit(GameManager* gameManager) override;
	};


	class GameLevelState : public GameState
	{
	public:

		GameLevelState(const glm::ivec4& color)
			:GameState(color) {}

		void OnEnter(GameManager* gameManager) override;
	};

	class HighscoreState : public GameState
	{
	public:

		HighscoreState(const glm::ivec4& color)
			:GameState(color) {}

		void OnEnter(GameManager* gameManager) override;
		void OnExit(GameManager* gameManager) override;

		void SetIsSaving(bool isSaving);

	private:
		bool m_IsSaving{false};
		std::string m_PreviousSceneName{};
	};
}




