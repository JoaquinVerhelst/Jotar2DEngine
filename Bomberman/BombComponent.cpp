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
#include "ExplosionEvent.h"


#include <iostream>

Jotar::BombComponent::BombComponent(GameObject* owner, float explodeTime, int range)
	: Component(owner)
	, m_ExplodeTime{ explodeTime }
	, m_IsExploded { false }
	, m_TimeCounter { 0 }
    , m_Range{ range }
{
    m_pSubject = std::make_unique<Subject<ExplosionEvent>>();

}

void Jotar::BombComponent::Update()
{
	if (!m_IsExploded)
	{
		m_TimeCounter += WorldTimeManager::GetInstance().GetDeltaTime();

		if (m_TimeCounter >= m_ExplodeTime)
		{
			m_TimeCounter = 0;
			OnExplode(m_Range);
		}
	}
}

void Jotar::BombComponent::OnDestroy()
{
    m_pSubject->RemoveAllObservers();
}


void Jotar::BombComponent::OnNotify(const CollisionEvent& triggerEvent)
{
    if (m_IsExploded) return;


    if (typeid(triggerEvent) == typeid(TriggerEndEvent))
    {
        std::cout << "On Collsiion ENd" << '\n';

        if (triggerEvent.GetOtherCollider()->CompareTag("Player"))
        {
            GetOwner()->GetComponent<ColliderComponent>()->SetIsTrigger(false);
        }
    }
}

void Jotar::BombComponent::AddObserver(Observer<ExplosionEvent>* pObserver)
{
    m_pSubject->AddObserver(pObserver);
}

void Jotar::BombComponent::RemoveObserver(Observer<ExplosionEvent>* pObserver)
{
    m_pSubject->RemoveObserver(pObserver);
}

void Jotar::BombComponent::Explode()
{
    if (!m_IsExploded)
        OnExplode(m_Range);
}

void Jotar::BombComponent::OnExplode(int range)
{
    m_IsExploded = true;

    SoundServiceLocator::GetSoundSystem().Play(1, 100);
    ExplosionEvent explosionevent{};
    m_pSubject->NotifyObservers(explosionevent);


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

                if (auto objPtr = cell.ObjectOnCell.lock())
                {
                    auto collider = objPtr->GetComponent<ColliderComponent>();

                    if (collider == nullptr) return;

                    if (collider->CompareTag("Undestroyable"))
                        break;
                    else if (collider->CompareTag("Destroyable"))
                    {
                        objPtr->Destroy();
                        break;
                    }
                    else if (collider->CompareTag("Bomb"))
                    {
                        objPtr->GetComponent<BombComponent>()->Explode();
                    }
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

    //GetOwner()->GetComponent<ColliderComponent>()->RemoveThisColliderFromManager();
    GetOwner()->Destroy();
}


void Jotar::BombComponent::CreateChildExplosion(int explosionPosition,const glm::vec2& pos, Scene& scene)
{
    auto explosion = scene.CreateGameObject("Explosion");

    auto texture = explosion->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedSpriteSheet("Explosion"), explosionPosition);
    texture->SetDestroyOnLastFrame(true);
    explosion->GetTransform()->SetPosition(pos);
    auto triggerCollider = explosion->AddComponent<ColliderComponent>(false, true);
    triggerCollider->SetTag("Explosion");
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



