#pragma once
#include "Component.h"
#include "Observer.h"
#include "WindowEvents.h"
#include <glm/glm.hpp>
#include <unordered_map>


namespace Jotar
{
	class GameObject;
	class TextureComponent;


	enum class HUDPosition
	{
		Center,
		CenterUp,
		CenterDown,
		CenterRight,
		RightUp,
		RightDown,
		CenterLeft,
		LeftUp,
		LeftDown
	};


	class HUDComponent : public Component, public Observer<WindowResizeEvent>
	{
	public:
		HUDComponent(GameObject* owner, HUDPosition position = HUDPosition::Center, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
		~HUDComponent();


		void Start() override;
		void OnNotify(const WindowResizeEvent& event) override;
		void OnDestroy() override;

		void SetPosition(HUDPosition position, glm::vec2 offset = glm::vec2(0.0f, 0.0f));


		HUDComponent(const HUDComponent& other) = delete;
		HUDComponent(HUDComponent&& other) = delete;
		HUDComponent& operator=(const HUDComponent& other) = delete;
		HUDComponent& operator=(HUDComponent&& other) = delete;
	private:

		glm::vec2 CalculatePosition(HUDPosition position, glm::ivec2 size, glm::vec2 screenSize);

		void CalculateScreenPos();


		//TextureComponent* m_pTextureComponent;

		HUDPosition m_Position;
		glm::vec2 m_Offset;

		static const std::unordered_map<HUDPosition, glm::vec2> s_PositionOffsets;

	};

}