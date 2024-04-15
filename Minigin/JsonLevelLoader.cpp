#include "JsonLevelLoader.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include "WorldGrid.h"
#include "GameObject.h"
#include "Scene.h"

#include "TextureComponent.h"


using json = nlohmann::json;


bool Jotar::JsonLevelLoader::LoadLevelsFromJson(Scene& scene, const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open JSON file: " << filePath << std::endl;
        return false;
    }
    try
    {
        json jsonData;
        file >> jsonData;



        const auto& level = jsonData["level"];

        const auto& levelLayout = level["levelLayout"];
        const auto& gridSize = levelLayout["gridSize"];

        const auto& gridDimensions = levelLayout["gridDimensions"];
        int gridRows = gridDimensions[0];
        int gridColumns = gridDimensions[1];

        const auto& layout = levelLayout["layout"];

        auto& worldGrid = WorldGrid::GetInstance();
        worldGrid.Init(gridRows, gridColumns, gridSize);


        // Iterate over the layout array
        for (size_t i = 0; i < layout.size(); ++i)
        {
            const std::string& row = layout[i];

            // Iterate over the characters in the row
            for (size_t j = 0; j < row.size(); ++j)
            {
                char tile = row[j];
                if (tile == '1')
                {
                    auto wall = CreateUnbreakableWall(scene);
                    auto& cell = worldGrid.GetGridCellByID({ static_cast<int>(j), static_cast<int>(i) });
                    wall->GetTransform()->SetPosition(cell.CenterCellPosition);
                }
            }
        }


        return true;
    }
    catch (const json::exception& ex)
    {
        std::cout << "Failed to parse JSON file: " << filePath << std::endl;
        std::cout << "Error: " << ex.what() << std::endl;
        return false;
    }

}

std::shared_ptr<Jotar::GameObject> Jotar::JsonLevelLoader::CreateUnbreakableWall(Scene& scene)
{
    auto wall = scene.CreateGameObject("Wall");
    wall->AddComponent<ColliderComponent>(true);
    wall->AddComponent<TextureComponent>("../Data/Sprites/UnbreakableWall.png");
    return wall;
}

