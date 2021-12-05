#pragma once
#include "Window/Window.h"
#include "Game/Game.h"
#include "Game/MainMenu.h"

class Application
{
public:
	Application();
	~Application();
	void Run();

private:
	Window window;
	Graphics gfx;
	Game game;
	MainMenu mainMenu;

	GameState m_State = GameState::MainMenu;
};

