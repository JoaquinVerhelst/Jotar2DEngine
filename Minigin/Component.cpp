
#include "Component.h"
#include <cassert>
#include "GameObject.h"

Jotar::Component::Component(GameObject* pOwner)
	: m_pOwner(pOwner)
{}
