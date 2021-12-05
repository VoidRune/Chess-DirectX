#pragma once
#include <bitset>

class Mouse
{
	std::bitset<8> mouseStates;
	friend class Window;
public:
	Mouse() = default;
	bool LMouseIsPressed() noexcept;
	bool RMouseIsPressed() noexcept;
	bool LMouseDown() noexcept;
	bool RMouseDown() noexcept;
	void OnWheelUp();
	void OnWheelDown();
	int wheel = 1;
private:
	void OnLMousePressed();
	void OnLMouseReleased();
	void OnRMousePressed();
	void OnRMouseReleased();
	void OnLMouseDown();
	void OnRMouseDown();
	bool Lmouse = false;
	bool Rmouse = false;

	unsigned char key;
	void ClearState();
};