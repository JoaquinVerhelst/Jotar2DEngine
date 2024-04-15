#pragma once
#include "Singleton.h"
#include "ColliderComponent.h"
#include <vector>

namespace Jotar
{
	class CollisionManager final
	{
	public:

		void AddCollider(ColliderComponent* pCollider);
		void RemoveCollider(ColliderComponent* pCollider);

		void FixedUpdate();

	private:
		std::vector<ColliderComponent*> GetOverlappingColliders(ColliderComponent* pCollider) const;

		std::vector<ColliderComponent*> m_pSceneColliders;

	};
}
