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
#include "RotatorComponent.h"

using namespace dae;



void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//background 
	auto go = scene.CreateGameObject("Background");
	go->AddComponent<TextureComponent>("../Data/Sprites/background.tga");



	// logo
	go = scene.CreateGameObject("Logo");
	go->AddComponent<TextureComponent>("../Data/Sprites/logo.tga");
	go->GetTransform()->SetLocalPosition(216, 180);




	auto font1 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);


	//// Text

	go = scene.CreateGameObject("Prog4Text");
	go->AddComponent<TextComponent>("Programming 4 Assignment", font1, SDL_Color(255, 255, 0, 255),  false);
	go->GetTransform()->SetLocalPosition(80, 20);
	go->GetTransform()->SetLocalPosition(80, 20);



	go = scene.CreateGameObject("FPS Counter");
	go->AddComponent<dae::FPSComponent>(font2, SDL_Color(0, 255, 0, 255));
	go->GetTransform()->SetLocalPosition(10, 10);




	auto player1 = scene.CreateGameObject("Bomberman1");
	player1->AddComponent<TextureComponent>("../Data/Sprites/bomberman.png");
	player1->AddComponent<RotatorComponent>(glm::vec2{ 250,250 }, 20.f, 5.f);


	auto player2 = player1->CreateChildGameObject("Bomberman2");
	player2->AddComponent<TextureComponent>("../Data/Sprites/bomberman2.png");
	player2->AddComponent<RotatorComponent>(glm::vec2{ 0,0 }, 10.f, 10.f);

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}