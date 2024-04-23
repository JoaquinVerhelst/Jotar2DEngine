#pragma once
#include "Command.h"
#include "MovementComponent.h"


namespace Jotar
{
	class MovementCommand : public Command
	{
	public:
		MovementCommand(MovementComponent* pMovement, const glm::vec2& dir)
			:m_pMovement{ pMovement }
			, m_Dir{ dir }
		{
		}
		void Execute() override
		{
			if (m_pMovement) m_pMovement->Move(glm::vec3{ m_Dir.r,m_Dir.g,0 });
		}

	private:
		MovementComponent* m_pMovement;
		glm::vec2 m_Dir;
	};
}