#include "stdafx.h"
#include "InputManager.h"

InputManager* InputManager::s_Instance = nullptr;

InputManager::InputManager()
{
	for (int i = 0; i < N_KEYS; i++)
	{
		m_bIsPressed[i] = false;
	}
	m_vec2MousePosition[0] = m_vec2MousePosition[1];
}


InputManager::~InputManager()
{
}

void InputManager::SetKeyEvent(int keyNumber, bool isPressed)
{
	m_bIsPressed[keyNumber] = isPressed;
}

void InputManager::SetMousePosition(int index, int x, int y)
{
	m_vec2MousePosition[index] = Vector2(x, y);
}

bool InputManager::IsPressed(int keyNumber)
{
	return m_bIsPressed[keyNumber];
}

Vector2 InputManager::GetMousePosition(int index)
{
	return m_vec2MousePosition[index];
}

Vector2 InputManager::ResetMousePosition()
{
	Vector2 delta = m_vec2MousePosition[1] - m_vec2MousePosition[0];
	m_vec2MousePosition[0] = m_vec2MousePosition[1];
	return delta;
}

InputManager* InputManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new InputManager();
	}
	return s_Instance;
}
