#pragma once

#include "Component.h"
#include <memory>
#include <string>
#include <SDL_pixels.h>


namespace dae
{
    class Texture2D;
    class string;
    class Font;

    class TextComponent final : public Component {
    public:

        explicit TextComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, bool needUpdating = false);
        virtual ~TextComponent() override = default;


        virtual void Update() override;
        void Render() const override;

        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) = delete;

        void SetText(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);
        void SetText(const std::string& text);


    private:
        void UpdateText();


        bool m_NeedsUpdate;
        bool m_DoOnce;

        std::string m_Text;

        std::shared_ptr<Font> m_Font;

        std::unique_ptr<Texture2D> m_Texture;

        SDL_Color m_Color{ 255, 255, 255, 255 };
    };


}