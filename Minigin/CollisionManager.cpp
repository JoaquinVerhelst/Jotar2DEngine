#include "CollisionManager.h"
#include "GameObject.h"
#include <iostream>


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
        // A static object will not trigger a collision
        if (pCollider->GetIsStatic()) continue;

        for (auto pOtherCollider : m_pSceneColliders)
        {
            if (pCollider == pOtherCollider) continue;

            if (pCollider->IsOverlapping(pOtherCollider->GetCollisionRect()))
            {
                if (pOtherCollider->GetIsTrigger())
                {
                    TriggerEvent triggerEvent{ pOtherCollider, pCollider };
                    pOtherCollider->OnTriggerCollision(triggerEvent);
                }
                else
                {
                    std::cout << "IsOverlapping " << '\n';
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

std::vector<Jotar::ColliderComponent*> Jotar::CollisionManager::GetOverlappingColliders(ColliderComponent*) const
{
    return std::vector<Jotar::ColliderComponent*>{nullptr};
}