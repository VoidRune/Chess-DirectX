#pragma once
#include "Window/Window.h"
#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_dx11.h"
#include "Vendor/ImGui/imgui_impl_win32.h"

enum class GameState
{
	Play = 0, MainMenu = 1, GameOver = 2
};

class MainMenu
{
public:
	MainMenu(GameState& state);
	~MainMenu();
	void Update();
	void Draw();
private:

	GameState* state;
};

class Button
{
public:
	Button();
	~Button();
	bool isHeld();
};

void ImGuiStartFrame();
void ImGuiEndFrame();