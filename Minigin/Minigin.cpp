#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "WorldTimeManager.h"
#include "GLSDLManager.h"
#include "ImguiRenderer.h"


Jotar::Minigin::Minigin(const std::string &dataPath)
{

	GLSDLManager::GetInstance().Init();

	Renderer::GetInstance().Init();

	ResourceManager::GetInstance().Init(dataPath);
}

Jotar::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	GLSDLManager::GetInstance().Destroy();
}

void Jotar::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = WorldTimeManager::GetInstance();
	//auto& imgui = ImguiRenderer::GetInstance();


	//Game loop
	bool doContinue = true;
	
	time.Init();

	float lag = 0.0f;

	float fixedTimeStep = 0.02f;

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

	
		renderer.Render();

		//imgui.Render();

		//renderer.EndRender();

		sceneManager.CleanUpDestroyedObjects();

		input.ClearFrameEvents();
	}

	sceneManager.Destroy();
	//imgui.Destroy();
}
