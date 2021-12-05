#include "MainMenu.h"

MainMenu::MainMenu(GameState& state):
	state(&state)
{

}

MainMenu::~MainMenu()
{
    ImGui_ImplWin32_Shutdown();
}

void MainMenu::Update()
{
    Window::mouse.LMouseDown();
	ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("PLAY!", { 500, 200 }))
	{
        *state = GameState::Play;
	}

	ImGui::End();

    //RECT winPos;
    //GetClientRect(Window::GetHandle(), &winPos);
    //POINT ClientAreaPos = { winPos.left, winPos.top };
    //ClientToScreen(Window::GetHandle(), &ClientAreaPos);
    //POINT MousePos = {};
    //GetCursorPos(&MousePos);
    //bool hasChanged = false;
    //bool clicked = Window::mouse.LMouseDown();
    //
	//if (clicked)
	//	*state = GameState::Play;
}

void MainMenu::Draw()
{

}


Button::Button()
{

}

Button::~Button()
{

}

bool Button::isHeld()
{
	return true;
}

void ImGuiStartFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}
void ImGuiEndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}