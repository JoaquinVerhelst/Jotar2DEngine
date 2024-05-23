#include "JsonLevelLoader.h"

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

#include "CameraComponent.h"
#include "MenuComponent.h"
#include "AIScoreComponent.h"
#include "DamageComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "HealthDisplayComponent.h"
#include "ScoreDisplayComponent.h"

#include "ScoreComponent.h"
#include "PlaceBombComponent.h"

#include "InputManager.h"
#include "Command.h"
#include "MovementCommand.h"
#include "PlaceBombCommand.h"
#include "Renderer.h"
#include "PickUpComponent.h"
#include "GameCommands.h"
#include "ExitComponent.h"
#include "DeathCheckerComponent.h"
#include "PlayerDeathComponent.h"
#include "HighScoreMenuComponent.h"
#include "HighScoreEntryComponent.h"
#include "JsonHighScoreLoaderComponent.h"


#include "Font.h"

using json = nlohmann::json;


bool Jotar::JsonLevelLoader::InitGame()
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


        //Make The Scenes

        Jotar::SceneManager::GetInstance().CreateScene("mainMenu");

       // Jotar::SceneManager::GetInstance().CreateScene("transition");


        //const auto& levels = jsonData["gameInfo"]["gameScenes"]["levels"];
        //for (json::const_iterator it = levels.begin(); it != levels.end(); ++it)
        //{
        //    Jotar::SceneManager::GetInstance().CreateScene(it.key());
        //}

        Jotar::SceneManager::GetInstance().CreateScene("highScoreMenu");

        //Load The Shared Textures
        const auto& sharedTexturesInfo = jsonData["gameInfo"]["sharedTextures"];
        for (json::const_iterator it = sharedTexturesInfo.begin(); it != sharedTexturesInfo.end(); ++it)
        {
            const auto& textureInfo = it.value();
            ResourceManager::GetInstance().AddSharedTexture(textureInfo["filePath"], textureInfo["name"]);
        }



        // Load The Shared Sprite Sheet
        const auto& sharedSpriteSheetInfo = jsonData["gameInfo"]["sharedSpriteSheet"];
        for (json::const_iterator it = sharedSpriteSheetInfo.begin(); it != sharedSpriteSheetInfo.end(); ++it)
        {
            const auto& spriteSheetInfo = it.value();
            const auto& spriteSheet = spriteSheetInfo["spriteSheet"];

            SpriteSheet explosionSheet{};
            explosionSheet.TotalColumns = spriteSheet["columns"];
            explosionSheet.TotalRows = spriteSheet["rows"];
            explosionSheet.Direction = spriteSheet["isDirectionDown"] ? SpriteSheet::SpriteSheetDirection::Down : SpriteSheet::SpriteSheetDirection::Right;

            ResourceManager::GetInstance().AddSharedSpriteSheet(spriteSheetInfo["filePath"], explosionSheet, spriteSheetInfo["name"]);
        }


        ///  Init Genral Input 

        const auto& generalInput = jsonData["gameInfo"]["generalInput"];

        auto & input = InputManager::GetInstance();

        input.AddKeyBinding(KeyboardKey{ generalInput["skipLevel"] , InputType::Up }, std::make_unique<SkipLevelCommand>());
        input.AddKeyBinding(KeyboardKey{ generalInput["mute"] , InputType::Up }, std::make_unique<MuteSoundCommand>());

        input.AddKeyBinding(KeyboardKey{ generalInput["increaseMusicVolume"] , InputType::Up }, std::make_unique<ChangeMusicVolume>(5));
        input.AddKeyBinding(KeyboardKey{ generalInput["decreaseMusicVolume"] , InputType::Up }, std::make_unique<ChangeMusicVolume>(-5));

        input.AddKeyBinding(KeyboardKey{ generalInput["increaseSoundEffectsVolume"] , InputType::Up }, std::make_unique<ChangeSoundEffectVolume>(5));
        input.AddKeyBinding(KeyboardKey{ generalInput["decreaseSoundEffectsVolume"] , InputType::Up }, std::make_unique<ChangeSoundEffectVolume>(-5));

        return true;
    }
    catch (const json::exception& ex)
    {
        std::cout << "Failed to parse JSON file: " << m_GameLevelsFilePath << std::endl;
        std::cout << "Error: " << ex.what() << std::endl;
        return false;
    }

    return false;
}

