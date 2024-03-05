#include "ImguiRenderer.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>



dae::ImguiRenderer::ImguiRenderer()
	:m_Window{}
{
}

dae::ImguiRenderer::~ImguiRenderer() = default;



void dae::ImguiRenderer::Init(SDL_Window* window, const ImguiWindowProperties& properties)
{
	m_Window = window;


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	InitializeProperties(properties);


	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::ImguiRenderer::Render() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

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

void dae::ImguiRenderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void dae::ImguiRenderer::InitializeProperties(const ImguiWindowProperties& properties)
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

void dae::ImguiRenderer::RenderImgui() const
{

	//ImGui::ShowDemoWindow();

	//ImGui::End();

	if (ImGui::Begin("Test1"))
	{
	}
	ImGui::End();

	if (ImGui::Begin("Test2"))
	{
	}
	ImGui::End();
}
