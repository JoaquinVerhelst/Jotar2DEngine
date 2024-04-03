#pragma once

#include "Observer.h"
#include "Component.h"
#include "TextComponent.h"
#include "EventAddscore.h"

namespace Jotar
{
    class ScoreDisplayComponent final : public Component, public Observer<EventAddScore>
    {
    public:

        explicit ScoreDisplayComponent(GameObject* owner);
        virtual ~ScoreDisplayComponent() = default;

        ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
        ScoreDisplayComponent(ScoreDisplayComponent&& other) = delete;
        ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
        ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) = delete;

        void OnNotify(EventAddScore* event) override;

    private:

        TextComponent* m_pTextComponent;
    };
}

