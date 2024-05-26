#pragma once
#include "Singleton.h"
#include "ColliderComponent.h"
#include <vector>
#include "ThreadPool.h"
#include <future>


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
		std::future<ColliderComponent*> RayCastCollisionAsync(glm::vec2 startpos, glm::vec2 direction, float distance, std::string tags);

		bool RayCastIsColliderInRange(glm::vec2 startpos, glm::vec2 direction, float distance, std::string tags);



		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) = delete;

		void Reset();
	private:



		ColliderComponent* RayCastCollision(glm::vec2 startpos, glm::vec2 direction, float distance, std::string tagToIgnore);
		bool RayBoxIntersection(const glm::vec2& rayOrigin, const glm::vec2& rayDir, const glm::vec4& box, float& t);



		std::vector<ColliderComponent*> m_pSceneColliders;
		ThreadPool m_threadPool;
	};
}
