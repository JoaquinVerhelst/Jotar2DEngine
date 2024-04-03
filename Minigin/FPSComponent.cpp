#include "FPSComponent.h"
#include "TextComponent.h"

#include "GameObject.h"
#include "WorldTimeManager.h"
#include <string>


Jotar::FPSComponent::FPSComponent(GameObject* owner,  float timeToUpdate)
	: Component(owner),
	m_Count{ 0.5f },
	m_TimeToUpdate{ timeToUpdate },
	m_pTextComponent{}
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}



void Jotar::FPSComponent::Update()
{
	m_Count += WorldTimeManager::GetInstance().GetDeltaTime();

	if (m_Count >= m_TimeToUpdate)
	{
		m_pTextComponent->SetText("FPS: " + std::to_string(static_cast<int>(WorldTimeManager::GetInstance().GetFPS())));
	}
}
