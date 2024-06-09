#pragma once
#include "Command.h"
#include "PlaceBombComponent.h"

namespace Jotar
{
	class PlaceBombCommand final : public Command
	{
	public:
		PlaceBombCommand(PlaceBombComponent* bomb);
		virtual void Execute() override;

	private:
		PlaceBombComponent* m_pBombComp{};

	};

	class DetonateBombCommand final : public Command
	{
	public:
		DetonateBombCommand(PlaceBombComponent* bomb);
		virtual void Execute() override;

	private:
		PlaceBombComponent* m_pBombComp{};

	};
}