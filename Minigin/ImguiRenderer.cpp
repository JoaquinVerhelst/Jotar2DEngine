#include "ImguiRenderer.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui_plot.h"
#include "Renderer.h"
#include <vector>
#include <chrono>

Jotar::ImguiRenderer::ImguiRenderer()
	:m_Window{}
	, m_AmountOfIntegerSamples{10}
	, m_AmountOfGameObjectSamples{10}
	, m_YdataGO{}
	, m_YdataGOAlt{}
	, m_YdataInt{}
{
}

Jotar::ImguiRenderer::~ImguiRenderer() = default;



void Jotar::ImguiRenderer::Init(SDL_Window* window, const ImguiWindowProperties& properties)
{
	m_Window = window;


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	InitializeProperties(properties);


	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void Jotar::ImguiRenderer::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	RenderImgui();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& IO = ImGui::GetIO();
	if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(m_Window, SDL_GL_GetCurrentContext());

	}
}

void Jotar::ImguiRenderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}



void Jotar::ImguiRenderer::InitializeProperties(const ImguiWindowProperties& properties)
{
	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigWindowsMoveFromTitleBarOnly = properties.MoveFromTitleBarOnly;
	if (properties.IsDockingEnabled)
	{
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if (properties.IsViewportEnabled)
	{
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

}

// Game Texture here was here / will be here to render the game on to IMGUI to make a scene window
void Jotar::ImguiRenderer::RenderImgui(/*SDL_Texture* gameTexture*/)
{
	ImGui::Begin("Exercise 1");

	ImGui::InputInt("Samples", &m_AmountOfIntegerSamples, 0, 10000000);

	if (ImGui::Button("Trash the Cache"))
	{
		TrashTheCacheExercise1(m_AmountOfIntegerSamples);
		m_IsIntDataCached = true;
	}
	if (m_IsIntDataCached)
	{
		DrawIntegerGraph();
	}
	ImGui::End();


	ImGui::Begin("Exercise 2");

	ImGui::InputInt("Samples", &m_AmountOfGameObjectSamples, 0, 10000000);

	if (ImGui::Button("Trash the Cache GameObjects"))
	{
		TrashTheCacheExercise2GO(m_AmountOfGameObjectSamples);
		m_IsGODataCached = true;
	}
	if (m_IsGODataCached)
	{
		DrawGOGraph();
	}

	if (ImGui::Button("Trash the Cache GameObjects Alts"))
	{
		TrashTheCacheExercise2GOAlt(m_AmountOfGameObjectSamples);
		m_IsGOAltDataCached = true;
	}
	if (m_IsGOAltDataCached)
	{
		DrawGOAltGraph();
	}

	if (m_IsGOAltDataCached && m_IsGODataCached)
	{
		DrawCombination();
	}

	ImGui::End();
}


void Jotar::ImguiRenderer::TrashTheCacheExercise1(int amountOfSamples)
{
	const int size = 10000000;
	int* numbers = new int[size] {};

	std::vector<float> averageDurations{};
	averageDurations.resize(11, 0);

	int index = 0;

	for (int i = 0; i < amountOfSamples; i++)
	{
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();


			for (int y = 0; y < size; y += stepsize)
			{
				numbers[y] *= 2;
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			averageDurations[index] += total / 100.f;
			++index;
		}

		index = 0;
	}

	// remove first and last 
	averageDurations.erase(averageDurations.begin());
	averageDurations.pop_back();

	// divide it vy the amoount of tests
	for (size_t i = 0; i < averageDurations.size(); i++)
	{
		averageDurations[i] /= amountOfSamples;
		m_YdataInt[i] = averageDurations[i];
	}

	delete[] numbers;
	numbers = nullptr;

}

void Jotar::ImguiRenderer::TrashTheCacheExercise2GO(int amountOfSamples)
{
	const int size = 10000000;

	std::vector<float> averageDurations{};

	GameObjectTTC* matrix = new GameObjectTTC[size]{};

	averageDurations.resize(11, 0);

	int index = 0;

	for (int i = 0; i < amountOfSamples; i++)
	{
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();


			for (int y = 0; y < size; y += stepsize)
			{
				matrix[y].ID *= 2;
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();


			averageDurations[index] += total / 100.f;
			++index;
		}
		index = 0;
	}

	// remove first and last 
	averageDurations.erase(averageDurations.begin());
	averageDurations.pop_back();

	// divide it vy the amoount of tests
	for (size_t i = 0; i < averageDurations.size(); i++)
	{
		averageDurations[i] /= amountOfSamples;
		m_YdataGO[i] = averageDurations[i];
	}

	delete[] matrix;
	matrix = nullptr;
}

void Jotar::ImguiRenderer::TrashTheCacheExercise2GOAlt(int amountOfSamples)
{
	const int size = 10000000;

	std::vector<float> averageDurations{};

	GameObjectAltTTC* matrix = new GameObjectAltTTC[size]{};

	averageDurations.resize(11, 0);

	int index = 0;

	for (int i = 0; i < amountOfSamples; i++)
	{
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();


			for (int y = 0; y < size; y += stepsize)
			{
				matrix[y].ID *= 2;
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();


			averageDurations[index] += total / 100.f;
			++index;
		}
		index = 0;
	}

	// remove first and last 
	averageDurations.erase(averageDurations.begin());
	averageDurations.pop_back();

	// divide it vy the amoount of tests
	for (size_t i = 0; i < averageDurations.size(); i++)
	{
		averageDurations[i] /= amountOfSamples;
		m_YdataGOAlt[i] = averageDurations[i];
	}

	delete[] matrix;
	matrix = nullptr;
}



void Jotar::ImguiRenderer::DrawIntegerGraph()
{
	static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
	static uint32_t selection_start = 0, selection_length = 0;

	ImGui::PlotConfig conf;
	conf.values.xs = m_Xdata;
	conf.values.count = 9;
	conf.values.ys = m_YdataInt;
	conf.values.ys_count = 1;
	conf.values.colors = colors;
	conf.scale.min = 0.f;
	conf.scale.max = 80.f;
	conf.tooltip.show = true;
	conf.grid_x.show = true;
	conf.grid_x.size = 512.f;
	conf.grid_x.subticks = 5;
	conf.grid_y.show = true;
	conf.grid_y.size = 0.5f;
	conf.grid_y.subticks = 5;
	conf.selection.show = true;
	conf.selection.start = &selection_start;
	conf.selection.length = &selection_length;
	conf.frame_size = ImVec2(512, 200);
	ImGui::Plot("Integers", conf);
}

void Jotar::ImguiRenderer::DrawGOGraph()
{
	static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
	static uint32_t selection_start = 0, selection_length = 0;

	ImGui::PlotConfig conf;
	conf.values.xs = m_Xdata;
	conf.values.count = 9;
	conf.values.ys = m_YdataGO;
	conf.values.ys_count = 1;
	conf.values.colors = colors;
	conf.scale.min = 0.f;
	conf.scale.max = 400.f;
	conf.tooltip.show = true;
	conf.grid_x.show = true;
	conf.grid_x.size = 512.f;
	conf.grid_x.subticks = 5;
	conf.grid_y.show = true;
	conf.grid_y.size = 0.5f;
	conf.grid_y.subticks = 5;
	conf.selection.show = true;
	conf.selection.start = &selection_start;
	conf.selection.length = &selection_length;
	conf.frame_size = ImVec2(512, 200);
	ImGui::Plot("GameObject", conf);
}

void Jotar::ImguiRenderer::DrawGOAltGraph()
{
	static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
	static uint32_t selection_start = 0, selection_length = 0;

	ImGui::PlotConfig conf;
	conf.values.xs = m_Xdata;
	conf.values.count = 9;
	conf.values.ys = m_YdataGOAlt;
	conf.values.ys_count = 1;
	conf.values.colors = colors;
	conf.scale.min = 0.f;
	conf.scale.max = 200.f;
	conf.tooltip.show = true;
	conf.grid_x.show = true;
	conf.grid_x.size = 512.f;
	conf.grid_x.subticks = 5;
	conf.grid_y.show = true;
	conf.grid_y.size = 0.5f;
	conf.grid_y.subticks = 5;
	conf.selection.show = true;
	conf.selection.start = &selection_start;
	conf.selection.length = &selection_length;
	conf.frame_size = ImVec2(512, 200);
	ImGui::Plot("GameObject Alt", conf);
}

void Jotar::ImguiRenderer::DrawCombination()
{
	static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
	static uint32_t selection_start = 0, selection_length = 0;

	static const float* y_data[] = { m_YdataGO, m_YdataGOAlt };

	ImGui::PlotConfig conf;
	conf.values.xs = m_Xdata;
	conf.values.count = 9;
	conf.values.ys_list = y_data;
	conf.values.ys_count = 2;
	conf.values.colors = colors;
	conf.scale.min = 0.f;
	conf.scale.max = 400.f;
	conf.tooltip.show = true;
	conf.grid_x.show = true;
	conf.grid_x.size = 512.f;
	conf.grid_x.subticks = 5;
	conf.grid_y.show = true;
	conf.grid_y.size = 0.5f;
	conf.grid_y.subticks = 5;
	conf.selection.show = true;
	conf.selection.start = &selection_start;
	conf.selection.length = &selection_length;
	conf.frame_size = ImVec2(512, 200);
	ImGui::Plot("plot2", conf);
}


