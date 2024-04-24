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

        void Move(const glm::vec2& dir);
        TransformComponent* GetTransform();
    private:

        float m_MovementSpeed;
        TransformComponent* m_pTransformComponent;
	};

}