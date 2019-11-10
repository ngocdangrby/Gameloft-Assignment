#pragma once
#include "Object.h"
#include "Camera.h"


class SceneManager
{
public:
	~SceneManager();
	static SceneManager * GetInstance();
	void Initialize(const char * filename);
	void Draw();
	void Update(float deltaTime);
private:
	SceneManager();
	std::vector<Object *> m_ObjectList;
	Camera m_Camera;
	float m_fPitch;
	float m_fYaw;
	Matrix m_matProjection;
	static SceneManager * s_Instance;
};

