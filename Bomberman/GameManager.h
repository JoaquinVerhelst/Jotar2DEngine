#pragma once



#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "WorldGrid.h"
#include "JsonLevelLoader.h"
#include "GameState.h"
#include "GameState.h"
#include "Observer.h"
#include "Event.h"
#include "AIEvents.h"

namespace Jotar
{
	class GameObject;
	class TransformComponent;


	enum class GameMode
	{
		SinglePlayer,
		Coop,
		Versus
	};

	class GameManager final : public Singleton<GameManager>
	{
	public:
		~GameManager() = default;

		void Init(std::string gameLevelsFilePath);

		void AddPlayer(TransformComponent* ptr);
		std::vector<TransformComponent*> GetPlayers() const;


		void ResetAndInitializeWorldGrid(int rows, int columns, int size);

		WorldGrid* GetWorldGrid();
		JsonLevelLoader& GetLevelLoader();

		int GetCurrentLevelID() const;
		int GetCurrentTotalLevelsPlayed() const;


		void StartAndSetGameMode(GameMode gameMode);
		void LoadLevel(bool isNextLevel = true);
		void LoadHighScore();
		void LoadMainMenu();

		GameMode GetGamemode();

	private:
		friend class Singleton<GameManager>;
		GameManager() = default;


		void ChangeState(GameState* newState);

		std::vector<TransformComponent*> m_Players;
		std::unique_ptr<WorldGrid> m_pWorldGrid;
		GameMode m_GameMode{ GameMode::SinglePlayer };
		JsonLevelLoader m_LevelLoader{};

		int m_AmountOfLevelIDs;
		int m_CurrentLevelID;
		int m_CurrentTotalLevelsPlayed;

		//STATE

		GameState* m_CurrentState{};

		std::unique_ptr<MainMenuState> m_pMainMenuState;
		std::unique_ptr<GameLevelState> m_pGameLevelState;
		std::unique_ptr<HighscoreState> m_pHighScoreState;

	};
}



