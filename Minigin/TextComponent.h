#pragma once

#include "BaseTextureComponent.h"
#include <memory>
#include <string>
#include <SDL_pixels.h>
#include <glm/glm.hpp>

namespace Jotar
{
    class Texture2D;
    class string;
    class Font;

    class TextComponent final : public BaseTextureComponent {
    public:

        explicit TextComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color = SDL_Color{ 255, 255, 255, 255 }, bool needUpdating = false);
        virtual ~TextComponent() override = default;


        void Update() override;
        void Render() const override;

        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) = delete;

        void SetText(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);
        void SetText(const std::string& text);

        void UpdateText();
    private:



        bool m_NeedsUpdate;
        bool m_DoOnce;

        std::string m_Text;

        std::shared_ptr<Font> m_Font;

        std::unique_ptr<Texture2D> m_Texture;
        glm::ivec2 m_TextSize;

        SDL_Color m_Color{ 255, 255, 255, 255 };
    };


}