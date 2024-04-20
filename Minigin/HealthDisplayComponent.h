#pragma once


#include "Observer.h"
#include "Component.h"
#include "TextComponent.h"
#include "HealthEvents.h"
    
namespace Jotar
{
    class HealthDisplayComponent final : public Component, public Observer<HealthEvent>
    {
    public:

        explicit HealthDisplayComponent(GameObject* owner);
        virtual ~HealthDisplayComponent() = default;

        HealthDisplayComponent(const HealthDisplayComponent& other) = delete;
        HealthDisplayComponent(HealthDisplayComponent&& other) = delete;
        HealthDisplayComponent& operator=(const HealthDisplayComponent& other) = delete;
        HealthDisplayComponent& operator=(HealthDisplayComponent&& other) = delete;

        void OnNotify(const HealthEvent& event) override;

    private:

        TextComponent* m_pTextComponent;
    };
}

