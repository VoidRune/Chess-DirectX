#include "Mouse.h"

bool Mouse::LMouseIsPressed() noexcept
{
	return mouseStates[0];
}
bool Mouse::RMouseIsPressed() noexcept
{
	return mouseStates[1];
}

void Mouse::OnLMousePressed()
{
	mouseStates[0] = true;
}

void Mouse::OnLMouseReleased()
{
	mouseStates[0] = false;
}

void Mouse::OnRMousePressed()
{
	mouseStates[1] = true;
}

void Mouse::OnRMouseReleased()
{
	mouseStates[1] = false;
}

void Mouse::ClearState()
{
	Lmouse = 0;
	Rmouse = 0;
	mouseStates.reset();
}

bool Mouse::LMouseDown() noexcept
{
	if (Lmouse)
	{
		Lmouse = 0;
		return true;
	}
	return false;
}
bool Mouse::RMouseDown() noexcept
{
	if (Rmouse)
	{
		Rmouse = 0;
		return true;
	}
	return false;
}

void Mouse::OnLMouseDown()
{
	Lmouse = true;
}
void Mouse::OnRMouseDown()
{
	Rmouse = true;
}

void Mouse::OnWheelUp()
{
	wheel += 1;
	if (wheel > 10)
		wheel = 10;
}

void Mouse::OnWheelDown()
{
	wheel -= 1;
	if (wheel < 1)
		wheel = 1;
}