#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "WorldGrid.h"
#include "Observer.h"
#include "CollisionEvents.h"
#include "Subject.h"

namespace Jotar
{

	class Scene;
	class ExplosionEvent;

	class BombComponent final : public Component, public Observer<CollisionEvent>
	{
	public:

		explicit BombComponent(GameObject* owner, float explodeTime, int range);
		virtual ~BombComponent() = default;


		void Update() override;
		void OnDestroy() override;

		void OnNotify(const CollisionEvent& event) override;
		void AddObserver(Observer<ExplosionEvent>* pObserver);
		void RemoveObserver(Observer<ExplosionEvent>* pObserver);


		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;


		void OnExplode(int range);


	private:


		//0 = middle, 1 = Vertical, 2 = horizontal, 3 = endUp, 4 = EndDown, 5 = EndRight, 6 = EndLeft
		void CreateChildExplosion(int explosionPosition, const glm::vec2& pos, Scene& scene);
		int CalculateSpriteSheetRow(int xDir, int yDir, int range, int currentRange);

		std::unique_ptr<Subject<ExplosionEvent>> m_pSubject;

		int m_Range;
		float m_ExplodeTime;
		float m_TimeCounter;
		bool m_IsExploded;

	};
}
