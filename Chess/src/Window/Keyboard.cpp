#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) noexcept
{
	return keyStates[keycode];
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	keyStates[keycode] = true;
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyStates[keycode] = false;
}

void Keyboard::ClearState()
{
	key = 0;
	keyStates.reset();
}

bool Keyboard::KeyDown(unsigned char keycode) noexcept
{
	if (key == keycode)
	{
		key = 0;
		return true;
	}
	//key = 0;
	return false;
}

void Keyboard::OnKeyDown(unsigned char keycode)
{
	key = keycode;
}