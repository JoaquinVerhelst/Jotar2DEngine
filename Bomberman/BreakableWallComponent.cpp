#include "BreakableWallComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PickUpComponent.h"
#include "ColliderComponent.h"

#include "GameManager.h"
#include "WorldGrid.h"

Jotar::BreakableWallComponent::BreakableWallComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::BreakableWallComponent::OnDestroy()
{
	//TODO get rid of hardcoded index
	auto& scene = SceneManager::GetInstance().GetSceneByID(0);
	auto pickupObj = scene.CreateGameObject("PickUp");
	auto pickupComp = pickupObj->AddComponent<PickUpComponent>();
	auto collider = pickupObj->AddComponent<ColliderComponent>(true , true);

	collider->AddObserver(pickupComp);

	auto& cell = GameManager::GetInstance().GetWorldGrid()->GetGridCellByPosition(GetOwner()->GetTransform()->GetLocalPosition());

	pickupObj->GetTransform()->SetPosition(cell.CenterCellPosition);
	collider->UpdatePosition();
}
