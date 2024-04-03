#pragma once

#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace Jotar
{
    class Texture2D;
    class string;
    class Font;



 



    struct SpriteSheet
    {

        enum class SpriteSheetDirection
        {
            Right,
            Down
        };



        std::unique_ptr<Texture2D> pTexture = nullptr;
        int ClipHeight = 0;
        int ClipWidth = 0;

        int TotalColumns = 0;
        int TotalRows = 0;

        int CurrentColumn = 0;
        int CurrentRow = 0;

        SpriteSheetDirection Direction = SpriteSheetDirection::Right;
    };


    class TextureComponent : public Component {
    public:
        explicit TextureComponent(GameObject* owner, const std::string& filePath, bool isStatic = true, int columns = 1, int rows = 1);
        virtual ~TextureComponent() override = default;

        virtual void Update() override;
        virtual void Render() const override;

        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator=(const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;



        void SetIsStatic(bool isStatic) { m_IsStatic = isStatic; }
        bool GetIsStatic() { return m_IsStatic; }

        void UpdateFrame();


    private:
        SpriteSheet m_SpriteSheet;

        glm::ivec4 m_SrcRect;

        bool m_IsStatic;
        int m_NrFramesPerSec;
        float m_AnimTime;
        int m_AnimFrame;


    };


}

