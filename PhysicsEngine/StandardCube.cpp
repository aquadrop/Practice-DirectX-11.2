#include "pch.h"
#include "StandardCube.h"

using namespace PhysicsEngine;
using namespace DirectX;

StandardCube::StandardCube(double a, DirectX::XMFLOAT3 position)
{
	this->a = a;

	this->set_n_mesh();
	this->set_n_color_vertex();
	this->set_n_texture_vertex();

	this->Position(position);

	this->create_color_mesh();
	this->create_mesh_color_index();

	this->create_texture_mesh();
	this->create_mesh_texture_index();

}

void StandardCube::set_n_mesh()
{
	this->n_mesh = 36;
}

void StandardCube::set_n_color_vertex()
{
	this->n_color_vertex = 8;
}

void StandardCube::set_n_texture_vertex()
{
	this->n_texture_vertex = 24;
}

void StandardCube::create_color_mesh()
{
	float x = m_position.x;
	float y = m_position.y;
	float z = m_position.z;

	VertexPositionColor vertexPositionColor[8] =
	{
		{ XMFLOAT3(-a + x, -a+y, -a+z), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-a+x, -a+y, a+z), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-a+x, a+y, -a+z), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-a+x, a+y, a+z), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(a+x, -a+y, -a+z), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(a+x, -a+y, a+z), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(a+x, a+y, -a+z), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(a+x, a+y, a+z), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};
	this->vertexPositionColor = new VertexPositionColor[8];
	for (int i = 0; i < 8; i++)
	{
		this->vertexPositionColor[i] = vertexPositionColor[i];
	}
}

void StandardCube::create_mesh_color_index()
{
	this->meshColorIndex = new unsigned short[36];
	//clockwise
	unsigned short meshIndex[36] = {
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	for (int i = 0; i < 36; i++)
	{
		this->meshColorIndex[i] = meshIndex[i];
	}
}

void StandardCube::create_texture_mesh()
{
	float x = m_position.x;
	float y = m_position.y;
	float z = m_position.z;

	VertexPositionTexture vertexPositionTexture[24] =
	{
		{ XMFLOAT3(-a, a, -a), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // +Y (top face)
		{ XMFLOAT3(a, a, -a), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(a, a, a), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-a, a, a), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-a, -a, a), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // -Y (bottom face)
		{ XMFLOAT3(a, -a, a), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(a, -a, -a), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-a, -a, -a), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(a, a, a), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // +X (right face)
		{ XMFLOAT3(a, a, -a), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(a, -a, -a), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(a, -a, a), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-a, a, -a), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // -X (left face)
		{ XMFLOAT3(-a, a, a), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-a, -a, a), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-a, -a, -a), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-a, a, a), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }, // +Z (front face)
		{ XMFLOAT3(a, a, a), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(a, -a, a), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-a, -a, a), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(a, a, -a), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }, // -Z (back face)
		{ XMFLOAT3(-a, a, -a), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-a, -a, -a), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(a, -a, -a), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	};
	this->vertexPositionTexture = new VertexPositionTexture[24];
	for (int i = 0; i < 24; i++)
	{
		this->vertexPositionTexture[i] = vertexPositionTexture[i];
	}
}

void StandardCube::create_mesh_texture_index()
{
	this->meshTextureIndex = new unsigned short[36];
	
	unsigned short meshIndex[36] = {
		0, 2, 1,
		0, 3, 2,

		4, 6, 5,
		4, 7, 6,

		8, 10, 9,
		8, 11, 10,

		12, 14, 13,
		12, 15, 14,

		16, 18, 17,
		16, 19, 18,

		20, 22, 21,
		20, 23, 22
		/*0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23*/
	};
	
	for (int i = 0; i < 36; i++)
	{
		this->meshTextureIndex[i] = meshIndex[i];
	}

}
