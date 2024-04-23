#include "BombComponent.h"
#include "WorldTimeManager.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "ColliderComponent.h"
#include "DamageComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SoundServiceLocator.h"

Jotar::BombComponent::BombComponent(GameObject* owner, float explodeTime)
	: Component(owner)
	, m_ExplodeTime{ explodeTime }
	, m_IsExploded { false }
	, m_TimeCounter { 0 }
    , m_Range{}
{


}

void Jotar::BombComponent::Update()
{
	if (!m_IsExploded)
	{
		m_TimeCounter += WorldTimeManager::GetInstance().GetDeltaTime();

		if (m_TimeCounter >= m_ExplodeTime)
		{
			m_IsExploded = true;
			m_TimeCounter = 0;

            //todo refactor magic number
			OnExplode(3);
            GetOwner()->Destroy();
		}
	}
}

void Jotar::BombComponent::OnNotify(const CollisionEvent& triggerEvent)
{

    if (triggerEvent.GetOtherCollider()->GetOwner()->GetName() == "Explosion")
    {
        //todo refactor magic number
        OnExplode(3);
        GetOwner()->Destroy();
    }
}

void Jotar::BombComponent::OnExplode(int range)
{
    SoundServiceLocator::GetSoundSystem().Play(1);

    auto pos = GetOwner()->GetTransform()->GetLocalPosition();
    auto& centerCell = WorldGrid::GetInstance().GetGridCellByPosition(pos);


    const std::vector<std::pair<int, int>> directions = {
        {0, -1}, // Up
        {0, 1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };

    auto& worldGrid = WorldGrid::GetInstance();
    auto gridSize = worldGrid.GetGridSize();
    //int cellSize = worldGrid.GetCellSize();
    auto centerIndex = centerCell.Index;


    auto& scene = SceneManager::GetInstance().GetScene(0);

    CreateChildExplosion(0, centerCell.CenterCellPosition, scene);


    for (const auto& direction : directions)
    {
        int dx = direction.first;
        int dy = direction.second;

        for (int i = 1; i <= range; ++i)
        {
            int x = centerIndex.x + (i * dx);
            int y = centerIndex.y + (i * dy);

            if (x >= 0 && x < gridSize.x &&
                y >= 0 && y < gridSize.y)
            {

                auto& cell = worldGrid.GetGridCellByID({ x,y });

                if (cell.HasWall == GridCell::WallType::Undestroyable)
                    break;
                else if (cell.HasWall == GridCell::WallType::Destroyable)
                {
                    auto colllider = scene.GetCollisionManager().GetOverlappingColliderInPosition(cell.CenterCellPosition);
                    if (colllider == nullptr) break;

                    cell.HasWall = GridCell::WallType::None;
                    colllider->GetOwner()->Destroy();

                    break;
                }

                int animationRow = CalculateSpriteSheetRow(dx, dy, range, i);
                //Position
                //int distanceX = x - centerIndex.x;
                //int distanceY = y - centerIndex.y;
                //0 = middle, 1 = Vertical, 2 = horizontal, 3 = endUp, 4 = EndDown, 5 = EndRight, 6 = EndLeft
                CreateChildExplosion(animationRow, cell.CenterCellPosition, scene);

            }
            else
            {
                break;
            }
        }
    }

}

void Jotar::BombComponent::CreateChildExplosion(int explosionPosition,const glm::vec2& pos, Scene& scene)
{
    auto explosion = scene.CreateGameObject("Explosion");

    auto texture = explosion->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedSpriteSheet("Explosion"), explosionPosition);
    texture->SetDestroyOnLastFrame(true);
    explosion->GetTransform()->SetPosition(pos);
    auto triggerCollider = explosion->AddComponent<ColliderComponent>(false, true);

    int damage = 1;
    auto damageComp = explosion->AddComponent<DamageComponent>(damage);

    triggerCollider->AddObserver(damageComp);
}

int Jotar::BombComponent::CalculateSpriteSheetRow(int xDir, int yDir, int range, int currentRange)
{
    int animationRow = -1;

    if (currentRange == range)
    {
        if (xDir == 0 && yDir == -1) // Up
            animationRow = 3;
        else if (xDir == 0 && yDir == 1) // Down
            animationRow = 4;
        else if (xDir == -1 && yDir == 0) // Left
            animationRow = 6;
        else if (xDir == 1 && yDir == 0) // Right
            animationRow = 5;
    }
    else
    {
        // if vertical 
        if (xDir == 0)
            animationRow = 1;
        else
            animationRow = 2;
    }

    return animationRow;
}



