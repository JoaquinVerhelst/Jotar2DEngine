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
#include "BreakableWallComponent.h"

#include "GameManager.h"

Jotar::BombComponent::BombComponent(GameObject* owner, GameObject* bombPlacer, float explodeTime, int range, int explosionSpeed, const std::vector<std::string>& tagsToKill)
	: Component(owner)
	, m_ExplodeTime{ explodeTime }
	, m_IsExploded { false }
	, m_TimeCounter { 0 }
    , m_Range{ range }
    , m_BombPlacer{ bombPlacer }
    , m_ExplosionSpeed{ explosionSpeed }
    , m_TagsToKill{tagsToKill}
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
    if (!m_IsExploded && typeid(triggerEvent) == typeid(TriggerEndEvent))
        if (triggerEvent.GetOtherCollider()->CompareTag("Player"))
            GetOwner()->GetComponent<ColliderComponent>()->SetIsTrigger(false);
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

    SoundServiceLocator::GetSoundSystem().Play("Explosion");

    ExplosionEvent explosionevent{};
    m_pSubject->NotifyObservers(explosionevent);

    auto* worldGrid = GameManager::GetInstance().GetWorldGrid();

    auto pos = GetOwner()->GetTransform()->GetLocalPosition();
    auto& centerCell = worldGrid->GetGridCellByPosition(pos);


    const std::vector<std::pair<int, int>> directions = {
        {0, -1}, // Up
        {0, 1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };


    auto gridSize = worldGrid->GetGridSize();
    auto centerIndex = centerCell.Index;

    auto& scene = SceneManager::GetInstance().GetCurrentScene();

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
                auto& cell = worldGrid->GetGridCellByID({ x,y });

                if (auto objPtr = cell.ObjectOnCell.lock())
                {
                    auto collider = objPtr->GetComponent<ColliderComponent>();

                    if (collider == nullptr) return;

                    if (collider->CompareTag("Undestroyable"))
                        break;
                    else if (collider->CompareTag("Destroyable"))
                    {
                        objPtr->GetComponent<BreakableWallComponent>()->OnWallBreak();
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


                //0 = middle, 1 = Vertical, 2 = horizontal, 3 = endUp, 4 = EndDown, 5 = EndRight, 6 = EndLeft
                CreateChildExplosion(animationRow, cell.CenterCellPosition, scene);

            }
            else
            {
                break;
            }
        }
    }
    GetOwner()->Destroy();
}


void Jotar::BombComponent::CreateChildExplosion(int explosionPosition,const glm::vec2& pos, Scene& scene)
{
    std::string explosionTag = "Explosion";

    auto explosion = scene.CreateGameObject(explosionTag);

    auto texture = explosion->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedSpriteSheet(explosionTag), explosionPosition);
    texture->SetDestroyOnLastFrame(true);
    texture->SetAnimationSpeedInNrOfFramesPerSecond(m_ExplosionSpeed);
    explosion->GetTransform()->SetPosition(pos);
    auto triggerCollider = explosion->AddComponent<ColliderComponent>(true, true);
    triggerCollider->SetTag(explosionTag);

    int damage = 1;
    auto damageComp = explosion->AddComponent<DamageComponent>(damage, m_TagsToKill, m_BombPlacer);

    triggerCollider->AddObserver(damageComp);

    explosion->Start();
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



