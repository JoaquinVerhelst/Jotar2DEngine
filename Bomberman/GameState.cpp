#include "GameState.h"
#include "GameManager.h"
#include "JsonLevelLoader.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

// ------------------------------------------------------------------------------------- //
//									MAIN MENU STATE										 //
// ------------------------------------------------------------------------------------- //

void Jotar::MainMenuState::OnEnter(GameManager* gameManager)
{
	Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0, 255));
	InputManager::GetInstance().ClearPlayerInputBindings();


	SceneManager::GetInstance().GetCurrentScene().MarkAllForDestroy();
	gameManager->Reset();

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
	InputManager::GetInstance().ClearPlayerInputBindings();
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
	Renderer::GetInstance().SetBackgroundColor(SDL_Color(20, 180, 20, 255));

	std::string levelName = "level" + std::to_string(gameManager->GetCurrentTotalLevelsPlayed());

	auto& prevScene = SceneManager::GetInstance().GetCurrentScene();

	auto& nextScene = SceneManager::GetInstance().CreateScene(levelName);

	prevScene.HandleDontDestroyOnLoadObjects(nextScene);
	prevScene.Reset();


	gameManager->GetLevelLoader().LoadLevelFromJson(nextScene, gameManager->GetCurrentLevelID(), gameManager->GetIsGameInit());
	gameManager->SetGameInit(true);

	SceneManager::GetInstance().SetCurrentSceneByScene(nextScene);

	nextScene.Start();


	if (prevScene.GetName() != "mainMenu")
	{
		SceneManager::GetInstance().DestroyScene(prevScene);
	}
}

Jotar::GameState* Jotar::GameLevelState::OnHandle()
{
	return nullptr;
}

void Jotar::GameLevelState::OnExit(GameManager* )
{

	//SceneManager::GetInstance().GetCurrentScene().MarkSceneForDestroy();

}


// ------------------------------------------------------------------------------------- //
//								 HIGHSCORE  STATE										 //
// ------------------------------------------------------------------------------------- //



void Jotar::HighscoreState::OnEnter(GameManager* gameManager)
{
	Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0, 255));

	m_PreviousSceneName = SceneManager::GetInstance().GetCurrentSceneName();
	auto& prevScene = SceneManager::GetInstance().GetSceneByName(m_PreviousSceneName);
	prevScene.HandleDontDestroyOnLoadObjects(prevScene);

	auto& nextScene = SceneManager::GetInstance().GetSceneByName("highScoreMenu");

	gameManager->GetLevelLoader().LoadHighScoreFromJson(nextScene, m_IsSaving);
	m_IsSaving = false;

	SceneManager::GetInstance().SetCurrentSceneByScene(nextScene);
	nextScene.Start();
}

Jotar::GameState* Jotar::HighscoreState::OnHandle()
{
	return nullptr;
}

void Jotar::HighscoreState::OnExit(GameManager*)
{

	auto& prevScene = SceneManager::GetInstance().GetSceneByName(m_PreviousSceneName);
	prevScene.MarkAllForDestroy();

	if (m_PreviousSceneName != "mainMenu")
	{
		InputManager::GetInstance().ClearPlayerInputBindings();
		SceneManager::GetInstance().DestroyScene(prevScene);

	}

	auto& scene = SceneManager::GetInstance().GetCurrentScene();
	scene.MarkAllForDestroy();
}

void Jotar::HighscoreState::SetIsSaving(bool isSaving)
{
	m_IsSaving = isSaving;
}



