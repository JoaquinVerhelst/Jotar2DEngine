#pragma once

#include "Command.h"
#include "MenuComponent.h"

namespace Jotar
{

	class GameObject;

	class MenuCommand : public Command
	{
	public:
		MenuCommand(MenuComponent* pMenu);
		virtual ~MenuCommand() = default;
	protected:
		MenuComponent* m_pMenuComponent;
	};


	class SelectUpCommand final : public MenuCommand
	{
	public:
		SelectUpCommand(MenuComponent* pMenu)
			: MenuCommand(pMenu) {}

		virtual void Execute() override;
	};

	class SelectDownCommand final : public MenuCommand
	{
	public:
		SelectDownCommand(MenuComponent* pMenu)
			: MenuCommand(pMenu) {}

		virtual void Execute() override;
	};

	class PressButtonCommand final : public MenuCommand
	{
	public:
		PressButtonCommand(MenuComponent* pMenu)
			: MenuCommand(pMenu) {}

		virtual void Execute() override;
	};

}