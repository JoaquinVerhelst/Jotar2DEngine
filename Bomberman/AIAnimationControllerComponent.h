#pragma once
#include "Component.h"
#include "ResourceManager.h"

namespace Jotar
{
	class GameObject;
	class TextureComponent;

	struct AnimationIndexesInfo
	{
		AnimationIndexesInfo(glm::ivec2 leftWalkStartEndIndexes = { 0,0 }, glm::ivec2 rightWalkStartEndIndexes = { 0,0 }, glm::ivec2 damageStartEndIndexes = { 0,0 }, glm::ivec2 deathStartEndIndexes = { 0,0 })
			: LeftWalkIndexes{ leftWalkStartEndIndexes }
			, RightWalkIndexes{ rightWalkStartEndIndexes }
			, DamageIndexes{ damageStartEndIndexes }
			, DeathIndexes{ deathStartEndIndexes }
		{
		}
		glm::ivec2 LeftWalkIndexes;
		glm::ivec2 RightWalkIndexes;
		glm::ivec2 DamageIndexes;
		glm::ivec2 DeathIndexes;
	};



	class AIAnimationControllerComponent final : public Component
	{
	public:

		AIAnimationControllerComponent(GameObject* owner, AnimationIndexesInfo animationIndexInfo);
		~AIAnimationControllerComponent() = default;


		void Start() override;

		AIAnimationControllerComponent(const AIAnimationControllerComponent& other) = delete;
		AIAnimationControllerComponent(AIAnimationControllerComponent&& other) = delete;
		AIAnimationControllerComponent& operator=(const AIAnimationControllerComponent& other) = delete;
		AIAnimationControllerComponent& operator=(AIAnimationControllerComponent&& other) = delete;

		void SetWalkAnimation(glm::ivec2 direction);
		void SetDamageAnimation();
		void SetDeathAnimation();
		void SetDestroyOnLastFrame(bool destroyItOnLastFrame);

	private:

		TextureComponent* m_pTextureComponent;
		AnimationIndexesInfo m_AnimationIndexesInfo;

	};
}