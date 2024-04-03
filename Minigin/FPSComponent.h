#pragma once

#include "TransformComponent.h"
#include "Component.h"
#include "Texture2D.h"
#include <SDL_pixels.h>
#include <memory>

namespace Jotar
{
	class Font;
	class TextComponent;

	class FPSComponent final : public Component
	{
	public:

		explicit FPSComponent(GameObject* owner, float timeToUpdate = 0.5f);
		~FPSComponent() = default;

		virtual void Update() override;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;


		float GetTimeToUpdate() { return m_TimeToUpdate; }
		void SetTimeToUpdate(float timeToUpdate) { m_TimeToUpdate = timeToUpdate; }


		void SetUpdateTime(float time) { m_TimeToUpdate = time; }
	private:

		TextComponent* m_pTextComponent;

		float m_Count;
		float m_TimeToUpdate;
	};
}

