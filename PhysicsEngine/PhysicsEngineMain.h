#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
//#include "Content\Sample3DSceneRenderer.h"
#include "Content\PhysicsSceneRenderer.h"
#include "PhysicsTextureSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace PhysicsEngine
{
	class PhysicsEngineMain : public DX::IDeviceNotify
	{
	public:
		PhysicsEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~PhysicsEngineMain();
		void CreateWindowSizeDependentResources();
		void StartTracking() 
		{ 
			//m_sceneRenderer->StartTracking(); 
			for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
				(*it)->StartTracking();
		}
		void TrackingUpdate(float positionX, float positionY) 
		{
			if (m_initPointerLocationX == 0 && m_initPointerLocationY == 0)
			{
				m_initPointerLocationX = positionX;
				m_initPointerLocationY = positionY;
			}
			m_pointerLocationX = positionX - m_initPointerLocationX;
			m_pointerLocationY = positionY - m_initPointerLocationY;
		}
		void StopTracking() 
		{ 
			//m_sceneRenderer->StopTracking(); 
			for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
				(*it)->StopTracking();
			m_initPointerLocationX = 0;
			m_initPointerLocationY = 0;
		}
		bool IsTracking() 
		{ 
			for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
				return (*it)->IsTracking();
		}
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		void InitilizeModels();
		void InitilizeRenderers();

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		//std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;
		float m_pointerLocationY;
		// Track initial input pointer position.
		float m_initPointerLocationX = 0;
		float m_initPointerLocationY = 0;


	private:
		std:: vector<Geometry^> modelBundle;
		std::vector<std::unique_ptr<PhysicsTextureSceneRenderer>> rendererBundle;
		//std::vector<std::unique_ptr<PhysicsTextureSceneRenderer>> textureRendererBundle;

	};
}