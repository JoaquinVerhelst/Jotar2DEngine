#include "AIAnimationControllerComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"


Jotar::AIAnimationControllerComponent::AIAnimationControllerComponent(GameObject* owner, AnimationIndexesInfo animationIndexesInfo)
	: Component(owner)
	, m_AnimationIndexesInfo{ animationIndexesInfo }
	, m_pTextureComponent{ }
{
}

void Jotar::AIAnimationControllerComponent::Start()
{
	m_pTextureComponent = GetOwner()->GetComponent<TextureComponent>();
}

void Jotar::AIAnimationControllerComponent::SetWalkAnimation(glm::ivec2 direction)
{
	if (direction.x == 1 || direction.y == 1)
		m_pTextureComponent->SetSelectedFrames(m_AnimationIndexesInfo.RightWalkIndexes);
	if (direction.x == -1 || direction.y == -1)
		m_pTextureComponent->SetSelectedFrames(m_AnimationIndexesInfo.LeftWalkIndexes);
}

void Jotar::AIAnimationControllerComponent::SetDamageAnimation()
{
	m_pTextureComponent->SetSelectedFrames(m_AnimationIndexesInfo.DamageIndexes);
}

void Jotar::AIAnimationControllerComponent::SetDeathAnimation()
{
	m_pTextureComponent->SetSelectedFrames(m_AnimationIndexesInfo.DeathIndexes);
}



void Jotar::AIAnimationControllerComponent::SetDestroyOnLastFrame(bool destroyItOnLastFrame)
{
	m_pTextureComponent->SetDestroyOnLastFrame(destroyItOnLastFrame);
}