bool Jotar::JsonLevelLoader::LoadLevelFromJson(Scene& scene, int level, bool isGameModeInit)
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

        //InitializeLevel

        std::string levelID = "level" + std::to_string(level);

        const auto& levelData = jsonData["gameInfo"]["gameScenes"]["levels"][levelID];

        const auto& levelLayout = levelData["levelLayout"];
        const auto& cellSize = levelLayout["cellSize"];

        const auto& gridDimensions = levelLayout["gridDimensions"];
        int gridRows = gridDimensions[0];
        int gridColumns = gridDimensions[1];

        const auto& layout = levelLayout["layout"];


        GameManager::GetInstance().ResetAndInitializeWorldGrid(gridRows, gridColumns, cellSize);
        auto* worldGrid = GameManager::GetInstance().GetWorldGrid();



        int windowWidth, windowHeight;
        SDL_GetWindowSize(Renderer::GetInstance().GetGlSDLManager()->GetWindow(), &windowWidth, &windowHeight);

        const auto& UIInfo = jsonData["gameInfo"]["UI"];
        float screenHeightPercent = UIInfo["heightPercent"];



        GeneralLevelInfo levelInfo;
        levelInfo.CellSize = cellSize;
        levelInfo.Rows = gridRows;
        levelInfo.Columns = gridColumns;
        levelInfo.ScreenHeight = windowHeight;
        levelInfo.ScreenWidth = windowWidth;
        levelInfo.UIPercent = screenHeightPercent;



        if (!isGameModeInit)
            CreateGameMode(scene, levelInfo);
        //else
        //    UpdateCamera(scene, levelInfo);


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


        // Set the players on the right position

        auto players = GameManager::GetInstance().GetPlayers();
        for (size_t i = 0; i < players.size(); i++)
        {
            const std::string playerPosIndexString = "playerPosIndex" + std::to_string(i);

            glm::ivec2 playerPosIndex = { levelData[playerPosIndexString][0], levelData[playerPosIndexString][1] };

            const auto& cell2 = worldGrid->GetGridCellByID(playerPosIndex);
            players[i]->SetPosition(cell2.CenterCellPosition);
        }


        // Exit


        auto exit = scene.CreateGameObject("Exit Wall");
        auto collider = exit->AddComponent<ColliderComponent>(true);
        collider->SetTag("Destroyable");
        auto exitComp = exit->AddComponent<ExitComponent>(jsonData["gameInfo"]["otherTextures"]["exitTextureFilePath"]);

        collider->AddObserver(exitComp);

        exit->AddComponent<BreakableWallComponent>(
            [exitComp]() {
                exitComp->RevealExit();
            }
        );

        exit->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("BreakableWall"));

        PlaceGameObjectRandomly(exit, levelInfo, 8, true);



        // DeathCkecker


        auto deathCkeckerObj = scene.CreateGameObject("Death Ckecker");
        auto deatchCheckerComp = deathCkeckerObj->AddComponent<DeathCheckerComponent>();


        for (auto& player : players)
        {
            player->GetOwner()->GetComponent<PlayerDeathComponent>()->AddObserver(deatchCheckerComp);
        }



        //Camera

        glm::ivec4 camRect = { 0, 0, levelInfo.ScreenHeight, levelInfo.ScreenWidth };
        glm::ivec4 levelBounds = { 0, levelInfo.ScreenHeight * levelInfo.UIPercent , 1000, levelInfo.Rows * levelInfo.CellSize };

        auto cameraObj = scene.CreateGameObject("Camera");
        auto camera = cameraObj->AddComponent<CameraComponent>(camRect, levelBounds);
        scene.SetCamera(camera);
        camera->SetTargets(players);


        // Enemies
        int totalEnemies = CreateEnemies(scene, levelData["enemyInfo"], levelInfo, exitComp);
        exitComp->SetAmountOfEnemies(totalEnemies);

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
    title->AddComponent<HUDComponent>(HUDPosition::CenterUp);





    auto menuObj = scene.CreateGameObject("Menu", false);
    //menuObj->GetTransform()->SetPosition(540.f, 360.f);
    auto menuComp = menuObj->AddComponent<MenuComponent>();
    menuObj->AddComponent<HUDComponent>(HUDPosition::Center);
    menuComp->AddButton("SinglePlayer", std::bind(&GameManager::StartAndSetGameMode, &GameManager::GetInstance(), GameMode::SinglePlayer), {0, 0, 25, 150}, bombermanFont);
    menuComp->AddButton("Coop", std::bind(&GameManager::StartAndSetGameMode, &GameManager::GetInstance(), GameMode::Coop), { 0, 0, 25, 150 }, bombermanFont);
    menuComp->AddButton("Versus", std::bind(&GameManager::StartAndSetGameMode, &GameManager::GetInstance(), GameMode::Versus), { 0, 0, 25, 150 }, bombermanFont);
    menuComp->AddButton("HighScore", std::bind(&GameManager::LoadHighScoreMenu, &GameManager::GetInstance(), false), { 0, 0, 25, 150 }, bombermanFont);


    return true;
}


