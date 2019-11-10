#include "stdafx.h"
#include "Camera.h"
#include <cmath>


Camera::Camera() : m_vec3Position(), m_vec3Up(0.0f, 1.0f, 0.0f), m_isViewDirty(true), m_fSpeed(10)
{
	Rotate();
}

Camera::Camera(float speed): m_fSpeed(speed), m_vec3Position(0.0f, 4.0f, 0.0f), m_vec3Up(0.0f, 1.0f, 0.0f), m_isViewDirty(true)
{
	Rotate();
}

Camera::~Camera()
{
}

Matrix Camera::GetViewMatrix() const
{
	return m_matView;
}

void Camera::UpdateViewMatrix()
{
	if (m_isViewDirty)
	{
		Vector3 zaxis = (m_vec3Position - m_vec3Target).Normalize();
		Vector3 xaxis = m_vec3Up.Cross(zaxis).Normalize();
		Vector3 yaxis = zaxis.Cross(xaxis);
		m_matView.m[0][0] = xaxis.x;
		m_matView.m[0][1] = yaxis.x;
		m_matView.m[0][2] = zaxis.x;
		m_matView.m[0][3] = 0;
		m_matView.m[1][0] = xaxis.y;
		m_matView.m[1][1] = yaxis.y;
		m_matView.m[1][2] = zaxis.y;
		m_matView.m[1][3] = 0;
		m_matView.m[2][0] = xaxis.z;
		m_matView.m[2][1] = yaxis.z;
		m_matView.m[2][2] = zaxis.z;
		m_matView.m[2][3] = 0;
		m_matView.m[3][0] = -xaxis.Dot(m_vec3Position);
		m_matView.m[3][1] = -yaxis.Dot(m_vec3Position);
		m_matView.m[3][2] = -zaxis.Dot(m_vec3Position);
		m_matView.m[3][3] = 1;
		m_isViewDirty = false;
	}
}

void Camera::Move(Vector3 direction, float deltaTime)
{
	Vector3 deltaMove;
	Vector3 zaxis = (m_vec3Position - m_vec3Target).Normalize();
	Vector3 xaxis = m_vec3Up.Cross(zaxis).Normalize();
	Vector3 yaxis = zaxis.Cross(xaxis);
	deltaMove += zaxis * direction.z + xaxis * direction.x + yaxis * direction.y;
	deltaMove *= deltaTime * m_fSpeed;
	if (deltaMove.Length())
	{
		m_vec3Position += deltaMove;
		m_vec3Target += deltaMove;
		m_isViewDirty = true;
	}
}

void Camera::Rotate(float angleX, float angleY)
{
	Vector3 front;
	front.x = cos(angleX) * cos(angleY);
	front.y = sin(angleX);
	front.z = cos(angleX) * sin(angleY);
	m_vec3Target = m_vec3Position + front.Normalize();
	m_isViewDirty = true;
}

void Camera::Reset()
{
	m_vec3Position = Vector3();
	m_vec3Up = Vector3(0.0f, 1.0f, 0.0f);
	Rotate();
	m_isViewDirty = true;
}
