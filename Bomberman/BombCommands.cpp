#include "BombCommands.h"
#include "GameObject.h"
#include "BombCommands.h"


Jotar::PlaceBombCommand::PlaceBombCommand(PlaceBombComponent* bomb)
	: m_pBombComp{ bomb } {}


void Jotar::PlaceBombCommand::Execute()
{
	if (!m_pBombComp->IsDisabled()) m_pBombComp->PlaceBomb();
}

Jotar::DetonateBombCommand::DetonateBombCommand(PlaceBombComponent* bomb)
	: m_pBombComp{ bomb } {}

void Jotar::DetonateBombCommand::Execute()
{
	if (!m_pBombComp->IsDisabled()) m_pBombComp->DetonateBomb();
}