bool Jotar::JsonLevelLoader::LoadHighScoreFromJson(Scene& scene, bool isSavingScore)
{
    auto titleFont = Jotar::ResourceManager::GetInstance().LoadFont("nes-field-combat.otf", 50);
    auto gameModeFont = Jotar::ResourceManager::GetInstance().LoadFont("nes-field-combat.otf", 30);
    auto scoreFont = Jotar::ResourceManager::GetInstance().LoadFont("nes-field-combat.otf", 10);


    //HighScore Title
    auto highScoreTitle = scene.CreateGameObject("HighScoreTitle", false);
    auto textComp = highScoreTitle->AddComponent<TextComponent>("HighScore", titleFont);
    highScoreTitle->AddComponent<HUDComponent>(HUDPosition::CenterUp,glm::vec2{ -textComp->GetSize().x / 2, 0 });


    int windowWidth, windowHeight;
    SDL_GetWindowSize(Renderer::GetInstance().GetGlSDLManager()->GetWindow(), &windowWidth, &windowHeight);


    //SinglePlayer Title
    auto singlePlayerTitle = scene.CreateGameObject("SinglePlayerTitle", false);
    textComp = singlePlayerTitle->AddComponent<TextComponent>("SinglePlayer", gameModeFont);

    glm::vec2 offset{ static_cast<float>(-windowWidth / 4 - textComp->GetSize().x/2), windowHeight / 5};
    singlePlayerTitle->AddComponent<HUDComponent>(HUDPosition::CenterUp, offset);


    //Coop Title
    auto coopTitle = scene.CreateGameObject("CoopTitle", false);
    textComp = coopTitle->AddComponent<TextComponent>("Coop", gameModeFont);
    offset. x = static_cast<float>(windowWidth / 4) - static_cast<float>(textComp->GetSize().x / 2);
    coopTitle->AddComponent<HUDComponent>(HUDPosition::CenterUp, offset);



    // Setup HighScore Game Object
    auto menu = scene.CreateGameObject("HighScore Menu", false);
    menu->AddComponent<HighScoreMenuComponent>(scoreFont, glm::ivec2{windowWidth, windowHeight});
    menu->AddComponent<JsonHighScoreLoaderComponent>("../Data/Json/HighScore.json");
    menu->AddComponent<HUDComponent>(HUDPosition::Center);

    if (isSavingScore)
    {
        // make the HighScoreEntry
        auto entryObj = scene.CreateGameObject("HighScore Menu", false);
        entryObj->AddComponent<HighScoreEntryComponent>();
        entryObj->AddComponent<TextComponent>("", scoreFont);
    }

    return true;
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

    wall->AddComponent<BreakableWallComponent>(std::bind(&JsonLevelLoader::CreatePickUpObject, &GameManager::GetInstance().GetLevelLoader(), wall.get()));
    wall->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("BreakableWall"));
    return wall;
}

void Jotar::JsonLevelLoader::UpdateCamera(Scene& scene, GeneralLevelInfo& levelInfo)
{
    //update camera levelbounds
    auto cam = scene.GetObjectByName("Camera");
    if (cam != nullptr)
    {
        scene.SetCamera(cam->GetComponent<CameraComponent>());

        glm::ivec4 levelBounds = { 0, levelInfo.ScreenHeight * levelInfo.UIPercent , 1000, levelInfo.Rows * levelInfo.CellSize };
        scene.GetCamera()->SetLevelBounds(levelBounds);
    }

}

