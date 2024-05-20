#pragma once
#include "Component.h"

namespace Jotar
{
	class BreakableWallComponent : public Component
	{
	public:



		void OnWallBreak();

		explicit BreakableWallComponent(GameObject* owner);
		virtual ~BreakableWallComponent() = default;

		virtual void OnDestroy() override;

		BreakableWallComponent(const BreakableWallComponent& other) = delete;
		BreakableWallComponent(BreakableWallComponent&& other) = delete;
		BreakableWallComponent& operator=(const BreakableWallComponent& other) = delete;
		BreakableWallComponent& operator=(BreakableWallComponent&& other) = delete;

	private:

	};
}

