#include "stdafx.h"
#include "Object.h"


Object::Object(): m_Id(-1), m_ModelId(-1), m_ShaderId(-1), m_bIsMatModelDirty(false)
{
}


Object::~Object()
{
}

int Object::GetId() const
{
	return m_Id;
}

void Object::SetId(int id)
{
	m_Id = id;
}

void Object::SetModelId(int modelId)
{
	m_ModelId = modelId;
}

void Object::SetTextureIds(std::vector<int> textureIds)
{
	m_TextureIds = textureIds;
}

void Object::SetShaderId(int shaderId)
{
	m_ShaderId = shaderId;
}

void Object::SetPosition( Vector3& position)
{
	m_Position = position;
	m_bIsMatModelDirty = true;
}

void Object::SetRotation(Vector3& rotation)
{
	m_Rotation = rotation;
	m_bIsMatModelDirty = true;
}

void Object::SetScale(Vector3& scale)
{
	m_Scale = scale;
	m_bIsMatModelDirty = true;
}

void Object::UpdateMatModel()
{
	if (m_bIsMatModelDirty)
	{
		Matrix matScale, matRotation, matRotZ, matRotX, matRotY, matTransalation;
		matScale.SetScale(m_Scale);
		matRotZ.SetRotationZ(Radians(m_Rotation.z));
		matRotX.SetRotationX(Radians(m_Rotation.x));
		matRotY.SetRotationY(Radians(m_Rotation.y));
		matRotation = matRotZ * matRotX * matRotY;
		matTransalation.SetTranslation(m_Position);
		m_matModel = matScale * matRotation * matTransalation;
		m_bIsMatModelDirty = false;
	}
}

int Object::GetModelId() const
{
	return m_ModelId;
}

void Object::SetModelId1(int modelId)
{
	m_ModelId = modelId;
}

std::vector<int> Object::GetTextureIds() const
{
	return m_TextureIds;
}

void Object::SetTextureIds1(const std::vector<int>& textureIds)
{
	m_TextureIds = textureIds;
}

int Object::GetShaderId() const
{
	return m_ShaderId;
}

void Object::SetShaderId1(int shaderId)
{
	m_ShaderId = shaderId;
}

Matrix Object::GetMatModel() const
{
	return m_matModel;
}
