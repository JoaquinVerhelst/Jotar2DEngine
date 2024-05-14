#include "CollisionManager.h"
#include "GameObject.h"
#include <iostream>
#include "set"
// TODO remove
#include "SDL.h"
#include "Renderer.h"

Jotar::CollisionManager::CollisionManager()
    :m_pSceneColliders{}
{
}

void Jotar::CollisionManager::AddCollider(ColliderComponent* pCollider)
{
	m_pSceneColliders.emplace_back(pCollider);
}

void Jotar::CollisionManager::RemoveCollider(ColliderComponent* pCollider)
{
    // Use std::remove_if along with a lambda function to remove the collider
    m_pSceneColliders.erase(std::remove_if(m_pSceneColliders.begin(), m_pSceneColliders.end(),
        [pCollider](ColliderComponent* collider) {
            return collider == pCollider;
        }),
        m_pSceneColliders.end());
}

void Jotar::CollisionManager::FixedUpdate()
{
    for (auto pCollider : m_pSceneColliders)
    {

        if (!pCollider)
        {
            RemoveCollider(pCollider);
            continue;
        }

        // A static object will not trigger a collision
        if (pCollider->GetIsStatic()) continue;


        for (auto pOtherCollider : m_pSceneColliders)
        {

            if (!pOtherCollider)
            {
                RemoveCollider(pOtherCollider);
                continue;
            }

            if (pCollider == pOtherCollider) continue;


            if (pCollider->IsOverlapping(pOtherCollider->GetCollisionRect()))
            {
                if (pOtherCollider->CompareTag("Bomb") && pCollider->CompareTag("Explosion"))
                {
                    std::cout << "Bomb Check!" << '\n';
                }

                if (pCollider->GetIsTrigger())
                {
                    TriggerEvent triggerEvent{ pCollider, pOtherCollider };
                    pCollider->OnTriggerCollision(triggerEvent);
                }
                else if (pOtherCollider->GetIsTrigger())
                {
                    TriggerEvent triggerEvent{ pOtherCollider, pCollider };
                    pOtherCollider->OnTriggerCollision(triggerEvent);
                }
                else
                {
                    CollideEvent collideEvent{ pCollider, pOtherCollider };
                    pCollider->OnColliderCollision(collideEvent);
                }
            }
        }
    }
}

Jotar::ColliderComponent* Jotar::CollisionManager::GetOverlappingColliderInPosition(const glm::vec2& position) const
{
    for (auto pCollider : m_pSceneColliders)
    {
        glm::vec4 colliderRect = pCollider->GetCollisionRect();

        if (position.x >= colliderRect.x && position.x < colliderRect.x + colliderRect.w &&
            position.y >= colliderRect.y && position.y < colliderRect.y + colliderRect.z)
        {
            return pCollider;
        }
    }

    return nullptr;
}


///TODO: BUG LINE GOES THRROUGH WALLS
Jotar::ColliderComponent* Jotar::CollisionManager::RaycastLookForCollider(glm::vec2 startpos, glm::vec2 direction, float distance, std::vector<std::string> tagsToFind)
{
    glm::vec2 dir = glm::normalize(direction);

    for (const auto& collider : m_pSceneColliders)
    {
        bool canCollide = std::any_of(tagsToFind.begin(), tagsToFind.end(), [&collider](const std::string& tag) {
            return collider->CompareTag(tag);
            });

        if (!canCollide)
            continue;

        // Calculate collision rectangle for the collider
        glm::vec4 collisionRect = collider->GetCollisionRect();

        if (!RayBoxIntersection(startpos, dir, collisionRect, distance))
            continue; // No intersection, continue to the next collider

        // Return the collider if there's an intersection
        return collider;
    }

    return nullptr;
}

bool Jotar::CollisionManager::RayBoxIntersection(const glm::vec2& startpos, const glm::vec2& direction, const glm::vec4& collisionRect, float maxDistance)
{
    if (startpos.x + direction.x * maxDistance + 1> collisionRect.x &&
        startpos.x < collisionRect.x + collisionRect.w &&
        startpos.y + direction.y * maxDistance + 1> collisionRect.y &&
        startpos.y < collisionRect.y + collisionRect.z)
    {
        return true;
    }       
    return false;
}
