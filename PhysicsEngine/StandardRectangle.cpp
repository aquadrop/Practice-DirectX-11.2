#include "pch.h"
#include "StandardRectangle.h"

using namespace PhysicsEngine;
using namespace DirectX;

StandardRectangle::StandardRectangle(double a, double b, DirectX::XMFLOAT3 position)
{
	this->a = a;
	this->b = b;
	this->set_n_mesh();
	this->set_n_color_vertex();
	this->set_n_texture_vertex();

	this->Position(position);

	this->create_color_mesh();
	this->create_mesh_color_index();

	this->create_texture_mesh();
	this->create_mesh_texture_index();

}

void StandardRectangle::set_n_mesh()
{
	this->n_mesh = 3;//indexCount
}

void StandardRectangle::set_n_color_vertex()
{
	this->n_color_vertex = 4;
}

void StandardRectangle::set_n_texture_vertex()
{
	this->n_texture_vertex = 3;
}

void StandardRectangle::create_color_mesh()
{
	float x = m_position.x;
	float y = m_position.y;
	float z = m_position.z;

	VertexPositionColor vertexPositionColor[] =
	{
		{ XMFLOAT3(-0.5f + x, -0.5f + y, 0.5f + z), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f + x, 0.5f + y, 0.5f + z), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f + x, -0.5f + y, 0.5f + z), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f + x, 0.5f + y, 0.5f + z), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};
	this->vertexPositionColor = new VertexPositionColor[8];
	for (int i = 0; i < 4; i++)
	{
		this->vertexPositionColor[i] = vertexPositionColor[i];
	}
}

void StandardRectangle::create_mesh_color_index()
{
	this->meshColorIndex = new unsigned short[36];
	//clockwise
	unsigned short meshIndex[6] = {
		0, 1, 3, // +z
		0, 3, 2,
	};

	for (int i = 0; i < 6; i++)
	{
		this->meshColorIndex[i] = meshIndex[i];
	}
}

void StandardRectangle::create_texture_mesh()
{
	float x = m_position.x;
	float y = m_position.y;
	float z = m_position.z;

	VertexPositionTexture vertexPositionTexture[] =
	{
		{ XMFLOAT3(-1.5 + x, -0.5*sqrtf(3) + y, 0.0f + z), XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT2(0.0, 1.0) },
		{ XMFLOAT3(0.0f + x, sqrtf(3) + y, 0.0f + z), XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT2(0.5, 1-0.5*sqrtf(3)) },
		{ XMFLOAT3(1.5 + x, -0.5*sqrtf(3) + y, 0.0f + z), XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT2(1.0, 1.0) },
		

		/*{ XMFLOAT3(-0.5f + x, -0.5f + y, 0.0f + z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0, 1.0) },
		{ XMFLOAT3(-0.5f + x, 0.5f + y, 0.0f + z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0, 0.0) },
		{ XMFLOAT3(0.5f + x, 0.5f + y, 0.0f + z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0, 0.0) },*/


		/*{ XMFLOAT3(-0.5f + x, -0.5f + y, 0.5f + z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0, 0.0) },
		{ XMFLOAT3(0.5f + x, 0.5f + y, 0.5f + z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0, 1.0) },
		{ XMFLOAT3(0.5f + x, -0.5f + y, 0.5f + z), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0, 1.0) },*/
	};
	this->vertexPositionTexture = new VertexPositionTexture[3];
	for (int i = 0; i < 3; i++)
	{
		this->vertexPositionTexture[i] = vertexPositionTexture[i];
	}
}

void StandardRectangle::create_mesh_texture_index()
{
	this->meshTextureIndex = new unsigned short[3];

	unsigned short meshIndex[] = {
		0, 1, 2,
		/*3, 4, 5*/
	};

	for (int i = 0; i < 3; i++)
	{
		this->meshTextureIndex[i] = meshIndex[i];
	}

}
