#include "GameManager.h"
#include "GameObject.h"
#include "AIEvents.h"
#include <iostream>

void Jotar::GameManager::Init(std::string gameLevelsFilePath)
{
    m_LevelLoader.SetGameLevelsFilePath(gameLevelsFilePath);

    m_pMainMenuState = std::make_unique<MainMenuState>();
    m_pGameLevelState = std::make_unique<GameLevelState>(3);
    m_pHighScoreState = std::make_unique<HighscoreState>();


    ChangeState(m_pMainMenuState.get());

}

void Jotar::GameManager::SetPlayerOne(std::shared_ptr<GameObject> ptr)
{
    m_PlayerOne = ptr;
}

void Jotar::GameManager::SetPlayerTwo(std::shared_ptr<GameObject> ptr)
{
    m_PlayerTwo = ptr;
}

std::shared_ptr<Jotar::GameObject> Jotar::GameManager::GetPlayerOne()
{
    return m_PlayerOne;
}

std::shared_ptr<Jotar::GameObject> Jotar::GameManager::GetPlayerTwo()
{
    return m_PlayerTwo;
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

void Jotar::GameManager::StartAndSetGameMode(GameMode)
{
    ChangeState(m_pGameLevelState.get());
}

void Jotar::GameManager::NextLevel()
{
    ChangeState(m_pGameLevelState.get());
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
