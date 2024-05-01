//#include <SDL.h>

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
//#include "Mesh2DComponent.h"
//#include "GLBuffers.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "MovementCommand.h"

#include "HealthComponent.h"
#include "HealthDisplayComponent.h"
#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"

#include "PlaceBombCommand.h"
#include "PlaceBombComponent.h"

#include "TempCommand.h"
#include <iostream>
#include "Enums.h"
#include "ColliderComponent.h"
#include "Camera.h"

#include "JsonLevelLoader.h"


using namespace Jotar;



void load()
{


	//background 
	//auto go = scene.CreateGameObject("Background");
	//go->AddComponent<TextureComponent>("../Data/Sprites/background.tga");






	// Shared Textures

	ResourceManager::GetInstance().AddSharedTexture("../Data/Sprites/Bomb.png", "Bomb");
	ResourceManager::GetInstance().AddSharedTexture("../Data/Sprites/BreakableWall.png", "BreakableWall");
	ResourceManager::GetInstance().AddSharedTexture("../Data/Sprites/UnbreakableWall.png", "UnbreakableWall");

	ResourceManager::GetInstance().AddSharedTexture("../Data/Sprites/ExtraBombPickUp.png", "ExtraBombPickUp");
	ResourceManager::GetInstance().AddSharedTexture("../Data/Sprites/ExplosionRangePickUp.png", "ExplosionRangePickUp");
	ResourceManager::GetInstance().AddSharedTexture("../Data/Sprites/DetonatorPickUp.png", "DetonatorPickUp");


	SpriteSheet explosionSheet{};
	explosionSheet.TotalColumns = 4;
	explosionSheet.TotalRows = 7;
	explosionSheet.Direction = SpriteSheet::SpriteSheetDirection::Down;

	ResourceManager::GetInstance().AddSharedSpriteSheet("../Data/Sprites/Explosions.png", explosionSheet, "Explosion");




	auto& scene = Jotar::SceneManager::GetInstance().CreateScene("Demo");


	auto levelLoader = scene.CreateGameObject("Background");
	levelLoader->AddComponent<JsonLevelLoader>(scene, "../Data/Json/Level1.json");


	glm::ivec4 camRect = { 0, 0, 720, 1080 };
	glm::ivec4 levelBounds = { 0, 0, 1000, 25*64 };

	auto cameraObj = scene.CreateGameObject("Camera");
	auto camera = cameraObj->AddComponent<Camera>(camRect, levelBounds);

	scene.SetCamera(camera);

	// logo
	auto go = scene.CreateGameObject("Logo");
	go->AddComponent<TextureComponent>("../Data/Sprites/logo.tga");
	go->GetTransform()->SetPosition(216, 180);




	auto font1 = Jotar::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = Jotar::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);


	//// HUD
	auto HUD = scene.CreateGameObject("HUD");


	auto prog4Text = HUD->CreateChildGameObject("Prog4Text", false);
	prog4Text->AddComponent<TextComponent>("Programming 4 Assignment", font1, SDL_Color(255, 255, 0, 255), false);
	prog4Text->GetTransform()->SetPosition(80, 20);


	auto fpsCounter = HUD->CreateChildGameObject("FPScounter", false);
	fpsCounter->AddComponent<TextComponent>("", font2, SDL_Color(0, 255, 0, 255));
	fpsCounter->AddComponent<Jotar::FPSComponent>();
	fpsCounter->GetTransform()->SetPosition(10, 10);


	// PLAYER 1 HUD

	auto player1Text = HUD->CreateChildGameObject("Player1Text", true);
	player1Text->AddComponent<TextComponent>("Player 1: ", font2);
	player1Text->GetTransform()->SetPosition(10, 40);


	auto player1HealthDisplayObj = player1Text->CreateChildGameObject("{Player1HealthDisplay", false);
	player1HealthDisplayObj->AddComponent<TextComponent>("Lives: ", font2);
	auto player1HealthDisplay = player1HealthDisplayObj->AddComponent<HealthDisplayComponent>();
	player1HealthDisplayObj->GetTransform()->SetPosition(10, 55);

	auto player1ScoreDisplayObj = player1Text->CreateChildGameObject("{Player1ScoreDisplay", false);
	player1ScoreDisplayObj->AddComponent<TextComponent>("Score: ", font2);
	auto player1ScoreDisplay = player1ScoreDisplayObj->AddComponent<ScoreDisplayComponent>();
	player1ScoreDisplayObj->GetTransform()->SetPosition(10, 70);





	// PLAYER 2 HUD

	auto player2Text = HUD->CreateChildGameObject("Player2Text", true);
	player2Text->AddComponent<TextComponent>("Player 2: ", font2);
	player2Text->GetTransform()->SetPosition(10, 90);

	auto player2HealthDisplayObj = player2Text->CreateChildGameObject("Player2HealthDisplay", false);
	player2HealthDisplayObj->AddComponent<TextComponent>("Lives: ", font2);
	auto player2HealthDisplay = player2HealthDisplayObj->AddComponent<HealthDisplayComponent>();
	player2HealthDisplayObj->GetTransform()->SetPosition(10, 105);

	auto player2ScoreDisplayObj = player2Text->CreateChildGameObject("{Player2ScoreDisplay", false);
	player2ScoreDisplayObj->AddComponent<TextComponent>("Score: ", font2);
	auto player2ScoreDisplay = player2ScoreDisplayObj->AddComponent<ScoreDisplayComponent>();
	player2ScoreDisplayObj->GetTransform()->SetPosition(10, 120);



	//WALL


	//auto wall = scene.CreateGameObject("Wall");
	//wall->AddComponent<ColliderComponent>(true);
	//wall->AddComponent<TextureComponent>("../Data/Sprites/UnbreakableWall.png");
	//wall->GetTransform()->SetPosition(200, 200);



	// bomb and explosion


	auto explosion = scene.CreateGameObject("Explosion");
	explosion->AddComponent<TextureComponent>("../Data/Sprites/Explosions.png", false, 4, 7);
	explosion->GetTransform()->SetPosition(200, 50);

	//Playerss

	//player 1

	auto player1 = scene.CreateGameObject("Bomberman1");
	player1->AddComponent<TextureComponent>("../Data/Sprites/bomberman.png");
	auto movementCompPlayer1 = player1->AddComponent<MovementComponent>(120.f);

	auto healthCompPlayer1 = player1->AddComponent<HealthComponent>();
	healthCompPlayer1->AddObserver(player1HealthDisplay);

	auto scoreCompPlayer1 = player1->AddComponent<ScoreComponent>();
	scoreCompPlayer1->AddObserver(player1ScoreDisplay);


	auto placeBombComp = player1->AddComponent<PlaceBombComponent>();
	auto colliderComp = player1->AddComponent<ColliderComponent>(false);
	colliderComp->SetTag("Player");
	player1->GetTransform()->SetPosition(200, 250);





	camera->SetTarget(player1->GetTransform());



	//player 2


	auto player2 = scene.CreateGameObject("Bomberman2");
	player2->AddComponent<TextureComponent>("../Data/Sprites/bomberman2.png");
	auto movementCompPlayer2 = player2->AddComponent<MovementComponent>(120.f);

	auto healthCompPlayer2 = player2->AddComponent<HealthComponent>();
	healthCompPlayer2->AddObserver(player2HealthDisplay);

	auto scoreCompPlayer2 = player2->AddComponent<ScoreComponent>();
	scoreCompPlayer2->AddObserver(player2ScoreDisplay);

	auto placeBombComp2 = player2->AddComponent<PlaceBombComponent>();
	colliderComp = player2->AddComponent<ColliderComponent>(false);
	colliderComp->SetTag("Player");
	player2->GetTransform()->SetPosition(280, 250);


	auto& input = InputManager::GetInstance();

	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadUp }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ 0, -1 }));
	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadDown }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ 0, 1 }));
	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadRight }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ 1, 0 }));
	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::DPadLeft }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ -1, 0 }));
	//input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::ButtonA , InputType::Up }, std::make_unique<TakeDamageCommand>(healthCompPlayer1));
	//input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::ButtonB , InputType::Up }, std::make_unique<AddScoreCommand>(scoreCompPlayer1));

	input.AddControllerBinding(ControllerKey{ 0, Jotar::ControllerButton::ButtonB , InputType::Up }, std::make_unique<PlaceBombCommand>(placeBombComp));




	input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::DPadUp }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ 0, -1 }));
	input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::DPadDown }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ 0, 1 }));
	input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::DPadRight }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ 1, 0 }));
	input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::DPadLeft }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ -1, 0 }));
	//input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::ButtonA, InputType::Up }, std::make_unique<TakeDamageCommand>(healthCompPlayer2));
	//input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::ButtonB , InputType::Up }, std::make_unique<AddScoreCommand>(scoreCompPlayer2));
	input.AddControllerBinding(ControllerKey{ 1, Jotar::ControllerButton::ButtonB , InputType::Up }, std::make_unique<PlaceBombCommand>(placeBombComp2));


	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_w }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ 0, -1 }));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_s }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ 0, 1 }));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_d }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ 1, 0 }));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_a }, std::make_unique<MovementCommand>(movementCompPlayer1, glm::vec2{ -1, 0 }));
	//input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_t, InputType::Up }, std::make_unique<TakeDamageCommand>(healthCompPlayer1));
	//input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_g, InputType::Up }, std::make_unique<AddScoreCommand>(scoreCompPlayer1));

	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_e , InputType::Up }, std::make_unique<PlaceBombCommand>(placeBombComp));


	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_UP }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ 0, -1 }));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_DOWN }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ 0, 1 }));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_RIGHT }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ 1, 0 }));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_LEFT }, std::make_unique<MovementCommand>(movementCompPlayer2, glm::vec2{ -1, 0 }));
	//input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_y, InputType::Up }, std::make_unique<TakeDamageCommand>(healthCompPlayer2));
	//input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_h, InputType::Up }, std::make_unique<AddScoreCommand>(scoreCompPlayer2));
	input.AddKeyBinding(KeyboardKey{ Jotar::KeyboardButton::Key_m , InputType::Up }, std::make_unique<PlaceBombCommand>(placeBombComp2));


	std::cout << '\n';
	std::cout << '\n';

	std::cout << "Player1 Controls: " << '\n';
	std::cout << "Movement: Controller 1 DPad and WASD" << '\n';
	std::cout << "E or Button B to Place Bomb for sound" << '\n';

	std::cout << '\n';
	std::cout << '\n';

	std::cout << "Player2 Controls: " << '\n';
	std::cout << "Movement: Controller 2 DPad and arrow keys" << '\n';
	std::cout << "M or Button B to add Score" << '\n';

	std::cout << '\n';
	std::cout << '\n';
	
	std::cout << '\n';
	std::cout << '\n';
	 
}


int main(int, char* []) {

	Jotar::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}