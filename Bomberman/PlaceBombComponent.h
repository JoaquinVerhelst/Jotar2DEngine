#pragma once
#include "Component.h"
#include <memory>
#include "Observer.h"
#include "ExplosionEvent.h"
#include <vector>
#include "BombComponent.h"

namespace Jotar
{
	struct GridCell;

	class PlaceBombComponent : public Component , public Observer<ExplosionEvent>
	{
	public:

		explicit PlaceBombComponent(GameObject* owner, float bombTime = 4.0f, int startAmountOfBombs = 1, int startAmountOfFlames = 1);
		virtual ~PlaceBombComponent() = default;

		void OnNotify(const ExplosionEvent& eventData) override;

		PlaceBombComponent(const PlaceBombComponent& other) = delete;
		PlaceBombComponent(PlaceBombComponent&& other) = delete;
		PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
		PlaceBombComponent& operator=(PlaceBombComponent&& other) = delete;

		void Reset() override;

		void UpgradeExplosionRange();
		void UpgradeMaxAmountOfBombs();
		void GiveDetonator();

		void PlaceBomb();
		void DetonateBomb();

	private:

		std::shared_ptr<GameObject> CreateBombGameObject(GridCell& cell);


		int m_StartAmountOfBombs;
		int m_StartAmountOfFlames;


		int m_MaxAmountOfBombs;
		//int m_CurrentPlacedBombs;
		std::vector<BombComponent*> m_BombsPlaced;

		bool m_HasDetonator;
		int m_AmountOfFlames;
		float m_BombTimer;


		//std::shared_ptr<GameObject> m_BombToPlace;


	};
}
