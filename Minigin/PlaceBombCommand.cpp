#include "PlaceBombCommand.h"
#include "GameObject.h"


Jotar::PlaceBombCommand::PlaceBombCommand(GameObject* player, BombComponent* bomb)

	: m_pPlayer{ player }
	, m_pBombComp{ bomb }
{
}

void Jotar::PlaceBombCommand::Execute()
{

	m_pBombComp->Activate(m_pPlayer->GetTransform()->GetLocalPosition());
}
