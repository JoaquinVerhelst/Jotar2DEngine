#pragma once


#include "Observer.h"
#include "Component.h"
#include "TextComponent.h"
#include "PlayerHealthEvents.h"
    
namespace Jotar
{
    class HealthDisplayComponent final : public Component, public Observer<PlayerHealthEvent>
    {
    public:

        explicit HealthDisplayComponent(GameObject* owner);
        virtual ~HealthDisplayComponent() = default;

        HealthDisplayComponent(const HealthDisplayComponent& other) = delete;
        HealthDisplayComponent(HealthDisplayComponent&& other) = delete;
        HealthDisplayComponent& operator=(const HealthDisplayComponent& other) = delete;
        HealthDisplayComponent& operator=(HealthDisplayComponent&& other) = delete;

        void OnNotify(const PlayerHealthEvent& event) override;

    private:

        TextComponent* m_pTextComponent;
    };
}

