#include "pch.h"
#include "PhysicsTextureSceneRenderer.h"
#include "BasicLoader.h"
#include "..\Common\DirectXHelper.h"

using namespace PhysicsEngine;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
PhysicsTextureSceneRenderer::PhysicsTextureSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, Geometry^ model) :
m_loadingComplete(false),
m_indexCount(0),
m_tracking(false),
model(model),
m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void PhysicsTextureSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&this->model->m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 2.7f, 30.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&this->model->m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	XMStoreFloat4x4(&this->model->m_constantBufferData.model, XMMatrixIdentity());
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void PhysicsTextureSceneRenderer::Update(DX::StepTimer const& timer)
{
	//if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(this->model->m_rotate_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float rotate_radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		radiansPerSecond = XMConvertToRadians(this->model->m_spin_degreesPerSecond);
		totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float spin_radians = static_cast<float>(fmod(totalRotation, XM_2PI));
		Rotate(spin_radians, rotate_radians);
	}
}

// Rotate the 3D cube model a set amount of radians.
void PhysicsTextureSceneRenderer::Rotate(float spin_radians, float rotate_radians)
{
	// Prepare to pass the updated model matrix to the shader
	//XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationZ(radians)));
	this->model->Rotate(this->model->m_constantBufferData, spin_radians, rotate_radians);
}

void PhysicsTextureSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void PhysicsTextureSceneRenderer::TrackingUpdate(float positionX, float positionY)
{
	if (m_tracking)
	{
		XMVECTOR view_translation_core = XMMatrixTranspose(XMLoadFloat4x4(&this->model->m_constantBufferData.view)).r[3];
		float radians_Y = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		float radians_X = XM_2PI * 2.0f * positionY / m_deviceResources->GetOutputSize().Height;
		XMMATRIX m = XMMatrixRotationY(radians_Y);
		m = XMMatrixMultiply(m, XMMatrixRotationX(radians_X));
		m.r[3] = view_translation_core;
		XMStoreFloat4x4(&this->model->m_constantBufferData.view, XMMatrixTranspose(m));
	}
}

void PhysicsTextureSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void PhysicsTextureSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Set render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&this->model->m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionTexture);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	context->PSSetShaderResources(
		0,
		1,
		m_textureSRV.GetAddressOf()
		);

	context->PSSetSamplers(
		0,                          // starting at the first sampler slot
		1,                          // set one sampler binding
		m_sampler.GetAddressOf()
		);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}

void PhysicsTextureSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"TextureVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"TexturePixelShader.cso");
	BasicLoader^ loader = ref new BasicLoader(m_deviceResources->GetD3DDevice());

	loader->LoadTexture(
		this->model->textureFileName,
		&m_texture,
		&m_textureSRV
		);

	// create the sampler
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	// use 4x on feature level 9.2 and above, otherwise use only 2x
	samplerDesc.MaxAnisotropy = 8;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	// allow use of all mip levels
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateSamplerState(
		&samplerDesc,
		&m_sampler)
		);

	ID3D11RasterizerState* noCull;
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;

	m_deviceResources->GetD3DDevice()->CreateRasterizerState(&rastDesc, &noCull);

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
			&fileData[0],
			fileData.size(),
			nullptr,
			&m_vertexShader
			)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			&fileData[0],
			fileData.size(),
			&m_inputLayout
			)
			);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
			&fileData[0],
			fileData.size(),
			nullptr,
			&m_pixelShader
			)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&m_constantBuffer
			)
			);
	});

	

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {

		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionColor cubeVertices[] =
		{
			{ XMFLOAT3(-1.0f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(0.0f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(0.0f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(0.0f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = this->model->getVertexPositionTexture();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(this->model->getVertexPositionTexture()[0])*this->model->n_texture_vertex, D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
			)
			);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.
		static const unsigned short cubeIndices[] =
		{
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

		m_indexCount = this->model->n_mesh;//ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = this->model->getMeshTextureIndex();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(this->model->getMeshTextureIndex()[0])*m_indexCount, D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
			)
			);
	});

	// Once the cube is loaded, the object is ready to be rendered.
	createCubeTask.then([this]() {
		m_loadingComplete = true;
	});
}

void PhysicsTextureSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}