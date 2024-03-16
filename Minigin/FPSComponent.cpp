#include "FPSComponent.h"
#include "TextComponent.h"

#include "GameObject.h"
#include "WorldTimeManager.h"
#include <string>


Jotar::FPSComponent::FPSComponent(GameObject* owner, const std::shared_ptr<Font>& pFont, const SDL_Color& color, float timeToUpdate)
	: Component(owner),
	m_Count{ 0.5f },
	m_TimeToUpdate{ timeToUpdate },
	m_TextComponent{}
{
	m_TextComponent = GetOwner()->AddComponent<TextComponent>("", pFont, color);
}



void Jotar::FPSComponent::Update()
{
	m_Count += WorldTimeManager::GetInstance().GetDeltaTime();

	if (m_Count >= m_TimeToUpdate)
	{
		m_TextComponent->SetText("FPS: " + std::to_string(static_cast<int>(WorldTimeManager::GetInstance().GetFPS())));
	}
}
