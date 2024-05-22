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


	class TransitionState : public GameState
	{
	public:
		void OnEnter(GameManager* gameManager) override;
		GameState* OnHandle() override;
		void OnExit(GameManager* gameManager) override;
	};



	class GameLevelState : public GameState
	{
	public:
		GameLevelState();

		void OnEnter(GameManager* gameManager) override;
		GameState* OnHandle() override;
		void OnExit(GameManager* gameManager) override;
	private:
		bool m_IsGameModeInitialized;
	};

	class HighscoreState : public GameState
	{
	public:
		void OnEnter(GameManager* gameManager) override;
		GameState* OnHandle() override;
		void OnExit(GameManager* gameManager) override;

		void SetIsSaving(bool isSaving);

	private:
		bool m_IsSaving{false};
	};
}




