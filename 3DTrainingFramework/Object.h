#pragma once
#include "../Utilities/Math.h"
#include <vector>

class Object
{
public:
	Object();
	~Object();
	int GetId() const;
	void SetId(int id);
	void SetModelId(int modelId);
	void SetTextureIds(std::vector<int> textureIds);
	void SetShaderId(int shaderId);
	void SetPosition(Vector3& position);
	void SetRotation(Vector3& rotation);
	void SetScale(Vector3& scale);
	void UpdateMatModel();
	int GetModelId() const;
	void SetModelId1(int modelId);
	std::vector<int> GetTextureIds() const;
	void SetTextureIds1(const std::vector<int>& textureIds);
	int GetShaderId() const;
	void SetShaderId1(int shaderId);
	Matrix GetMatModel() const;
private:
	int m_Id;
	int m_ModelId;
	std::vector<int>  m_TextureIds;
	int m_ShaderId;
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;
	Matrix m_matModel;
	bool m_bIsMatModelDirty;
};

