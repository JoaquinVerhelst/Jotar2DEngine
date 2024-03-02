#pragma once

#include "TransformComponent.h"
#include "Component.h"
#include "Texture2D.h"
#include <SDL_pixels.h>
#include <memory>

namespace dae
{
	class Font;
	class TextComponent;

	class FPSComponent final : public Component
	{
	public:

		explicit FPSComponent(GameObject* owner, const std::shared_ptr<Font>& pFont, const SDL_Color& color, float timeToUpdate = 0.5f);
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

		std::shared_ptr<TextComponent> m_TextComponent{};

		float m_Count;
		float m_TimeToUpdate;
	};
}

