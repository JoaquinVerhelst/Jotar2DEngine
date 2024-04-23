#include "BreakableWallComponent.h"



Jotar::BreakableWallComponent::BreakableWallComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::BreakableWallComponent::OnDestroy()
{
	//TODO: Chance on placing Item
}
