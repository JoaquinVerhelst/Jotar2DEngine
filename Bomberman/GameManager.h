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

		void SetPlayerOne(std::shared_ptr<GameObject> ptr);
		void SetPlayerTwo(std::shared_ptr<GameObject> ptr);

		std::shared_ptr<GameObject> GetPlayerOne();
		std::shared_ptr<GameObject> GetPlayerTwo();


		void ResetAndInitializeWorldGrid(int rows, int columns, int size);

		WorldGrid* GetWorldGrid();
		JsonLevelLoader& GetLevelLoader();


		void StartAndSetGameMode(GameMode gameMode);
		void NextLevel();

	private:
		friend class Singleton<GameManager>;
		GameManager() = default;


		void ChangeState(GameState* newState);

		////Change to Vector??
		std::shared_ptr<GameObject> m_PlayerOne{ nullptr };
		std::shared_ptr<GameObject> m_PlayerTwo{ nullptr };

		std::unique_ptr<WorldGrid> m_pWorldGrid;

		GameMode m_GameMode{ GameMode::SinglePlayer };

		JsonLevelLoader m_LevelLoader{};


		//STATE

		GameState* m_CurrentState;

		std::unique_ptr<MainMenuState> m_pMainMenuState;
		std::unique_ptr<GameLevelState> m_pGameLevelState;
		std::unique_ptr<HighscoreState> m_pHighScoreState;

	};
}



