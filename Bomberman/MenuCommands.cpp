#include "MenuCommands.h"

Jotar::MenuCommand::MenuCommand(MenuComponent* pMenu)
	: m_pMenuComponent( pMenu )
{
}

void Jotar::SelectUpCommand::Execute()
{
	if (!m_pMenuComponent->IsDisabled()) m_pMenuComponent->ButtonSelectUp();
}

void Jotar::SelectDownCommand::Execute()
{
	if (!m_pMenuComponent->IsDisabled()) m_pMenuComponent->ButtonSelectDown();
}

void Jotar::PressButtonCommand::Execute()
{
	if (!m_pMenuComponent->IsDisabled()) m_pMenuComponent->PressButton();
}
