#pragma once
#include <memory>
#include "Texture2D.h"

namespace Jotar
{


    struct SpriteSheet
    {

        enum class SpriteSheetDirection
        {
            Right,
            Down
        };

        SpriteSheetDirection Direction = SpriteSheetDirection::Right;

        //Reason for Shared_ptr: To be able to use shared texture resources  
        std::shared_ptr<Texture2D> m_pTexture{};

        std::string Name = "";
        int ClipHeight = 0;
        int ClipWidth = 0;

        int TotalColumns = 0;
        int TotalRows = 0;

        int CurrentColumn = 0;
        int CurrentRow = 0;

    };








}