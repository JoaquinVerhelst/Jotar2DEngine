#include "PlaceBombComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"


Jotar::PlaceBombComponent::PlaceBombComponent(GameObject* owner)
	: Component(owner)
{
}

void Jotar::PlaceBombComponent::PlaceBomb()
{
	// todo unhardcode this



	// copy m_BombToPlace and add to scene;

	//std::shared_ptr<GameObject> newBomb = std::make_shared<GameObject>(*m_BombToPlace);




}

std::shared_ptr<Jotar::GameObject> Jotar::PlaceBombComponent::CreateBombGameObject()
{

	//Scene& scene = SceneManager::GetInstance().GetScene(0);

	//auto bomb = scene.CreateGameObject("Bomb");

	//bomb->AddComponent<BombC
	//bomb->GetTransform()->SetPosition(GetOwner()->GetTransform()->GetWorldPosition());

	

	return nullptr;

}


