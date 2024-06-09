//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "GameManager.h"
#include <iostream>

using namespace Jotar;

void load()
{

	GameManager::GetInstance().Init("../Data/Json/Levels.json");


	std::cout << '\n';
	std::cout << '\n';

	std::cout << "General Controls: " << '\n';
	std::cout << "Mute: M" << '\n';
	std::cout << "SkipLevel: F1" << '\n';
	std::cout << "mainMenu: Escape" << '\n';
	std::cout << "increaseMusicVolume: ]" << '\n';
	std::cout << "decreaseMusicVolume: [" << '\n';
	std::cout << "increaseSoundEffectsVolume: +" << '\n';
	std::cout << "decreaseSoundEffectsVolume: -" << '\n';


	std::cout << '\n';
	std::cout << '\n';



	std::cout << "Player 0 Controls: " << '\n';
	std::cout << "Movement: Controller 1 DPad and WASD" << '\n';
	std::cout << "E or Button B to Place Bomb" << '\n';
	std::cout << "Q or Button A to Detonate Bomb" << '\n';
	std::cout << '\n';
	std::cout << '\n';

	std::cout << "Player 1 Controls: " << '\n';
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