#pragma once

#include "BaseTextureComponent.h"
#include "Texture2D.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "Enums.h"

namespace Jotar
{
    class Texture2D;
    class string;
    class Font;




    class TextureComponent final : public BaseTextureComponent {
    public:
        explicit TextureComponent(GameObject* owner, const std::string& filePath, bool isStatic = true, int columns = 1, int rows = 1);
        explicit TextureComponent(GameObject* owner, std::shared_ptr<Texture2D> texture);
        explicit TextureComponent(GameObject* owner, SpriteSheet& spriteSheet, int currentRow = 0, int currentColumn = 0);

        ~TextureComponent() override = default;

        void Update() override;
        void Render() const override;

 
        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator=(const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;

        void SetTexture(std::shared_ptr<Texture2D> texture);

        void SetDestroyOnLastFrame(bool newValue);
        void SetCurrentRow(int currentRow);
        void SetCurrentColumn(int currentColumn);
        void SetSize(glm::ivec2 size);
        void SetIsStatic(bool isStatic) { m_IsStatic = isStatic; }
        bool GetIsStatic() const { return m_IsStatic; }

        void SetAnimationSpeedInNrOfFramesPerSecond(int animSpeed);

        void SetSelectedFrames(glm::ivec2 startAndEndFrames);



        void UpdateFrame();


    private:
        void Initizialize();
        void InitSpriteSheet(int columns, int rows);
        void CheckForDestroy(int currentFrame, int totalFrames);

        SpriteSheet m_SpriteSheet;


        glm::ivec2 m_SelectedFramesIndexes;
        glm::ivec4 m_SrcRect;

        glm::ivec2 m_Size;
        bool m_IsStatic;
        bool m_DestroyOnLastFrame = false;
        int m_NrFramesPerSec;
        float m_AnimTime;
        int m_AnimFrame;

    };


}

