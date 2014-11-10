#include "pch.h"
#include "PhysicsEngineMain.h"
#include "Common\DirectXHelper.h"
#include "StandardSphere.h"
#include "StandardEllipsoid.h"
#include "StandardCube.h"
#include "StandardTriangle.h"
#include "SolarSystem.h"

using namespace PhysicsEngine;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
PhysicsEngineMain::PhysicsEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocationX(0.0f)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	//m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));
	
	this->InitilizeModels();
	this->InitilizeRenderers();

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

PhysicsEngineMain::~PhysicsEngineMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

void PhysicsEngineMain::InitilizeModels()
{
	/*Geometry^ ss1 = ref new StandardEllipsoid(80.0, 80.0, 80.0, 100, 100, DirectX::XMFLOAT3(0.0, 0.0, 0.0));
	this->modelBundle.push_back(ss1);*/
	/*Geometry^ ss2 = ref new StandardEllipsoid(0.5, 0.5, 0.5, 100, 100, DirectX::XMFLOAT3(3.0, 0.0, 0.0));
	this->modelBundle.push_back(ss2);*/
	SolarSystem^ ss = ref new SolarSystem();
	this->modelBundle = ss->modelBundle;
}

void PhysicsEngineMain::InitilizeRenderers()
{
	for (auto it = this->modelBundle.begin(); it != this->modelBundle.end(); ++it)
	{
		this->rendererBundle.push_back(std::unique_ptr<PhysicsTextureSceneRenderer>(new PhysicsTextureSceneRenderer(m_deviceResources, *it)));
	}
}

// Updates application state when the window size changes (e.g. device orientation change)
void PhysicsEngineMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	//m_sceneRenderer->CreateWindowSizeDependentResources();
	for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
		(*it)->CreateWindowSizeDependentResources();
}

void PhysicsEngineMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void PhysicsEngineMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void PhysicsEngineMain::Update() 
{
	ProcessInput();

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		//m_sceneRenderer->Update(m_timer);
		for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
			(*it)->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

// Process all input from the user before updating game state
void PhysicsEngineMain::ProcessInput()
{
	// TODO: Add per frame input handling here.
	//m_sceneRenderer->TrackingUpdate(m_pointerLocationX);
	for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
		(*it)->TrackingUpdate(m_pointerLocationX, m_pointerLocationY);
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool PhysicsEngineMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	//m_sceneRenderer->Render();
	for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
		(*it)->Render();
	m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void PhysicsEngineMain::OnDeviceLost()
{
	//m_sceneRenderer->ReleaseDeviceDependentResources();
	for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
		(*it)->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void PhysicsEngineMain::OnDeviceRestored()
{
	//m_sceneRenderer->CreateDeviceDependentResources();
	for (auto it = this->rendererBundle.begin(); it != this->rendererBundle.end(); ++it)
		(*it)->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
