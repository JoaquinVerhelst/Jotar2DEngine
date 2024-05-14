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
#include "GameManager.h"
#include "MovementComponent.h"
#include "AIBehaviorComponent.h"
#include "AIAnimationControllerComponent.h"
#include "AIPerceptionComponent.h"
#include "AIEvents.h"
#include "HealthComponent.h"

#include "HUDComponent.h"

#include "MenuComponent.h"
#include "AIScoreComponent.h"
#include "DamageComponent.h"

using json = nlohmann::json;


bool Jotar::JsonLevelLoader::LoadLevelFromJson(Scene& scene, int level)
{
    std::ifstream file(m_GameLevelsFilePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open JSON file: " << m_GameLevelsFilePath << std::endl;
        return false;
    }
    try
    {
        json jsonData;
        file >> jsonData;


        std::string levelID = "level" + std::to_string(level);

        const auto& levelData = jsonData["gameScenes"][levelID];

        const auto& levelLayout = levelData["levelLayout"];
        const auto& gridSize = levelLayout["gridSize"];

        const auto& gridDimensions = levelLayout["gridDimensions"];
        int gridRows = gridDimensions[0];
        int gridColumns = gridDimensions[1];

        const auto& layout = levelLayout["layout"];

        GameManager::GetInstance().ResetAndInitializeWorldGrid(gridRows, gridColumns, gridSize);
        auto* worldGrid = GameManager::GetInstance().GetWorldGrid();


        std::vector<glm::ivec2> spawnCells{};

        for (size_t i = 0; i < layout.size(); ++i)
        {
            const std::string& row = layout[i];

            for (size_t j = 0; j < row.size(); ++j)
            {
                char tile = row[j];
                if (tile == '1')
                {
                    auto wall = CreateUnbreakableWall(scene);
                    auto& cell = worldGrid->GetGridCellByID({ j, i });
                    cell.ObjectOnCell = wall;
                    wall->GetTransform()->SetPosition(cell.CenterCellPosition);
                }
                else if (tile == '2')
                {
                    auto wall = CreateBreakableWall(scene);
                    auto& cell = worldGrid->GetGridCellByID({ j, i });
                    cell.ObjectOnCell = wall;
                    wall->GetTransform()->SetPosition(cell.CenterCellPosition);
                }
                else if (tile == '3')
                {
                    spawnCells.emplace_back(glm::ivec2{j, i});
                }
            }
        }

        const auto& randomizeBreakableWalls = levelLayout["randomizeBreakableWalls"];
        const auto& isRandomized = randomizeBreakableWalls["isRandomized"];

        if (isRandomized)
        {
            const auto& amount = levelLayout["randomizeBreakableWalls"]["amount"];
            RandomizeBreakableWalls(gridRows, gridColumns, scene, amount, spawnCells);
        }

        CreateEnemies(scene);



        return true;
    }
    catch (const json::exception& ex)
    {
        std::cout << "Failed to parse JSON file: " << m_GameLevelsFilePath << std::endl;
        std::cout << "Error: " << ex.what() << std::endl;
        return false;
    }

}

bool Jotar::JsonLevelLoader::LoadMenuFromJson(Scene& scene)
{
    auto bombermanFont = Jotar::ResourceManager::GetInstance().LoadFont("nes-field-combat.otf", 30);


    //logo
    auto title = scene.CreateGameObject("Background", false ); 
    title->GetTransform()->SetSize(glm::vec2{ 800, 350 });
    title->AddComponent<TextureComponent>("../Data/Sprites/Backgrounds/MenuBackground.png");
    title->AddComponent<HUDComponent>(HUDPosition::CenterUp, glm::vec2 { 0.f, 350.f / 2 });





    auto menuObj = scene.CreateGameObject("Menu", false);
    //menuObj->GetTransform()->SetPosition(540.f, 360.f);
    auto menuComp = menuObj->AddComponent<MenuComponent>();
    menuObj->AddComponent<HUDComponent>(HUDPosition::Center);
    menuComp->AddButton("SinglePlayer", std::bind(&GameManager::StartAndSetGameMode, &GameManager::GetInstance(), GameMode::SinglePlayer), {0, 0, 25, 150}, bombermanFont);
    menuComp->AddButton("Coop", std::bind(&GameManager::StartAndSetGameMode, &GameManager::GetInstance(), GameMode::Coop), { 0, 0, 25, 150 }, bombermanFont);
    menuComp->AddButton("Versus", std::bind(&GameManager::StartAndSetGameMode, &GameManager::GetInstance(), GameMode::Versus), { 0, 0, 25, 150 }, bombermanFont);
    menuComp->AddButton("HighScore", nullptr, { 0, 0, 25, 150 }, bombermanFont);


    return false;
}

void Jotar::JsonLevelLoader::SetGameLevelsFilePath(std::string filePath)
{
    m_GameLevelsFilePath = filePath;
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

void Jotar::JsonLevelLoader::CreateEnemies(Scene& scene)
{
    std::vector <std::string> enemyTarget = { "Player" };
    AnimationIndexesInfo animationInfo{ {3,5},{0, 2},{6, 6},{7, 10} };

    for (float i = 0; i < 3; i++)
    {
        auto enemy = scene.CreateGameObject("enemy");
        enemy->AddComponent<TextureComponent>("../Data/Sprites/Enemy/BalloomSpriteSheet.png", false, 1, 11 );
        enemy->AddComponent<MovementComponent>(60.f);
        auto behavior = enemy->AddComponent<AIBehaviorComponent>();


        auto perception = enemy->AddComponent< AIPerceptionComponent>(200.f, enemyTarget);
        perception->AddObserver(behavior);
        enemy->AddComponent<AIAnimationControllerComponent>(animationInfo);
        auto healthComp = enemy->AddComponent<HealthComponent>(1);
        healthComp->AddObserver(behavior);
        auto collEnemy = enemy->AddComponent<ColliderComponent>(false, true);
        collEnemy->SetTag("Enemy");
        collEnemy->AddIgnoreCollisionTag("Enemy");

        auto damageComp = enemy->AddComponent<DamageComponent>(1, enemyTarget);
        collEnemy->AddObserver(damageComp);


        auto scoreComp = enemy->AddComponent<AIScoreComponent>(100);
        behavior->AddObserver(scoreComp);


        enemy->GetTransform()->SetPosition(64.f * (i * 3 + 2.f), 100.f);
    }
}


void Jotar::JsonLevelLoader::RandomizeBreakableWalls(int rows, int columns, Scene& scene, int amount, std::vector<glm::ivec2>& spawnCells)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribX(0, rows - 1); // Random distribution for X coordinate
    std::uniform_int_distribution<int> distribY(0, columns - 1);    // Random distribution for Y coordinate

    auto* worldGrid = GameManager::GetInstance().GetWorldGrid();

    int wallsPlaced = 0;
 

    while (wallsPlaced < amount)
    {
        int randomX, randomY;
        glm::ivec2 cellIndex;

        do
        {
            randomX = distribX(gen);
            randomY = distribY(gen);
            cellIndex = { randomX, randomY };
        } while (std::find(spawnCells.begin(), spawnCells.end(), cellIndex) != spawnCells.end());


        auto& cell = worldGrid->GetGridCellByID(cellIndex);

        if (cell.ObjectOnCell.expired())
        {
            auto wall = CreateBreakableWall(scene);
            cell.ObjectOnCell = wall;
            wall->GetTransform()->SetPosition(cell.CenterCellPosition);
            ++wallsPlaced;
        }
    }
}