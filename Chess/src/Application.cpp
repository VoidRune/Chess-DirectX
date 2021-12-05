#include "Application.h"

Application::Application():
	window(1280, 720, "Chess", false),
	gfx(Window::GetHandle()),
	mainMenu(m_State)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window.GetHandle());
	ImGui_ImplDX11_Init(Graphics::GetDevice(), Graphics::GetDeviceContext());
}

void Application::Run()
{
	while (window.ProcessMessages())
	{
		gfx.ClearBuffer();
		ImGuiStartFrame();

		switch (m_State)
		{
			case GameState::Play:
			{
				game.Update();
				game.Draw();
				break;
			}
			case GameState::MainMenu:
			{
				mainMenu.Update();
				break;
			}
			case GameState::GameOver:
			{

				break;
			}
		}

		ImGuiEndFrame();
		gfx.SwapBuffer(1);
	}
}

Application::~Application()
{

}