#include "GameManager.h"
#include "GameObject.h"
#include "AIEvents.h"
#include <iostream>

void Jotar::GameManager::Init(std::string gameLevelsFilePath)
{
    m_LevelLoader.SetGameLevelsFilePath(gameLevelsFilePath);

    m_LevelLoader.InitGame();

    m_pMainMenuState = std::make_unique<MainMenuState>();
    m_pGameLevelState = std::make_unique<GameLevelState>(3);
    m_pHighScoreState = std::make_unique<HighscoreState>();


    ChangeState(m_pMainMenuState.get());

}

void Jotar::GameManager::AddPlayer(TransformComponent* ptr)
{
    m_Players.emplace_back(ptr);
}

std::vector<Jotar::TransformComponent*> Jotar::GameManager::GetPlayers() const
{
    return m_Players;
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

void Jotar::GameManager::StartAndSetGameMode(GameMode gameMode)
{
    m_GameMode = gameMode;
    ChangeState(m_pGameLevelState.get());
}

void Jotar::GameManager::NextLevel()
{
    ChangeState(m_pGameLevelState.get());
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
