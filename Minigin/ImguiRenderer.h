#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace Jotar
{
	// credit: https://www.youtube.com/watch?v=lID9X5TT774


	struct ImguiWindowProperties
	{
		bool MoveFromTitleBarOnly = false;
		bool IsDockingEnabled = false;
		bool IsViewportEnabled = false;
	};


	struct TransformTTC
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };

	};

	class GameObjectAltTTC
	{
	public:
		TransformTTC* transform;
		int ID{};

	};


	class GameObjectTTC
	{
	public:
		TransformTTC transform;
		int ID{};

	};



	class ImguiRenderer final : public Singleton<ImguiRenderer>
	{
	public:
		ImguiRenderer();
		~ImguiRenderer();	

		void Init(SDL_Window* window, const ImguiWindowProperties& properties);
		void Render(/*SDL_Texture* gameTexture*/);

		void Destroy();

		ImguiRenderer(const ImguiRenderer& other) = delete;
		ImguiRenderer(ImguiRenderer&& other) = delete;
		ImguiRenderer& operator=(const ImguiRenderer& other) = delete;
		ImguiRenderer& operator=(ImguiRenderer&& other) = delete;

	private:

		void InitializeProperties(const ImguiWindowProperties& properties);
		void RenderImgui(/*SDL_Texture* gameTexture*/);

		void TrashTheCacheExercise1(int amountOfSamples);
		void TrashTheCacheExercise2GO(int amountOfSamples);
		void TrashTheCacheExercise2GOAlt(int amountOfSamples);


		void DrawIntegerGraph();
		void DrawGOGraph();
		void DrawGOAltGraph();
		void DrawCombination();

		SDL_Window* m_Window{};
		//unsigned int m_Fbo;


		bool m_IsIntDataCached = false;
		bool m_IsGODataCached = false;
		bool m_IsGOAltDataCached = false;

		int m_AmountOfIntegerSamples;
		int m_AmountOfGameObjectSamples;

		// temp magic numbers
		const float m_Xdata[9] = { 2.f,4.f,8.f,16.f,32.f,64.f,128.f,256.f,512.f };
		float m_YdataInt[9];
		float m_YdataGO[9];
		float m_YdataGOAlt[9];
	};
}
