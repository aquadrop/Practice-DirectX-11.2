#include "pch.h"
#include "StandardTriangle.h"

using namespace PhysicsEngine;
using namespace DirectX;

StandardTriangle::StandardTriangle(DirectX::XMFLOAT3 position)
{
	this->set_n_mesh();
	this->set_n_color_vertex();
	this->set_n_texture_vertex();

	this->Position(position);

	this->create_color_mesh();
	this->create_mesh_color_index();

	this->create_texture_mesh();
	this->create_mesh_texture_index();

}

void StandardTriangle::set_n_mesh()
{
	this->n_mesh = 3;//indexCount
}

void StandardTriangle::set_n_color_vertex()
{
	this->n_color_vertex = 3;
}

void StandardTriangle::set_n_texture_vertex()
{
	this->n_texture_vertex = 3;
}

void StandardTriangle::create_color_mesh()
{
	VertexPositionColor vertexPositionColor[] =
	{
		{ XMFLOAT3(-1.5, -0.5*sqrtf(3), 0.0f), XMFLOAT3(0.1f, 0.5f, 0.4f)},
		{ XMFLOAT3(0.0f, sqrtf(3), 0.0f), XMFLOAT3(0.5f, 0.2f, 0.4f) },
		{ XMFLOAT3(1.5, -0.5*sqrtf(3), 0.0f), XMFLOAT3(0.8f, 0.1f, 0.4f)},
	};
	this->vertexPositionColor = new VertexPositionColor[8];
	for (int i = 0; i < 3; i++)
	{
		this->vertexPositionColor[i] = vertexPositionColor[i];
	}
}

void StandardTriangle::create_mesh_color_index()
{
	this->meshColorIndex = new unsigned short[36];
	//clockwise
	unsigned short meshIndex[3] = {
		0, 1, 2,
	};

	for (int i = 0; i < 3; i++)
	{
		this->meshColorIndex[i] = meshIndex[i];
	}
}

void StandardTriangle::create_texture_mesh()
{
	VertexPositionTexture vertexPositionTexture[] =
	{
		{ XMFLOAT3(-1.5, -0.5*sqrtf(3), 0.0f), XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT2(0.0, 1.0) },
		{ XMFLOAT3(0.0f, sqrtf(3), 0.0f), XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT2(0.5, 1 - 0.5*sqrtf(3)) },
		{ XMFLOAT3(1.5, -0.5*sqrtf(3), 0.0f), XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT2(1.0, 1.0) },
	};
	this->vertexPositionTexture = new VertexPositionTexture[3];
	for (int i = 0; i < 3; i++)
	{
		this->vertexPositionTexture[i] = vertexPositionTexture[i];
	}
}

void StandardTriangle::create_mesh_texture_index()
{
	this->meshTextureIndex = new unsigned short[3];

	unsigned short meshIndex[] = {
		0, 1, 2,
	};

	for (int i = 0; i < 3; i++)
	{
		this->meshTextureIndex[i] = meshIndex[i];
	}

}
