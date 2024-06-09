#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "WorldTimeManager.h"

#include "ImguiRenderer.h"
#include "SoundServiceLocator.h"
#include "SoundSystem.h"


Jotar::Minigin::Minigin(const std::string &dataPath)
{
	Renderer::GetInstance().Init();

	SoundServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDL_SoundSystem>()));

	//CSteamAchievements::GetInstance().Init(g_Achievements, 4);

	ResourceManager::GetInstance().Init(dataPath);
}

Jotar::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
}

void Jotar::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = WorldTimeManager::GetInstance();
	//auto& imgui = ImguiRenderer::GetInstance();
	//auto& worldGrid = WorldGrid::GetInstance();

	//Game loop
	bool doContinue = true;
	
	time.Init();

	float lag = 0.0f;

	float fixedTimeStep = 0.02f;



	//sceneManager.Start();

	while (doContinue)
	{

		time.Update();

		doContinue = input.ProcessInput();

		lag += time.GetDeltaTime();



		while (lag >= fixedTimeStep)
		{
			sceneManager.FixedUpdate();
			lag -= fixedTimeStep;
		}


		sceneManager.Update();

		sceneManager.LateUpdate();
		SoundServiceLocator::GetSoundSystem().Update();

		renderer.Update();
		renderer.Render();


		sceneManager.CleanUpDestroyedObjects();

		input.ClearFrameEvents();
	}

	sceneManager.Destroy();
	//imgui.Destroy();
}
