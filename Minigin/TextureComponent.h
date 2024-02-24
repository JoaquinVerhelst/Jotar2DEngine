#pragma once

#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include <string>

namespace dae
{
    class Texture2D;
    class string;
    class Font;


    class TextureComponent : public Component {
    public:
        explicit TextureComponent(GameObject* owner, const std::string& filePath);
        virtual ~TextureComponent() override = default;

        virtual void Render() const override;

        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator=(const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;

        void SetTexture(const std::string& filePath);

    private:
        std::unique_ptr<Texture2D> m_Texture;
    };


}

