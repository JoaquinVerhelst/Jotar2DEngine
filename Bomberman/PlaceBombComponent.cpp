#include "PlaceBombComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "BombComponent.h"
#include "WorldGrid.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "SoundServiceLocator.h"

Jotar::PlaceBombComponent::PlaceBombComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::PlaceBombComponent::PlaceBomb()
{

	auto bomb = CreateBombGameObject();
	auto cell = WorldGrid::GetInstance().GetGridCellByPosition(GetOwner()->GetTransform()->GetWorldPosition());
	bomb->GetTransform()->SetPosition(cell.CenterCellPosition);
	SoundServiceLocator::GetSoundSystem().Play(0);
}

std::shared_ptr<Jotar::GameObject> Jotar::PlaceBombComponent::CreateBombGameObject()
{
	// TODO get rid of hardcoded scene index
	Scene& scene = SceneManager::GetInstance().GetScene(0);
	auto bomb = scene.CreateGameObject("Bomb");
	auto size = WorldGrid::GetInstance().GetCellSize() / 2;
	glm::vec2 sizeVec = { size, size };

	bomb->GetTransform()->SetSize({ sizeVec });
	auto triggerCollider = bomb->AddComponent<ColliderComponent>(false, true);
	auto bombComp = bomb->AddComponent<BombComponent>(5.f);
	triggerCollider->AddObserver(bombComp);

	bomb->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("Bomb"));
	return bomb;
}


