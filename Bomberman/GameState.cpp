#include "GameState.h"
#include "GameManager.h"
#include "JsonLevelLoader.h"
#include "SceneManager.h"
#include "Scene.h"

// ------------------------------------------------------------------------------------- //
//									MAIN MENU STATE										 //
// ------------------------------------------------------------------------------------- //

void Jotar::MainMenuState::OnEnter(GameManager* gameManager)
{
	auto& scene = SceneManager::GetInstance().SetCurrentSceneByName("mainMenu");
	scene.MarkAllForDestroy();


	gameManager->GetLevelLoader().LoadMenuFromJson(scene);

	scene.Start();
}

Jotar::GameState* Jotar::MainMenuState::OnHandle()
{

	return nullptr;
}

void Jotar::MainMenuState::OnExit(GameManager* )
{
	auto& scene = SceneManager::GetInstance().GetCurrentScene();
	scene.MarkAllForDestroy();
}



// ------------------------------------------------------------------------------------- //
//									Game Level STATE									 //
// ------------------------------------------------------------------------------------- //


Jotar::GameLevelState::GameLevelState(int amountofLevels)
	: m_CurrentLevel{ 0 }
	, m_AmountOfLevels{ amountofLevels }
	, m_IsGameModeInitialized{ false }
{
}

void Jotar::GameLevelState::OnEnter(GameManager* gameManager)
{
	++m_CurrentLevel;
	if (m_CurrentLevel > m_AmountOfLevels)
	{
		m_CurrentLevel = 1;
	}

	std::string levelName = "level" + std::to_string(m_CurrentLevel);

	auto& prevScene = SceneManager::GetInstance().GetCurrentScene();

	auto& nextScene = SceneManager::GetInstance().GetSceneByName(levelName);
	nextScene.RemoveAll();

	prevScene.HandleDontDestroyOnLoadObjects(nextScene);

	prevScene.Reset();



	SceneManager::GetInstance().SetCurrentSceneByName(levelName);


	gameManager->GetLevelLoader().LoadLevelFromJson(nextScene, m_CurrentLevel, m_IsGameModeInitialized);
	m_IsGameModeInitialized = true;

	nextScene.Start();
}

Jotar::GameState* Jotar::GameLevelState::OnHandle()
{
	return nullptr;
}

void Jotar::GameLevelState::OnExit(GameManager* )
{
	//auto& scene = SceneManager::GetInstance().GetCurrentScene();

}


// ------------------------------------------------------------------------------------- //
//								 HighScore State STATE									 //
// ------------------------------------------------------------------------------------- //



void Jotar::HighscoreState::OnEnter(GameManager* )
{
}

Jotar::GameState* Jotar::HighscoreState::OnHandle()
{
	return nullptr;
}

void Jotar::HighscoreState::OnExit(GameManager* )
{
}
