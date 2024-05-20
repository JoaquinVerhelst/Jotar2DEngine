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
        if (pCollider->IsDisabled()) continue;




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


Jotar::ColliderComponent* Jotar::CollisionManager::RaycastLookForCollider(glm::vec2 startpos, glm::vec2 direction, float distance, std::vector<std::string> tagsToFind)
{
    glm::vec2 dir = glm::normalize(direction);

    struct RaycastHit {
        Jotar::ColliderComponent* collider;
        float distance;
    };

    std::vector<RaycastHit> hits;

    for (const auto& collider : m_pSceneColliders)
    {
        // Calculate collision rectangle for the collider
        glm::vec4 collisionRect = collider->GetCollisionRect();

        float hitDistance;
        if (RayBoxIntersection(startpos, dir, collisionRect, hitDistance) && hitDistance <= distance)
        {
            hits.push_back({ collider, hitDistance });
        }
    }

    // Sort hits by distance
    std::sort(hits.begin(), hits.end(), [](const RaycastHit& a, const RaycastHit& b) {
        return a.distance < b.distance;
        });

    for (const auto& hit : hits)
    {
        bool canCollide = std::any_of(tagsToFind.begin(), tagsToFind.end(), [&hit](const std::string& tag) {
            return hit.collider->CompareTag(tag);
            });

        if (canCollide)
        {
            return hit.collider;
        }
    }

    return nullptr;
}


void Jotar::CollisionManager::Reset()
{
    m_pSceneColliders.clear();
}

bool Jotar::CollisionManager::RayBoxIntersection(const glm::vec2& rayOrigin, const glm::vec2& rayDir, const glm::vec4& box, float& t)
{
    float tmin = (box.x - rayOrigin.x) / rayDir.x;
    float tmax = (box.z - rayOrigin.x) / rayDir.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (box.y - rayOrigin.y) / rayDir.y;
    float tymax = (box.w - rayOrigin.y) / rayDir.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    t = tmin;
    return true;
}