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
	auto& scene = SceneManager::GetInstance().SetGetCurrentSceneByName("MainMenu");
	scene.RemoveAll();


	gameManager->GetLevelLoader().LoadMenuFromJson(scene);

	scene.Start();
}

Jotar::GameState* Jotar::MainMenuState::OnHandle()
{

	return nullptr;
}

void Jotar::MainMenuState::OnExit(GameManager* )
{
	auto& scene = SceneManager::GetInstance().GetSceneByName("MainMenu");
	scene.RemoveAll();
}



// ------------------------------------------------------------------------------------- //
//									Game Level STATE									 //
// ------------------------------------------------------------------------------------- //


Jotar::GameLevelState::GameLevelState(int amountofLevels )
	: m_CurrentLevel{ 0 }
	, m_AmountOfLevels{ amountofLevels }
{
}

void Jotar::GameLevelState::OnEnter(GameManager* gameManager )
{
	++m_CurrentLevel;
	if (m_CurrentLevel > m_AmountOfLevels)
	{
		m_CurrentLevel = 1;
	}

	auto& scene = SceneManager::GetInstance().SetGetCurrentSceneByName("Level" + std::to_string(m_CurrentLevel));
	scene.RemoveAll();

	// Set up Input

	gameManager->GetLevelLoader().LoadLevelFromJson(scene, m_CurrentLevel);


	scene.Start();
}

Jotar::GameState* Jotar::GameLevelState::OnHandle()
{
	return nullptr;
}

void Jotar::GameLevelState::OnExit(GameManager* )
{
	auto& scene = SceneManager::GetInstance().GetSceneByName("Level" + std::to_string(m_CurrentLevel));
	scene.RemoveAll();
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
