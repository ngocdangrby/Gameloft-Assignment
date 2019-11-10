#pragma once
#include "../Utilities/Math.h"


class Camera
{
public:
	Camera();
	Camera(float speed);
	~Camera();
	Matrix GetViewMatrix() const;
	void UpdateViewMatrix();
	void Move(Vector3 direction, float deltaTime);
	void Rotate(float angleX = 0.0f, float angleY = 0.0f);
	void Reset();
private:
	Matrix m_matView;
	Vector3 m_vec3Position;
	Vector3 m_vec3Target;
	Vector3 m_vec3Up;
	bool m_isViewDirty;
	float m_fSpeed;
};

