#include "CollisionManager.h"
#include "GameObject.h"
#include <iostream>


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

        if (pCollider == nullptr)
        {
            RemoveCollider(pCollider);
            continue;
        }

        // A static object will not trigger a collision
        if (pCollider->GetIsStatic()) continue;


        for (auto pOtherCollider : m_pSceneColliders)
        {

            if (pOtherCollider == nullptr)
            {
                RemoveCollider(pOtherCollider);
                continue;
            }

            if (pCollider == pOtherCollider) continue;

            if (pCollider->IsOverlapping(pOtherCollider->GetCollisionRect()))
            {
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

                    const auto& collisionRect = pCollider->GetCollisionRect();
                    const auto& otherCollisionRect = pOtherCollider->GetCollisionRect();

                    glm::vec2 offset{};
     
                    float horizontalOverlap = 0.0f;
                    if (collisionRect.x < otherCollisionRect.x)
                        horizontalOverlap = otherCollisionRect.x - (collisionRect.x + collisionRect.w);
                    else
                        horizontalOverlap = (otherCollisionRect.x + otherCollisionRect.w) - collisionRect.x;

                    float verticalOverlap = 0.0f;
                    if (collisionRect.y < otherCollisionRect.y)
                        verticalOverlap = otherCollisionRect.y - (collisionRect.y + collisionRect.z);
                    else
                        verticalOverlap = (otherCollisionRect.y + otherCollisionRect.z) - (collisionRect.y);

                    if (std::abs(horizontalOverlap) < std::abs(verticalOverlap)) {
       
                        offset.x = horizontalOverlap;
                    }
                    else {
   
                        offset.y = verticalOverlap;
                    }
                    pCollider->GetTransform()->Translate(offset);
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

std::vector<Jotar::ColliderComponent*> Jotar::CollisionManager::GetOverlappingColliders(ColliderComponent*) const
{
    return std::vector<Jotar::ColliderComponent*>{nullptr};
}