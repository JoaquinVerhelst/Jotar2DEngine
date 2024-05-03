#pragma once

#include "Component.h"
#include <glm/glm.hpp>


namespace Jotar
{
    class TransformComponent;

	class MovementComponent : public Component
	{
    public:
        explicit MovementComponent(GameObject* owner, float movementSpeed);

        MovementComponent(const MovementComponent& other) = delete;
        MovementComponent(MovementComponent&& other) = delete;
        MovementComponent& operator=(const MovementComponent& other) = delete;
        MovementComponent& operator=(MovementComponent&& other) = delete;

        void Move(const glm::ivec2& dir);
        TransformComponent* GetTransform();
    private:
        glm::vec2 Walk(glm::ivec2 direction, const glm::vec2& currentPosition);
        glm::vec2 CalculateWalk(int direction, float x, float y);
        float m_MovementSpeed;
        TransformComponent* m_pTransformComponent;



	};

}