bool Jotar::JsonLevelLoader::CreateGameMode(Scene& scene, GeneralLevelInfo& levelInfo)
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


        auto& gameManager = GameManager::GetInstance();





        const auto& gameInfo = jsonData["gameInfo"];
        const auto& fonts = gameInfo["fonts"];

        //Fonts
        const auto& fpsFontInfo = fonts["fpsFont"];
        const auto& bombermanFontInfo = fonts["bombermanFont"];
        auto fpsFont = Jotar::ResourceManager::GetInstance().LoadFont(fpsFontInfo["fontPath"], fpsFontInfo["size"]);
        auto bombermanFont = Jotar::ResourceManager::GetInstance().LoadFont(bombermanFontInfo["fontPath"], bombermanFontInfo["size"]);


        const auto& UIInfo = gameInfo["UI"];

        //HUD
        auto HUD = scene.CreateGameObject("HUD", false);
        HUD->SetDestroyOnLoad(false);
        HUD->GetTransform()->SetSize({ levelInfo.ScreenWidth, levelInfo.ScreenHeight * levelInfo.UIPercent });
        auto textureComp = HUD->AddComponent<TextureComponent>(UIInfo["UIBackground"]);
        textureComp->SetLayer(10);
        HUD->AddComponent<HUDComponent>(HUDPosition::CenterUp);

        ////FPS
        auto fpsCounter = HUD->CreateChildGameObject("FPScounter", false, false);

        auto textComp = fpsCounter->AddComponent<TextComponent>("FPS", fpsFont,
            SDL_Color(fpsFontInfo["color"][0], fpsFontInfo["color"][1], fpsFontInfo["color"][2], fpsFontInfo["color"][3]));
        textComp->SetLayer(10);

        fpsCounter->AddComponent<HUDComponent>(HUDPosition::LeftUp);
        fpsCounter->AddComponent<Jotar::FPSComponent>();





        // Time Left
        auto TimeLeftObj = HUD->CreateChildGameObject("TimeLeft", true, false);
        textComp = TimeLeftObj->AddComponent<TextComponent>("TimeLeft: ", bombermanFont);
        textComp->SetLayer(10);
        TimeLeftObj->AddComponent<HUDComponent>(HUDPosition::CenterLeft);


        // Player 0
        auto player0Obj = CreatePlayer(scene, gameInfo, HUD, bombermanFont, levelInfo.CellSize, 0);
        player0Obj->SetDestroyOnLoad(false);
        gameManager.AddPlayer(player0Obj->GetTransform());


        // gameMode
        if (gameManager.GetGamemode() == GameMode::Coop)
        {
            // player 1
            auto player1Obj = CreatePlayer(scene, gameInfo, HUD, bombermanFont, levelInfo.CellSize, 1);
            player1Obj->SetDestroyOnLoad(false);

            gameManager.AddPlayer(player1Obj->GetTransform());
        }
        else if (gameManager.GetGamemode() == GameMode::Versus)
        {

        }

        return true;
    }
    catch (const json::exception& ex)
    {
        std::cout << "Failed to parse JSON file: " << m_GameLevelsFilePath << std::endl;
        std::cout << "Error: " << ex.what() << std::endl;
        return false;
    }

    return false;
}

