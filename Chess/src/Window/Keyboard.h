#pragma once
#include <bitset>

class Keyboard
{
	std::bitset<256> keyStates;
	friend class Window;
public:
	Keyboard() = default;
	bool KeyIsPressed(unsigned char keycode) noexcept;
	bool KeyDown(unsigned char keycode) noexcept;
private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnKeyDown(unsigned char keycode);
	unsigned char key;
	static constexpr unsigned int nKeys = 256u;
	void ClearState();
};
