#pragma once
#include "Command.h"

#include "PlaceBombComponent.h"


namespace Jotar
{

	class GameObject;

	class PlaceBombCommand final : public Command
	{
	public:
		PlaceBombCommand(PlaceBombComponent* bomb);
		virtual void Execute() override;

	private:
		PlaceBombComponent* m_pBombComp{};

	};
}