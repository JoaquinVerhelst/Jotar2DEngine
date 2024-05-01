#include "PickUpComponent.h"
#include "PlaceBombComponent.h"
#include <cstdlib> // For std::rand
#include <ctime>   // For std::time
#include "TextureComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SoundServiceLocator.h"


Jotar::PickUpComponent::PickUpComponent(GameObject* owner)
	:Component(owner)
{
    //m_pTextureComponent = GetOwner()->GetComponent<TextureComponent>();
    RandomizePickUpType();
}

void Jotar::PickUpComponent::OnNotify(const CollisionEvent& eventData)
{
    if (typeid(eventData) == typeid(TriggerBeginEvent))
    {
        auto* otherCollider = eventData.GetOtherCollider();

        if (otherCollider->GetOwner()->HasComponent<PlaceBombComponent>())
        {
            SoundServiceLocator::GetSoundSystem().Play(2, 80);

            switch (m_PickUpType)
            {
            case Jotar::PickUpType::ExtraBomb:
                eventData.GetOtherCollider()->GetOwner()->GetComponent<PlaceBombComponent>()->UpgradeMaxAmountOfBombs();
                break;
            case Jotar::PickUpType::Detonator:
                eventData.GetOtherCollider()->GetOwner()->GetComponent<PlaceBombComponent>()->GiveDetonator();
                break;
            case Jotar::PickUpType::ExtraExplosionRange:
                eventData.GetOtherCollider()->GetOwner()->GetComponent<PlaceBombComponent>()->UpgradeExplosionRange();
                break;
            default:
                break;
            }

            GetOwner()->Destroy();
        }
    }
}

void Jotar::PickUpComponent::OnDestroy()
{
}


void Jotar::PickUpComponent::RandomizePickUpType()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int randomValue = std::rand() % 3;

    switch (randomValue)
    {
    case 0:
        m_PickUpType = PickUpType::ExtraBomb;
        GetOwner()->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("ExtraBombPickUp"));
        break;
    case 1:
        m_PickUpType = PickUpType::Detonator;
        GetOwner()->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("DetonatorPickUp"));
        break;
    case 2:
        m_PickUpType = PickUpType::ExtraExplosionRange;
        GetOwner()->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("ExplosionRangePickUp"));
        break;
    default:
        break;
    }
}
