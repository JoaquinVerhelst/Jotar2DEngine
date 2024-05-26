#include "GameState.h"
#include "GameManager.h"
#include "JsonLevelLoader.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

// ------------------------------------------------------------------------------------- //
//									MAIN MENU STATE										 //
// ------------------------------------------------------------------------------------- //

void Jotar::MainMenuState::OnEnter(GameManager* gameManager)
{
	SceneManager::GetInstance().GetCurrentScene().MarkAllForDestroy();
	gameManager->Reset();

	InputManager::GetInstance().ClearInputBindings();

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
	InputManager::GetInstance().ClearInputBindings();
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


void Jotar::GameLevelState::OnEnter(GameManager* gameManager)
{
	std::string levelName = "level" + std::to_string(gameManager->GetCurrentTotalLevelsPlayed());

	auto& prevScene = SceneManager::GetInstance().GetCurrentScene();

	auto& nextScene = SceneManager::GetInstance().CreateScene(levelName);

	prevScene.HandleDontDestroyOnLoadObjects(nextScene);
	prevScene.Reset();


	gameManager->GetLevelLoader().LoadLevelFromJson(nextScene, gameManager->GetCurrentLevelID(), gameManager->GetIsGameInit());
	gameManager->SetGameInit(true);


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

	SceneManager::GetInstance().GetCurrentScene().MarkSceneForDestroy();

}


// ------------------------------------------------------------------------------------- //
//								 HIGHSCORE  STATE										 //
// ------------------------------------------------------------------------------------- //



void Jotar::HighscoreState::OnEnter(GameManager* gameManager)
{


	auto& prevScene = SceneManager::GetInstance().GetCurrentScene();
	auto& nextScene = SceneManager::GetInstance().GetSceneByName("highScoreMenu");

	gameManager->GetLevelLoader().LoadHighScoreFromJson(nextScene, m_IsSaving);
	m_IsSaving = false;

	prevScene.MarkAllForDestroy();

	if (prevScene.GetName() != "mainMenu")
	{
		InputManager::GetInstance().ClearInputBindings();
		SceneManager::GetInstance().DestroyScene(prevScene);
	}


	SceneManager::GetInstance().SetCurrentSceneByScene(nextScene);

	nextScene.Start();
}

Jotar::GameState* Jotar::HighscoreState::OnHandle()
{
	return nullptr;
}

void Jotar::HighscoreState::OnExit(GameManager* )
{

	auto& scene = SceneManager::GetInstance().GetCurrentScene();
	scene.MarkAllForDestroy();
}

void Jotar::HighscoreState::SetIsSaving(bool isSaving)
{
	m_IsSaving = isSaving;
}



