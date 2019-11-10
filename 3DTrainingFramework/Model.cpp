#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"
#include "../Utilities/TGA.h"
#include <iostream>
#include <string>
#include "../Utilities/Math.h"
#include <vector>
#include  <glm/glm.hpp>

Model::Model() : m_Id(0), m_NIndices(0), m_NVertices(0), m_VboId(0), m_IboId(0)
{
}

Model::~Model()
{
	glDeleteBuffers(1, &m_VboId);
	glDeleteBuffers(1, &m_IboId);
}

void Model::LoadModel(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if (f)
	{
		std::vector<glm::vec3> out_vertices;
		std::vector<glm::vec2> out_uvs;
		std::vector<glm::vec3> out_normals;
		Vertex* vertices;

		std::string fname(filename);

		if (fname.substr(fname.find_last_of(".") + 1) != "obj") {
			fscanf(f, "NrVertices: %d\n", &m_NVertices);
			vertices = new Vertex[m_NVertices];
			if (strcmp(filename, "../Resources/Models/Terrain.nfg") == 0)
			{
				FILE* file;
				file = fopen("../Resources/Textures/heightmap33.raw", "rb");
				unsigned char* heightMap = new unsigned char[1089];
				fread(heightMap, 1, 1089, file);
				fclose(file);

				for (int i = 0; i < m_NVertices; i++)
				{
					Vertex tmp;
					fscanf(f, "%*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
						&tmp.pos.x, &tmp.pos.y, &tmp.pos.z, &tmp.uv.x, &tmp.uv.y);
					tmp.pos.y = heightMap[i];
					vertices[i] = tmp;
				}
				delete[] heightMap;
			}
			else
			{
				for (int i = 0; i < m_NVertices; i++)
				{
					Vertex tmp;
					fscanf(f, "%*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
						&tmp.pos.x, &tmp.pos.y, &tmp.pos.z, &tmp.uv.x, &tmp.uv.y);
					vertices[i] = tmp;
				}
			}

			fscanf(f, "NrIndices: %d\n", &m_NIndices);
			unsigned int* indices = new unsigned int[m_NIndices];
			for (int i = 0; i < m_NIndices / 3; i++)
			{
				fscanf(f, "%*d.    %d,    %d,    %d\n", &indices[3 * i], &indices[3 * i + 1], &indices[3 * i + 2]);
			}
			glGenBuffers(1, &m_VboId);
			glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
			printf("\nm_NVertices = %d", m_NVertices);
			printf("\nm_NIndices = %d", m_NIndices);

			glBufferData(GL_ARRAY_BUFFER, m_NVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glGenBuffers(1, &m_IboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			delete[] vertices;
			delete[] indices;
		}
		else {
			printf("Loading... %s" , filename);
			std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
			std::vector<glm::vec3> temp_vertices;
			std::vector<glm::vec2> temp_uvs;
			std::vector<glm::vec3> temp_normals;

			while (1) {
				char lineHeader[128];
				// read the first word of the line
				int res = fscanf(f, "%s", lineHeader);
				if (res == EOF)
					break; // EOF = End Of File. Quit the loop.

				// else : parse lineHeader
				if (strcmp(lineHeader, "v") == 0) {
					glm::vec3 vertex;
					fscanf(f, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					glm::vec2 uv;
					fscanf(f, "%f %f\n", &uv.x, &uv.y);
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					glm::vec3 normal;
					fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					std::string vertex1, vertex2, vertex3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (matches != 9) {
						return;
					}
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
			}
			// For each vertex of each triangle
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndex = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				out_vertices.push_back(vertex);
			}

			m_NVertices = temp_vertices.size();
			m_NIndices = vertexIndices.size();
			vertices = new Vertex[m_NVertices];
			unsigned int* indices = new unsigned int[m_NIndices];

			for (int i = 0; i < m_NVertices; i++)
			{
				Vertex tmp;
				tmp.pos.x = temp_vertices[i].x;
				tmp.pos.y = temp_vertices[i].y;
				tmp.pos.z = temp_vertices[i].z;
				tmp.uv.x = temp_uvs[i].x;
				tmp.uv.y = temp_uvs[i].y;
				vertices[i] = tmp;
			}
			for (int i = 0; i < m_NIndices; i++) {
				indices[i] = vertexIndices[i] - 1;
			}

			glGenBuffers(1, &m_VboId);
			glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
			glBufferData(GL_ARRAY_BUFFER, m_NVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glGenBuffers(1, &m_IboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			delete[] vertices;
			printf("ok");
		}
	}
	else
	{
		printf("Failed to open file: %s\n", filename);
	}
}

GLuint Model::GetVboId() const
{
	return m_VboId;
}

GLuint Model::GetIboId() const
{
	return m_IboId;
}

int Model::GetNIndices() const
{
	return m_NIndices;
}

int Model::GetNVertices() const
{
	return m_NVertices;
}

int Model::GetId() const
{
	return m_Id;
}

void Model::SetId(int id)
{
	m_Id = id;
}