std::shared_ptr<Jotar::GameObject> Jotar::JsonLevelLoader::CreatePlayer(Scene& scene, const nlohmann::json& gameInfo, std::shared_ptr<GameObject> HUD, std::shared_ptr<Font> font, int cellSize, unsigned int playerIndex)
{
    const auto& playerInfo = gameInfo["playerInfo"];
    const auto& UIInfo = gameInfo["UI"];

    // Player Text

    std::string playerString = "player" + std::to_string(playerIndex);

    auto playerTextObj = HUD->CreateChildGameObject("PlayerText", true, false);
    auto textComp = playerTextObj->AddComponent<TextComponent>(playerString + " : ", font);
    textComp->SetLayer(11);
    std::string playerbarPos = "playerInfoBarOffset" + std::to_string(playerIndex);
    glm::vec2 textPos = { UIInfo[playerbarPos][0],  UIInfo[playerbarPos][1] };

    playerTextObj->AddComponent<HUDComponent>(HUDPosition::CenterUp, textPos);

    // Player Lives Display

    auto playerHealthDisplayObj = playerTextObj->CreateChildGameObject("PlayerHealthDisplay" + std::to_string(playerIndex), false, false);
    textComp = playerHealthDisplayObj->AddComponent<TextComponent>("Lives: ", font);
    textComp->SetLayer(11);
    auto playerHealthDisplay = playerHealthDisplayObj->AddComponent<HealthDisplayComponent>();

    int xPos = playerTextObj->GetTransform()->GetSize().x + UIInfo["spacing"];

    playerHealthDisplayObj->GetTransform()->Translate({ xPos, 0 });

    // Player Score Display

    auto playerScoreDisplayObj = playerTextObj->CreateChildGameObject("PlayerScoreDisplay" + std::to_string(playerIndex), false, false);
    textComp = playerScoreDisplayObj->AddComponent<TextComponent>("Score: ", font);
    textComp->SetLayer(11);
    auto playerScoreDisplay = playerScoreDisplayObj->AddComponent<ScoreDisplayComponent>();

    xPos += playerHealthDisplayObj->GetTransform()->GetSize().x + UIInfo["spacing"];

    playerScoreDisplayObj->GetTransform()->Translate({ xPos, 0 });

    // Player OBJ

    auto playerObj = scene.CreateGameObject("Bomberman" + std::to_string(playerIndex));

    playerObj->GetTransform()->SetSize({ 60,60 });

    auto textureComp = playerObj->AddComponent<TextureComponent>(playerInfo["sprites"]["playerSprite"]);
    textureComp->SetLayer(10);


    auto movementCompPlayer = playerObj->AddComponent<MovementComponent>(playerInfo["speed"], cellSize);

    auto healthCompPlayer = playerObj->AddComponent<HealthComponent>(playerInfo["health"]);
    healthCompPlayer->AddObserver(playerHealthDisplay);

    auto scoreCompPlayer = playerObj->AddComponent<ScoreComponent>();
    scoreCompPlayer->AddObserver(playerScoreDisplay);


    auto deathComp = playerObj->AddComponent<PlayerDeathComponent>(2.f);
    healthCompPlayer->AddObserver(deathComp);


    auto placeBombComp = playerObj->AddComponent<PlaceBombComponent>();
    auto colliderComp = playerObj->AddComponent<ColliderComponent>(false);
    colliderComp->SetTag("Player");
    colliderComp->AddIgnoreCollisionTag("Enemy");

    auto& input = InputManager::GetInstance();

    input.AddControllerBinding(ControllerKey{ playerIndex, Jotar::ControllerButton::DPadUp }, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{ 0, -1 }));
    input.AddControllerBinding(ControllerKey{ playerIndex, Jotar::ControllerButton::DPadDown }, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{ 0, 1 }));
    input.AddControllerBinding(ControllerKey{ playerIndex, Jotar::ControllerButton::DPadRight }, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{ 1, 0 }));
    input.AddControllerBinding(ControllerKey{ playerIndex, Jotar::ControllerButton::DPadLeft }, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{ -1, 0 }));
    input.AddControllerBinding(ControllerKey{ playerIndex, Jotar::ControllerButton::ButtonB , InputType::Up }, std::make_unique<PlaceBombCommand>(placeBombComp));


    const auto& keyboardKeys = playerInfo["keyboardKeys"][playerString];

    input.AddKeyBinding(KeyboardKey{ keyboardKeys["forward"]}, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{0, -1}));
    input.AddKeyBinding(KeyboardKey{ keyboardKeys["backward"]}, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{0, 1}));
    input.AddKeyBinding(KeyboardKey{ keyboardKeys["right"] }, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{ 1, 0 }));
    input.AddKeyBinding(KeyboardKey{ keyboardKeys["left"]}, std::make_unique<MovementCommand>(movementCompPlayer, glm::vec2{-1, 0}));
    input.AddKeyBinding(KeyboardKey{ keyboardKeys["placeBomb"] , InputType::Up}, std::make_unique<PlaceBombCommand>(placeBombComp));


    return playerObj;
}

void Jotar::JsonLevelLoader::CreateBalloomPlayer(Scene& )
{
}

