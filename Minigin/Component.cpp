
#include "Component.h"

dae::Component::Component(GameObject* pOwner)
	: m_pOwner(pOwner)
{}

void dae::Component::SetOwner(GameObject* newOwner)
{
	m_pOwner = newOwner;
}
