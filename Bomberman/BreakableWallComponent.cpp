#include "BreakableWallComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PickUpComponent.h"
#include "ColliderComponent.h"

#include "GameManager.h"
#include "WorldGrid.h"

void Jotar::BreakableWallComponent::OnWallBreak()
{
	m_OnWallBreak();
}

Jotar::BreakableWallComponent::BreakableWallComponent(GameObject* owner, const std::function<void()>& onWallBreak)
	: Component(owner)
	, m_OnWallBreak{ onWallBreak }
{
}

void Jotar::BreakableWallComponent::OnDestroy()
{

}
