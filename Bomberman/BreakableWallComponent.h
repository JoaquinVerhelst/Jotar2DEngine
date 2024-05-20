#pragma once
#include "Component.h"
#include <functional>


namespace Jotar
{
	class BreakableWallComponent : public Component
	{
	public:



		void OnWallBreak();

		explicit BreakableWallComponent(GameObject* owner, const std::function<void()>& OnWallBreak);
		virtual ~BreakableWallComponent() = default;

		virtual void OnDestroy() override;

		BreakableWallComponent(const BreakableWallComponent& other) = delete;
		BreakableWallComponent(BreakableWallComponent&& other) = delete;
		BreakableWallComponent& operator=(const BreakableWallComponent& other) = delete;
		BreakableWallComponent& operator=(BreakableWallComponent&& other) = delete;

	private:
		std::function<void()> m_OnWallBreak;
	};
}

