#include "JsonLevelLoader.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include "WorldGrid.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BreakableWallComponent.h"
#include "TextureComponent.h"
#include <random>

using json = nlohmann::json;


Jotar::JsonLevelLoader::JsonLevelLoader(GameObject* owner, Scene& scene, const std::string& filePath)
    : Component(owner)
{
    LoadLevelFromJson(scene, filePath);
}


bool Jotar::JsonLevelLoader::LoadLevelFromJson(Scene& scene, const std::string& filePath)
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
                    cell.ObjectOnCell = wall;
                    wall->GetTransform()->SetPosition(cell.CenterCellPosition);
                }
                if (tile == '2')
                {
                    auto wall = CreateBreakableWall(scene);
                    auto& cell = worldGrid.GetGridCellByID({ static_cast<int>(j), static_cast<int>(i) });
                    cell.ObjectOnCell = wall;
                    wall->GetTransform()->SetPosition(cell.CenterCellPosition);
                }
            }
        }

        const auto& randomizeBreakableWalls = levelLayout["randomizeBreakableWalls"];
        const auto& isRandomized = randomizeBreakableWalls["isRandomized"];

        if (isRandomized)
        {
            const auto& amount = levelLayout["randomizeBreakableWalls"]["amount"];
            RandomizeBreakableWalls(gridRows, gridColumns, scene, amount);
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
    auto collider = wall->AddComponent<ColliderComponent>(true);
    collider->SetTag("Undestroyable");
    wall->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("UnbreakableWall"));
    return wall;
}

std::shared_ptr<Jotar::GameObject> Jotar::JsonLevelLoader::CreateBreakableWall(Scene& scene)
{
    auto wall = scene.CreateGameObject("Breakable Wall");
    auto collider = wall->AddComponent<ColliderComponent>(true);
    collider->SetTag("Destroyable");
    wall->AddComponent<BreakableWallComponent>();
    wall->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("BreakableWall"));
    return wall;
}


void Jotar::JsonLevelLoader::RandomizeBreakableWalls(int rows, int columns, Scene& scene, int amount)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribX(0, rows - 1); // Random distribution for X coordinate
    std::uniform_int_distribution<int> distribY(0, columns - 1);    // Random distribution for Y coordinate

    auto& worldGrid = WorldGrid::GetInstance();

    int wallsPlaced = 0;
 
    while (wallsPlaced < amount)
    {
        int randomX = distribX(gen); // Generate random X coordinate
        int randomY = distribY(gen); // Generate random Y coordinate

        auto& cell = worldGrid.GetGridCellByID({ randomX, randomY });

        if (cell.ObjectOnCell.expired())
        {
            auto wall = CreateBreakableWall(scene);
            cell.ObjectOnCell = wall;
            wall->GetTransform()->SetPosition(cell.CenterCellPosition);
            ++wallsPlaced;
        }
    }
}