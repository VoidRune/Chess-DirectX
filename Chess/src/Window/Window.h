#pragma once
#include <Windows.h>
#include "Graphics/Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"


class Window
{
public:
	Window(int width, int height, const char* name, bool isFullscreen = true);
	~Window();
	int ProcessMessages();
	static HWND GetHandle();
private:
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static Keyboard kbd;
	static Mouse mouse;
	static bool isFullscreen;
	static unsigned int screenWidth;
	static unsigned int screenHeight;
	static unsigned int m_Width;
	static unsigned int m_Height;
private:
	HINSTANCE hInstance;
	static HWND hWnd;
	const char* pclassName = "GameWindow";
	const char* windowName;
};

