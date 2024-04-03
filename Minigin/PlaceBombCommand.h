#pragma once
#include "Command.h"

#include "BombComponent.h"


namespace Jotar
{

	class GameObject;

	class PlaceBombCommand final : public Command
	{
	public:
		PlaceBombCommand(GameObject* player, BombComponent* bomb);
		virtual void Execute() override;

	private:
		GameObject* m_pPlayer{};
		BombComponent* m_pBombComp{};

	};
}