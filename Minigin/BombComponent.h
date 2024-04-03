#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "WorldGrid.h"

namespace Jotar
{
	class BombComponent : public Component
	{
	public:

		explicit BombComponent(GameObject* owner, GameObject* explosion, float explodeTime);
		virtual ~BombComponent() = default;


		virtual void Update() override;


		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;


		void Activate(const glm::vec2& pos);



	private:

		void OnExplode();
		void Reset();

		int m_Range;
		float m_ExplodeTime;
		float m_TimeCounter;
		bool m_IsExploded;

		Jotar::GridCell m_CurrentGridCell;

	};
}
