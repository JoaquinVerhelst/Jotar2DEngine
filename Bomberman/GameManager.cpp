#include "GameManager.h"
#include "AIEvents.h"
#include <iostream>

void Jotar::GameManager::Init(std::string gameLevelsFilePath)
{
    m_LevelLoader.SetGameLevelsFilePath(gameLevelsFilePath);

    m_LevelLoader.InitGame();


    m_AmountOfLevelIDs = 3;
    m_CurrentLevelID = 1;
    m_CurrentTotalLevelsPlayed = 0;


    m_pMainMenuState = std::make_unique<MainMenuState>();
    m_pGameLevelState = std::make_unique<GameLevelState>();
    m_pHighScoreState = std::make_unique<HighscoreState>();


    ChangeState(m_pMainMenuState.get());

}

void Jotar::GameManager::Reset()
{
    m_IsGameInit = false;
    m_Players.clear();

    m_CurrentLevelID = 1;
    m_CurrentTotalLevelsPlayed = 0;
}

void Jotar::GameManager::AddPlayer(TransformComponent* ptr)
{
    m_Players.emplace_back(ptr);
}

std::vector<Jotar::TransformComponent*> Jotar::GameManager::GetPlayers() const
{
    return m_Players;
}

void Jotar::GameManager::SetBalloomPlayer(GameObject* balloomPlayer)
{
    m_pBalloomPlayer = balloomPlayer;
}

Jotar::GameObject* Jotar::GameManager::GetBalloomPlayer()
{
    return m_pBalloomPlayer;
}


void Jotar::GameManager::ResetAndInitializeWorldGrid(int rows, int columns, int size)
{
    m_pWorldGrid = std::make_unique<WorldGrid>(rows, columns, size);
}

Jotar::WorldGrid* Jotar::GameManager::GetWorldGrid()
{
    return m_pWorldGrid.get();
}

Jotar::JsonLevelLoader& Jotar::GameManager::GetLevelLoader()
{
    return m_LevelLoader;
}

int Jotar::GameManager::GetCurrentLevelID() const
{
    return m_CurrentLevelID;
}

int Jotar::GameManager::GetCurrentTotalLevelsPlayed() const
{
    return m_CurrentTotalLevelsPlayed;
}

bool Jotar::GameManager::GetIsGameInit() const
{
    return m_IsGameInit;
}

void Jotar::GameManager::SetGameInit(bool isGameInit)
{
    m_IsGameInit = isGameInit;
}

void Jotar::GameManager::StartAndSetGameMode(GameMode gameMode)
{
    m_GameMode = gameMode;
    ChangeState(m_pGameLevelState.get());
}

void Jotar::GameManager::LoadLevel(bool isNextLevel)
{
    if (isNextLevel)
    {
         ++m_CurrentLevelID;
        if (m_CurrentLevelID > m_AmountOfLevelIDs)
           m_CurrentLevelID = 1;
        
        ++m_CurrentTotalLevelsPlayed;
    }
    ChangeState(m_pGameLevelState.get());
}

void Jotar::GameManager::LoadHighScoreMenu(bool isSaving)
{
    m_pHighScoreState->SetIsSaving(isSaving);
    ChangeState(m_pHighScoreState.get());
}

void Jotar::GameManager::LoadMainMenu()
{
    ChangeState(m_pMainMenuState.get());
}

Jotar::GameMode Jotar::GameManager::GetGamemode()
{
    return m_GameMode;
}


void Jotar::GameManager::ChangeState(GameState* newState)
{
    if (m_CurrentState) {
        m_CurrentState->OnExit(this);
    }

    m_CurrentState = newState;

    if (m_CurrentState) {
        m_CurrentState->OnEnter(this);
    }
}
