#pragma once
#include "Component.h"
#include "WorldGrid.h"
#include <memory>


namespace Jotar
{
	class GameObject;

	class ExplosionComponent : public Component
	{
	public:
		ExplosionComponent( GameObject* owner);

		void Update() override;


	private:

	};
}
