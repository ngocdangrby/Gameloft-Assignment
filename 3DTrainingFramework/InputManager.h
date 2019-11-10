#pragma once
#include "../Utilities/Math.h"

enum Key { Forward, Backward, Left, Right, Up, Down, Reset, N_KEYS };

class InputManager
{
public:
	~InputManager();
	void SetKeyEvent(int keyNumber, bool isPressed);
	void SetMousePosition(int index, int x, int y);
	bool IsPressed(int keyNumber);
	Vector2 GetMousePosition(int index);
	Vector2 ResetMousePosition();
	static InputManager* GetInstance();
private:
	InputManager();
	bool m_bIsPressed[N_KEYS];
	Vector2 m_vec2MousePosition[2];
	static InputManager * s_Instance;
};

