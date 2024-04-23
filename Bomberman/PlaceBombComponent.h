#pragma once
#include "Component.h"
#include <memory>
#include "Observer.h"
#include "ExplosionEvent.h"
#include <vector>
#include "BombComponent.h"

namespace Jotar
{

	class PlaceBombComponent : public Component , public Observer<ExplosionEvent>
	{
	public:

		explicit PlaceBombComponent(GameObject* owner);
		virtual ~PlaceBombComponent() = default;

		void OnNotify(const ExplosionEvent& eventData) override;

		PlaceBombComponent(const PlaceBombComponent& other) = delete;
		PlaceBombComponent(PlaceBombComponent&& other) = delete;
		PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
		PlaceBombComponent& operator=(PlaceBombComponent&& other) = delete;

		void UpgradeExplosionRange();
		void UpgradeMaxAmountOfBombs();
		void GiveDetonator();

		void PlaceBomb();
		void DetonateBomb();

	private:

		std::shared_ptr<GameObject> CreateBombGameObject();


		int m_MaxAmountOfBombs;
		//int m_CurrentPlacedBombs;
		std::vector<BombComponent*> m_BombsPlaced;

		bool m_HasDetonator;
		int m_AmountOfFlames;
		float m_BombTimer;


		//std::shared_ptr<GameObject> m_BombToPlace;


	};
}
