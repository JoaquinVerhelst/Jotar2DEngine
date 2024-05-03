#pragma once



#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "WorldGrid.h"


namespace Jotar
{
	class GameObject;


	enum class GameMode
	{
		SinglePlayer,
		Coop,
		Versus
	};

	class GameManager final : public Singleton<GameManager>
	{
	public:
		~GameManager() = default;



		void SetPlayerOne(std::shared_ptr<GameObject> ptr);
		void SetPlayerTwo(std::shared_ptr<GameObject> ptr);

		std::shared_ptr<GameObject> GetPlayerOne();
		std::shared_ptr<GameObject> GetPlayerTwo();


		void ResetAndInitializeWorldGrid(int rows, int columns, int size);

		WorldGrid* GetWorldGrid();




	private:
		friend class Singleton<GameManager>;
		GameManager() = default;

		////Change to Vector??
		std::shared_ptr<GameObject> m_PlayerOne{ nullptr };
		std::shared_ptr<GameObject> m_PlayerTwo{ nullptr };

		std::unique_ptr<WorldGrid> m_pWorldGrid;

		GameMode m_GameMode{ GameMode::SinglePlayer };
	};
}



