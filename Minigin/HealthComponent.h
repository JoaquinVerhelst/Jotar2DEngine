#pragma once
#include "Component.h"
#include "Subject.h"
#include "EventPlayerDamage.h"


namespace Jotar
{
	class GameObject;

	class HealthComponent final : public Component, public Subject<EventPlayerDamage>
	{
	public:

		HealthComponent(GameObject* owner, int health = 3);
		~HealthComponent();

		virtual void Start() override;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;


		int GetHealth() const;
		void TakeDamage(int damage = 1);
		void AddHealth(int health);

	private:

		int m_CurrentHealth;
	};

}