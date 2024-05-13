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

		ColliderComponent* RaycastLookForCollider(glm::vec2 startpos, glm::vec2 direction, float distance, std::vector<std::string> tagsToFind);

		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) = delete;

	private:

		bool RayBoxIntersection(const glm::vec2& startpos, const glm::vec2& direction, const glm::vec4& box, float maxDistance);



		std::vector<ColliderComponent*> m_pSceneColliders;

	};
}
