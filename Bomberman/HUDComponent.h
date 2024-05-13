#pragma once
#include "Component.h"
#include "Observer.h"
#include "WindowEvents.h"

namespace Jotar
{
	class GameObject;
	class TextureComponent;

	class HUDComponent : public Component, public Observer<WindowResizeEvent>
	{
	public:
		HUDComponent(GameObject* owner);

		void Start() override;


		void OnNotify(const WindowResizeEvent& event) override;

		HUDComponent(const HUDComponent& other) = delete;
		HUDComponent(HUDComponent&& other) = delete;
		HUDComponent& operator=(const HUDComponent& other) = delete;
		HUDComponent& operator=(HUDComponent&& other) = delete;
	private:

		void CalculateScreenPos();
		TextureComponent* m_pTextureComponent;

	};

}