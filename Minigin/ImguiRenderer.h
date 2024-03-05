#pragma once


#include <SDL.h>


namespace dae
{
	// credit: https://www.youtube.com/watch?v=lID9X5TT774


	struct ImguiWindowProperties
	{
		bool MoveFromTitleBarOnly = true;
		bool IsDockingEnabled = true;
		bool IsViewportEnabled = true;
	};





	class ImguiRenderer final 
	{
	public:
		ImguiRenderer();
		~ImguiRenderer();	

		void Init(SDL_Window* window, const ImguiWindowProperties& properties);
		void Render() const;

		void Destroy();

		ImguiRenderer(const ImguiRenderer& other) = delete;
		ImguiRenderer(ImguiRenderer&& other) = delete;
		ImguiRenderer& operator=(const ImguiRenderer& other) = delete;
		ImguiRenderer& operator=(ImguiRenderer&& other) = delete;

	private:

		void InitializeProperties(const ImguiWindowProperties& properties);
		void RenderImgui() const;
		SDL_Window* m_Window{};

	};
}
