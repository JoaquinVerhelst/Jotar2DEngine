#include "GameState.h"
#include "GameManager.h"
#include "JsonLevelLoader.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

Jotar::GameState::GameState(const glm::ivec4& color)
	:m_BackgroundColor(color)
{
}


// ------------------------------------------------------------------------------------- //
//									MAIN MENU STATE										 //
// ------------------------------------------------------------------------------------- //

void Jotar::MainMenuState::OnEnter(GameManager* gameManager)
{
	Renderer::GetInstance().SetBackgroundColorGLM(m_BackgroundColor);
	InputManager::GetInstance().ClearPlayerInputBindings();


	SceneManager::GetInstance().GetCurrentScene().MarkAllForDestroy();
	gameManager->Reset();

	auto& scene = SceneManager::GetInstance().SetCurrentSceneByName("mainMenu");
	scene.MarkAllForDestroy();


	gameManager->GetLevelLoader().LoadMenuFromJson(scene);

	scene.Start();
}


void Jotar::MainMenuState::OnExit(GameManager* )
{
	auto& scene = SceneManager::GetInstance().GetCurrentScene();
	InputManager::GetInstance().ClearPlayerInputBindings();
	scene.MarkAllForDestroy();
}

// ------------------------------------------------------------------------------------- //
//									Game Level STATE									 //
// ------------------------------------------------------------------------------------- //


void Jotar::GameLevelState::OnEnter(GameManager* gameManager)
{
	Renderer::GetInstance().SetBackgroundColorGLM(m_BackgroundColor);

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


// ------------------------------------------------------------------------------------- //
//								 HIGHSCORE  STATE										 //
// ------------------------------------------------------------------------------------- //



void Jotar::HighscoreState::OnEnter(GameManager* gameManager)
{
	Renderer::GetInstance().SetBackgroundColorGLM(m_BackgroundColor);

	m_PreviousSceneName = SceneManager::GetInstance().GetCurrentSceneName();
	auto& prevScene = SceneManager::GetInstance().GetSceneByName(m_PreviousSceneName);
	prevScene.HandleDontDestroyOnLoadObjects(prevScene);

	auto& nextScene = SceneManager::GetInstance().GetSceneByName("highScoreMenu");

	gameManager->GetLevelLoader().LoadHighScoreFromJson(nextScene, m_IsSaving);
	m_IsSaving = false;

	SceneManager::GetInstance().SetCurrentSceneByScene(nextScene);
	nextScene.Start();
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


