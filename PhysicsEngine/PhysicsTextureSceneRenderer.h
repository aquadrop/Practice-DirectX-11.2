#pragma once

#include "Common\DeviceResources.h"
#include "Content\ShaderStructures.h"
#include "Common\StepTimer.h"
#include "Mass.h"

namespace PhysicsEngine
{
	// This sample renderer instantiates a basic rendering pipeline.
	class PhysicsTextureSceneRenderer
	{
	public:
		PhysicsTextureSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, Geometry^ model);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void StartTracking();
		void TrackingUpdate(float positionX, float positionY);
		void StopTracking();
		bool IsTracking() { return m_tracking; }


	private:
		void Rotate(float spin_radians, float rotate_radians);

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_texture;                    // geometry texture
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_textureSRV;                 // geometry texture view
		Microsoft::WRL::ComPtr<ID3D11SamplerState>          m_sampler;                    // geometry texture sampler

		// System resources for cube geometry.
		//ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		bool	m_tracking;

	private:
		Geometry^ model;
	};
}

