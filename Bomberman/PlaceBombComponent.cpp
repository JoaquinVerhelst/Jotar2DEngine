#include "PlaceBombComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "WorldGrid.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "SoundServiceLocator.h"


#include <iostream>


Jotar::PlaceBombComponent::PlaceBombComponent(GameObject* owner)
	: Component(owner)
	, m_MaxAmountOfBombs{ 1 }
	, m_AmountOfFlames{ 1 }
	, m_BombTimer{ 5.f }
	, m_HasDetonator{ false }
{
}

void Jotar::PlaceBombComponent::OnNotify(const ExplosionEvent&)
{
	std::cout << "BombRemoved" << '\n';
	m_BombsPlaced.erase(m_BombsPlaced.begin());
}

void Jotar::PlaceBombComponent::UpgradeExplosionRange()
{
	++m_AmountOfFlames;
}

void Jotar::PlaceBombComponent::UpgradeMaxAmountOfBombs()
{
	++m_MaxAmountOfBombs;
}

void Jotar::PlaceBombComponent::GiveDetonator()
{
	m_HasDetonator = true;
}

void Jotar::PlaceBombComponent::PlaceBomb()
{
	if (m_BombsPlaced.size() >= m_MaxAmountOfBombs) return;

	auto& cell = WorldGrid::GetInstance().GetGridCellByPosition(GetOwner()->GetTransform()->GetWorldPosition());

	if (cell.ObjectOnCell.expired())
	{
		auto bomb = CreateBombGameObject();

		bomb->GetTransform()->SetPosition(cell.CenterCellPosition);
		cell.ObjectOnCell = bomb;
		SoundServiceLocator::GetSoundSystem().Play(0);
	}

}

void Jotar::PlaceBombComponent::DetonateBomb()
{
	if (!m_HasDetonator) return;

	for (size_t i = 0; i < m_BombsPlaced.size(); i++)
	{
		if (m_BombsPlaced[i] != nullptr)
		{
			m_BombsPlaced[i]->OnExplode(m_AmountOfFlames);
			return;
		}
	}
}

std::shared_ptr<Jotar::GameObject> Jotar::PlaceBombComponent::CreateBombGameObject()
{
	// TODO get rid of hardcoded scene index
	Scene& scene = SceneManager::GetInstance().GetScene(0);
	auto bombObj = scene.CreateGameObject("Bomb");
	auto size = WorldGrid::GetInstance().GetCellSize() / 1.2f;
	glm::vec2 sizeVec = { size, size };

	bombObj->GetTransform()->SetSize({ sizeVec });
	auto triggerCollider = bombObj->AddComponent<ColliderComponent>(true, false);
	auto bombComp = bombObj->AddComponent<BombComponent>(m_BombTimer, m_AmountOfFlames);
	triggerCollider->AddObserver(bombComp);
	bombComp->AddObserver(this);
	m_BombsPlaced.emplace_back(bombComp);

	bombObj->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("Bomb"));
	return bombObj;
}


