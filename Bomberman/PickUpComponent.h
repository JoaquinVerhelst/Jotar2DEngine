#pragma once
#include "Component.h"


namespace Jotar
{
	class PickUpComponent : public Component
	{
	public:

		explicit PickUpComponent(GameObject* owner);
		virtual ~PickUpComponent() = default;

		virtual void OnDestroy() override;

		PickUpComponent(const PickUpComponent& other) = delete;
		PickUpComponent(PickUpComponent&& other) = delete;
		PickUpComponent& operator=(const PickUpComponent& other) = delete;
		PickUpComponent& operator=(PickUpComponent&& other) = delete;

	private:

	};
}