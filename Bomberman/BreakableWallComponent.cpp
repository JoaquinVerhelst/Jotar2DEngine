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
	auto& scene = SceneManager::GetInstance().GetCurrentScene();
	auto pickupObj = scene.CreateGameObject("PickUp");
	auto pickupComp = pickupObj->AddComponent<PickUpComponent>();
	auto collider = pickupObj->AddComponent<ColliderComponent>(true, true);

	collider->AddObserver(pickupComp);

	auto& cell = GameManager::GetInstance().GetWorldGrid()->GetGridCellByPosition(GetOwner()->GetTransform()->GetLocalPosition());

	pickupObj->GetTransform()->SetPosition(cell.CenterCellPosition);
	collider->UpdatePosition();
	pickupObj->Start();
}

Jotar::BreakableWallComponent::BreakableWallComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::BreakableWallComponent::OnDestroy()
{

}
