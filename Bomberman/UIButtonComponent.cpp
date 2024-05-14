#include "UIButtonComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

#include "SDL.h"
#include "Renderer.h"
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()

Jotar::UIButtonComponent::UIButtonComponent(GameObject* owner, const std::function<void()>& buttonFunction)
	: Component(owner)
	, m_ButtonFunction{ buttonFunction }
	, m_ButtonRect{}
{
}

void Jotar::UIButtonComponent::Render() const
{
	//// Get the shape (assuming it returns glm::ivec4 as {x, y, width, height})
	//glm::ivec4 shape = GetOwner()->GetTransform()->GetShape();

	//// Define an SDL_Rect using the shape
	//SDL_Rect rect;
	//rect.x = shape.x;
	//rect.y = shape.y;
	//rect.w = shape.w;
	//rect.h = shape.z;

	//// Generate random color values
	//Uint8 r = static_cast<Uint8>(std::rand() % 256);
	//Uint8 g = static_cast<Uint8>(std::rand() % 256);
	//Uint8 b = static_cast<Uint8>(std::rand() % 256);
	//Uint8 a = 255; // Full opacity

	//// Set the draw color to the random color
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), r, g, b, a);

	//// Render the filled rectangle with the random color
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);

	//// Optionally, draw the rectangle outline with another random color
	//r = static_cast<Uint8>(std::rand() % 256);
	//g = static_cast<Uint8>(std::rand() % 256);
	//b = static_cast<Uint8>(std::rand() % 256);


	//// Optionally, draw the rectangle outline
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255); // Outline color, for example, black
	//SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
}

void Jotar::UIButtonComponent::Start()
{
	m_ButtonRect = GetOwner()->GetTransform()->GetShape();
}


void Jotar::UIButtonComponent::PressButton()
{

	if (m_ButtonFunction)
	{
		m_ButtonFunction();
	}
}

bool Jotar::UIButtonComponent::IsPointInRect(const glm::ivec2& mousePos)
{
	m_ButtonRect = GetOwner()->GetTransform()->GetShape();

	return (mousePos.x >= m_ButtonRect.x &&
		mousePos.x <= m_ButtonRect.x + m_ButtonRect.w &&
		mousePos.y >= m_ButtonRect.y &&
		mousePos.y <= m_ButtonRect.y + m_ButtonRect.z);
}
