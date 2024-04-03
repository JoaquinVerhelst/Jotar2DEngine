#include "BombComponent.h"
#include "WorldTimeManager.h"

#include "GameObject.h"


Jotar::BombComponent::BombComponent(GameObject* owner, GameObject*, float explodeTime)
	: Component(owner)
	, m_ExplodeTime{ explodeTime }
	, m_IsExploded { false }
	, m_TimeCounter { 0 }
	, m_CurrentGridCell{ {} }
{
}

void Jotar::BombComponent::Update()
{
	if (!m_IsExploded)
	{
		m_TimeCounter += WorldTimeManager::GetInstance().GetDeltaTime();

		if (m_TimeCounter >= m_ExplodeTime)
		{
			m_IsExploded = true;
			m_TimeCounter = 0;
			OnExplode();

		}
	}
}


void Jotar::BombComponent::Activate(const glm::vec2& pos)
{
	m_IsExploded = false;
	GetOwner()->GetTransform()->SetPosition(pos);
	m_CurrentGridCell = WorldGrid::GetInstance().GetGridCellByPosition(pos);



}

void Jotar::BombComponent::OnExplode()
{


}

void Jotar::BombComponent::Reset()
{


}