int Jotar::JsonLevelLoader::CreateEnemies(Scene& scene, const nlohmann::json& EnemyInfo, GeneralLevelInfo& levelInfo, ExitComponent* exitComp)
{

    std::vector <std::string> enemyTarget = { "Player" };
    AnimationIndexesInfo animationInfo{ {3,5},{0, 2},{6, 6},{7, 10} };
    std::string enemyTag = "Enemy";
    int totalEnemies{};

    for (const auto& enemyTypeInfo : EnemyInfo.items())
    {
        const auto& enemyData = enemyTypeInfo.value();
        const auto& amount = enemyData["amount"];

        for (size_t j = 0; j < amount; ++j)
        {
            auto enemy = scene.CreateGameObject(enemyTag);

            enemy->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedSpriteSheet(enemyData["spriteSheetName"]), 0, 0);
            enemy->AddComponent<MovementComponent>(enemyData["speed"], levelInfo.CellSize);
            auto behavior = enemy->AddComponent<AIBehaviorComponent>();

            if (enemyData["intelligence"]["level"] == 1)
            {
                auto perception = enemy->AddComponent<AIPerceptionComponent>(enemyData["intelligence"]["viewDistance"], enemyTarget);
                perception->AddObserver(behavior);
            }
 
            enemy->AddComponent<AIAnimationControllerComponent>(animationInfo);
            auto healthComp = enemy->AddComponent<HealthComponent>(1);
            healthComp->AddObserver(behavior);

            auto collEnemy = enemy->AddComponent<ColliderComponent>(false, false);
            collEnemy->SetTag(enemyTag);
            collEnemy->AddIgnoreCollisionTag(enemyTag);
            collEnemy->AddIgnoreCollisionTag("Player");

            auto scoreComp = enemy->AddComponent<AIScoreComponent>(enemyData["points"]);
            behavior->AddObserver(scoreComp);


            auto damageCollObj = enemy->CreateChildGameObject("EnemyTriggerCollider", false, false);
            auto damageComp = damageCollObj->AddComponent<DamageComponent>(1, enemyTarget);
            auto damageCollComp = damageCollObj->AddComponent<ColliderComponent>(false, true);
            damageCollComp->SetSize({ 50.f, 50.f });
            damageCollComp->AddObserver(damageComp);


            behavior->AddObserver(exitComp);



            PlaceGameObjectRandomly(enemy, levelInfo);

            ++totalEnemies;
        }
    }

    return totalEnemies;
}

void Jotar::JsonLevelLoader::CreatePickUpObject(GameObject* owner)
{
    auto& scene = SceneManager::GetInstance().GetCurrentScene();
    auto pickupObj = scene.CreateGameObject("PickUp");
    auto pickupComp = pickupObj->AddComponent<PickUpComponent>();
    auto collider = pickupObj->AddComponent<ColliderComponent>(true, true);

    collider->AddObserver(pickupComp);

    auto& cell = GameManager::GetInstance().GetWorldGrid()->GetGridCellByPosition(owner->GetTransform()->GetLocalPosition());

    pickupObj->GetTransform()->SetPosition(cell.CenterCellPosition);
    collider->UpdatePosition();
    pickupObj->Start();
}

void Jotar::JsonLevelLoader::PlaceGameObjectRandomly(std::shared_ptr<GameObject> gameObject, GeneralLevelInfo& levelInfo, int minRangeAwayFromSpawn, bool isStaticPlacedOnCell)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribX(0, levelInfo.Rows - 1); // Random distribution for X coordinate
    std::uniform_int_distribution<int> distribY(0, levelInfo.Columns - 1);    // Random distribution for Y coordinate

    auto* worldGrid = GameManager::GetInstance().GetWorldGrid();
    bool isPlaced = false;

    while (!isPlaced)
    {
        int randomX, randomY;
        glm::ivec2 cellIndex;

        do
        {
            randomX = distribX(gen);
            randomY = distribY(gen);
        } while (randomX < minRangeAwayFromSpawn && randomY < minRangeAwayFromSpawn);

        cellIndex = { randomX, randomY };

        auto& cell = worldGrid->GetGridCellByID(cellIndex);

        if (cell.ObjectOnCell.expired())
        {
            gameObject->GetTransform()->SetPosition(cell.CenterCellPosition);

            if (isStaticPlacedOnCell)
            {
                cell.ObjectOnCell = gameObject;
            }

            isPlaced = true;
        }
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

