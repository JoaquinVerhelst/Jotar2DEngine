#pragma once
#include "Component.h"
#include "Observer.h"
#include "CollisionEvents.h"
#include "ColliderComponent.h"
#include <vector>

namespace Jotar
{
	class TextureComponent;

	enum class PickUpType
	{
		ExtraBomb = 0,
		Detonator = 1,
		ExtraExplosionRange = 2
	};



	class PickUpComponent final : public Component , public Observer<CollisionEvent>
	{
	public:

		explicit PickUpComponent(GameObject* owner);
		virtual ~PickUpComponent() = default;

		void OnNotify(const CollisionEvent& eventData) override;
		void OnDestroy() override;

		PickUpComponent(const PickUpComponent& other) = delete;
		PickUpComponent(PickUpComponent&& other) = delete;
		PickUpComponent& operator=(const PickUpComponent& other) = delete;
		PickUpComponent& operator=(PickUpComponent&& other) = delete;

	private:

		void RandomizePickUpType();

		PickUpType m_PickUpType;
	};
}