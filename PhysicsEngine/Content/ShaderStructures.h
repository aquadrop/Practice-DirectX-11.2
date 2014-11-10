#pragma once

namespace PhysicsEngine
{
	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// Used to send per-vertex data to the vertex shader.
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	struct VertexPositionTexture
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 norm;
		DirectX::XMFLOAT2 tex;
	};

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		DirectX::XMFLOAT3 dir;
		float pad1;
		///////////////**************new**************////////////////////
		DirectX::XMFLOAT3 pos;
		float range;
		DirectX::XMFLOAT3 att;
		float pad2;
		///////////////**************new**************////////////////////
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
	};

}