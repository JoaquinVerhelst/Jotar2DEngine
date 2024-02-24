#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FPSComponent.h"

using namespace dae;



void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//background 
	auto go = std::make_shared<dae::GameObject>(&scene, "Background");
	go->AddComponent<TextureComponent>("../Data/background.tga");
	scene.Add(go);


	// logo
	go = std::make_shared<dae::GameObject>(&scene, "Logo");
	go->AddComponent<TextureComponent>("../Data/logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);



	auto font1 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);


	//// Text

	go = std::make_shared<dae::GameObject>(&scene, "AssignmentText");
	go->AddComponent<TextComponent>("Programming 4 Assignment", font1, SDL_Color(255, 255, 0, 255),  false);
	go->SetPosition(80, 20);
	scene.Add(go);


	go = std::make_shared<dae::GameObject>(&scene, "FPSCounter");
	go->AddComponent<dae::FPSComponent>(font2, SDL_Color(255, 255, 0, 255));
	go->SetPosition(10, 10);
	scene.Add(go);

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}