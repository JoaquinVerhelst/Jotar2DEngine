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

#include "ShaderComponent.h"
#include "Mesh2DComponent.h"
//#include "GLBuffers.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "MovementCommand.h"

using namespace Jotar;



void load()
{
	auto& scene = Jotar::SceneManager::GetInstance().CreateScene("Demo");

	//background 
	auto go = scene.CreateGameObject("Background");
	go->AddComponent<TextureComponent>("../Data/Sprites/background.tga");



	// logo
	go = scene.CreateGameObject("Logo");
	go->AddComponent<TextureComponent>("../Data/Sprites/logo.tga");
	go->GetTransform()->SetPosition(216, 180);




	auto font1 = Jotar::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = Jotar::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);


	//// Text

	go = scene.CreateGameObject("Prog4Text");
	go->AddComponent<TextComponent>("Programming 4 Assignment", font1, SDL_Color(255, 255, 0, 255),  false);
	go->GetTransform()->SetPosition(80, 20);
	go->GetTransform()->SetPosition(80, 20);



	go = scene.CreateGameObject("FPS Counter");
	go->AddComponent<Jotar::FPSComponent>(font2, SDL_Color(0, 255, 0, 255));
	go->GetTransform()->SetPosition(10, 10);




	auto player1 = scene.CreateGameObject("Bomberman1");
	player1->AddComponent<TextureComponent>("../Data/Sprites/bomberman.png");
	auto movementCompPlayer1 = player1->AddComponent<MovementComponent>(30.f);
	player1->GetTransform()->SetPosition(200, 250);


	auto player2 = scene.CreateGameObject("Bomberman2");
	player2->AddComponent<TextureComponent>("../Data/Sprites/bomberman2.png");
	auto movementCompPlayer2 = player2->AddComponent<MovementComponent>(60.f);
	player2->GetTransform()->SetPosition(280, 250);


	auto& input = InputManager::GetInstance();

	//input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadUp }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{0, -1}));
	//input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadDown }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ 0, 1 }));
	//input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadRight }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ 1, 0 }));
	//input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadLeft }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ -1, 0 }));


	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadUp }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ 0, -1 }));
	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadDown }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ 0, 1 }));
	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadRight }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ 1, 0 }));
	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadLeft }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ -1, 0 }));


	input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_w }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ 0, -1 }));
	input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_s }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ 0, 1 }));
	input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_d }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ 1, 0 }));
	input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_a }, std::make_unique<MovementCommand>(movementCompPlayer1.get(), glm::vec2{ -1, 0 }));


	//input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_UP }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ 0, -1 }));
	//input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_DOWN }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ 0, 1 }));
	//input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_RIGHT }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ 1, 0 }));
	//input.AddKeyBinding(KeyboardButton{ Jotar::KeyboardButton::Key_LEFT }, std::make_unique<MovementCommand>(movementCompPlayer2.get(), glm::vec2{ -1, 0 }));


	 //OPENGL RENDERING NOT READY YET - TESTING

	//float vertices[]
	//{
	//	-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.f,
	//	0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.f,
	//	0.0f, 0.5f, 0.0f, .8f, 0.8f, 0.2f, 1.f
	//};

	//unsigned int elements[]
	//{
	//	0, 1, 2
	//};

	//const char* vertexShader = R"(
	//	#version 410 core
	//	
	//	layout (location = 0) in vec3 position;
	//	layout (location = 1) in vec4 color;

	//	out vec3 vpos;
	//	out vec4 vcolor;


	//	void main()
	//	{
	//		vpos = position;
	//		vcolor = color;
	//		gl_Position = vec4(position, 1.0);

	//	}
	//)";


	//const char* fragmentShader = R"(
	//	#version 410 core
	//	
	//	layout (location = 0) out vec4 outColor;

	//	in vec3 vpos;
	//	in vec4 vcolor;


	//	uniform vec3 color = vec3(0.0);

	//	void main()
	//	{
	//		outColor = vec4(vpos * 0.5 + 0.5, 1.0);
	//		outColor = vcolor;
	//	}
	//)";


	//auto triangle = scene.CreateGameObject("triangle");
	//auto shader = triangle->AddComponent<ShaderComponent>(vertexShader, fragmentShader);
	////shader->SetUniformFloat3("color", 0.f, 0.f, 1.f);


	//auto mesh = triangle->AddComponent<Mesh2DComponent>(&vertices[0], 3, 7, &elements[0], 3);
	////mesh->SetLayout(layout);

	//triangle->GetTransform()->SetPosition(250, 250);

}

int main(int, char*[]) {
	Jotar::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}