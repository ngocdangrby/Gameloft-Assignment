// 3DTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Globals.h"
#include <conio.h>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"


int Initialize(ESContext* esContext)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	SceneManager::GetInstance()->Initialize("../Resources/SM.txt");
	return ResourceManager::GetInstance()->Initialize("../Resources/RM.txt");
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SceneManager::GetInstance()->Draw();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{
	if (deltaTime)
	{
		SceneManager::GetInstance()->Update(deltaTime);
	}
}

void TouchActionDown(ESContext* esContext, int x, int y)
{
	InputManager::GetInstance()->SetMousePosition(0, x, y);
	InputManager::GetInstance()->SetMousePosition(1, x, y);
}

void TouchActionUp(ESContext* esContext, int x, int y)
{
	InputManager::GetInstance()->ResetMousePosition();
}

void TouchActionMove(ESContext* esContext, int x, int y)
{
	InputManager::GetInstance()->SetMousePosition(1, x, y);
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	switch (key)
	{
	case 'W': case 'w':
		InputManager::GetInstance()->SetKeyEvent(Forward, bIsPressed);
		break;
	case 'A': case 'a':
		InputManager::GetInstance()->SetKeyEvent(Left, bIsPressed);
		break;
	case 'S': case 's':
		InputManager::GetInstance()->SetKeyEvent(Backward, bIsPressed);
		break;
	case 'D': case 'd':
		InputManager::GetInstance()->SetKeyEvent(Right, bIsPressed);
		break;
	case 'Q': case 'q':
		InputManager::GetInstance()->SetKeyEvent(Up, bIsPressed);
		break;
	case 'E': case 'e':
		InputManager::GetInstance()->SetKeyEvent(Down, bIsPressed);
		break;
	case 'R': case 'r':
		InputManager::GetInstance()->SetKeyEvent(Reset, bIsPressed);
		break;
	}
}

void CleanUp()
{
	delete InputManager::GetInstance();
	delete SceneManager::GetInstance();
	delete ResourceManager::GetInstance();
}


int main(int argc, _TCHAR* argv[])
{
	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "3D Training Framework", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Initialize(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseDownFunc(&esContext, TouchActionDown);
	esRegisterMouseUpFunc(&esContext, TouchActionUp);
	esRegisterMouseMoveFunc(&esContext, TouchActionMove);

	esMainLoop(&esContext);

	// releasing OpenGL resources
	CleanUp();

	// identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}
