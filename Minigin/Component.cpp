
#include "Component.h"
#include <cassert>
#include "GameObject.h"

dae::Component::Component(GameObject* pOwner)
	: m_pOwner(pOwner)
{}
