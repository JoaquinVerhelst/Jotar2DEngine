#pragma once

#include "Component.h"
#include <memory>
#include <string>
#include <SDL_pixels.h>
#include <glm/glm.hpp>

namespace Jotar
{
    class Texture2D;
    class string;
    class Font;

    class BaseTextureComponent : public Component {
    public:

        BaseTextureComponent(GameObject* owner);
        virtual ~BaseTextureComponent() override = default;


        virtual void Start() override;
        virtual void Reset() override;

        virtual void OnDestroy() override;



        void SetLayer(int layer) { m_Layer = layer; }
        int GetLayer() const { return m_Layer; }

        BaseTextureComponent(const BaseTextureComponent& other) = delete;
        BaseTextureComponent(BaseTextureComponent&& other) = delete;
        BaseTextureComponent& operator=(const BaseTextureComponent& other) = delete;
        BaseTextureComponent& operator=(BaseTextureComponent&& other) = delete;

    protected:


        int m_Layer = 0; // Default layer
    };


}