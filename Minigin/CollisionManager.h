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

		ColliderComponent* RayCastCollision(glm::vec2 startpos, glm::vec2 direction, float distance);

		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) = delete;

		void Reset();
	private:

		bool RayBoxIntersection(const glm::vec2& rayOrigin, const glm::vec2& rayDir, const glm::vec4& box, float& t);



		std::vector<ColliderComponent*> m_pSceneColliders;

	};
}
