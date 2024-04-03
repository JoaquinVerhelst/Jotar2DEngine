#pragma once
#include "Component.h"
#include <memory>




namespace Jotar
{
	class PlaceBombComponent : public Component
	{
	public:

		explicit PlaceBombComponent(GameObject* owner);
		virtual ~PlaceBombComponent() = default;


		PlaceBombComponent(const PlaceBombComponent& other) = delete;
		PlaceBombComponent(PlaceBombComponent&& other) = delete;
		PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
		PlaceBombComponent& operator=(PlaceBombComponent&& other) = delete;



		void PlaceBomb();

	private:

		std::shared_ptr<GameObject> CreateBombGameObject();





		//std::shared_ptr<GameObject> m_BombToPlace;


	};
}
