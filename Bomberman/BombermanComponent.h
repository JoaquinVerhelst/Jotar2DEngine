#pragma once
#include "Component.h"

/// <summary>
///  MAYBE NOT NEEDED
/// </summary>
/// 
/// 
namespace Jotar
{
	class GameObject;

	class BombermanComponent : public Component
	{
	public:
		BombermanComponent(GameObject* owner);

		BombermanComponent(const BombermanComponent& other) = delete;
		BombermanComponent(BombermanComponent&& other) = delete;
		BombermanComponent& operator=(const BombermanComponent& other) = delete;
		BombermanComponent& operator=(BombermanComponent&& other) = delete;

	private:



	};
}
