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
//								 TRANSITION  STATE										 //
// ------------------------------------------------------------------------------------- //


void Jotar::TransitionState::OnEnter(GameManager* )
{

}

Jotar::GameState* Jotar::TransitionState::OnHandle()
{
	return nullptr;
}

void Jotar::TransitionState::OnExit(GameManager* )
{
}




// ------------------------------------------------------------------------------------- //
//									Game Level STATE									 //
// ------------------------------------------------------------------------------------- //


Jotar::GameLevelState::GameLevelState()
	:  m_IsGameModeInitialized{ false }
{
}

void Jotar::GameLevelState::OnEnter(GameManager* gameManager)
{


	std::string levelName = "level" + std::to_string(gameManager->GetCurrentTotalLevelsPlayed());

	auto& prevScene = SceneManager::GetInstance().GetCurrentScene();

	auto& nextScene = SceneManager::GetInstance().CreateScene(levelName);

	prevScene.HandleDontDestroyOnLoadObjects(nextScene);
	prevScene.Reset();


	gameManager->GetLevelLoader().LoadLevelFromJson(nextScene, gameManager->GetCurrentLevelID(), m_IsGameModeInitialized);
	m_IsGameModeInitialized = true;


	if (prevScene.GetName() != "mainMenu")
	{
		SceneManager::GetInstance().DestroyScene(prevScene);
	}

	SceneManager::GetInstance().SetCurrentSceneByScene(nextScene);

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
//								 HIGHSCORE  STATE										 //
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



