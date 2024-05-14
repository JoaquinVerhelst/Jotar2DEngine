#pragma once
#include <string>


namespace Jotar
{
	class GameManager;

	class GameState
	{
	public:
		GameState() = default;

		virtual ~GameState() = default;

		GameState(GameState&& other) = delete;
		GameState(const GameState& other) = delete;
		const GameState& operator=(GameState&& other) = delete;
		const GameState& operator=(const GameState& other) = delete;

		virtual void OnEnter(GameManager* ) {}
		virtual GameState* OnHandle() { return this; }
		virtual void OnExit(GameManager* ) {}
	};


	class MainMenuState : public GameState 
	{
	public:
		void OnEnter(GameManager* gameManager) override;
		GameState* OnHandle() override;
		void OnExit(GameManager* gameManager) override;
	};


	class GameLevelState : public GameState
	{
	public:
		GameLevelState(int AmountOfLevels);

		void OnEnter(GameManager* gameManager) override;
		GameState* OnHandle() override;
		void OnExit(GameManager* gameManager) override;
	private:
		int m_CurrentLevel;
		int m_AmountOfLevels;
	};

	class HighscoreState : public GameState
	{
	public:
		void OnEnter(GameManager* gameManager) override;
		GameState* OnHandle() override;
		void OnExit(GameManager* gameManager) override;
	};
}




