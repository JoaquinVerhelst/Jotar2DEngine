#pragma once
#include "Singleton.h"
#include "ColliderComponent.h"
#include <vector>

namespace Jotar
{
	class CollisionManager final
	{
	public:

		CollisionManager();

		void AddCollider(ColliderComponent* pCollider);
		void RemoveCollider(ColliderComponent* pCollider);

		void FixedUpdate();

		ColliderComponent* GetOverlappingColliderInPosition(const glm::vec2& position) const;

		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) = delete;

	private:

		std::vector<ColliderComponent*> GetOverlappingColliders(ColliderComponent* pCollider) const;


		std::vector<ColliderComponent*> m_pSceneColliders;

	};
}
