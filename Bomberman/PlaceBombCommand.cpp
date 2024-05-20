#include "PlaceBombCommand.h"
#include "GameObject.h"


Jotar::PlaceBombCommand::PlaceBombCommand(PlaceBombComponent* bomb)
	: m_pBombComp{ bomb }
{
}

void Jotar::PlaceBombCommand::Execute()
{
	if (!m_pBombComp->IsDisabled()) m_pBombComp->PlaceBomb();
}
