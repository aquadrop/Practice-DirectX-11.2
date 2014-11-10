#include "pch.h"

#include "SolarSystem.h"

using namespace PhysicsEngine;
using namespace DirectX;

SolarSystem::SolarSystem()
{
	this->Initiate();
}

void SolarSystem::Initiate()
{
	this->shell = ref new StandardSphere(500, 100, 100, DirectX::XMFLOAT3(0.0, 0.0, 0.0));
	this->shell->rotation_reference_type = ROTATION_REFERENCE_ORIGIN;
	this->shell->m_rotate_degreesPerSecond = 2.0;
	this->shell->textureFileName = L"Assets/SolarSystem/universe.dds";
	this->modelBundle.push_back(shell);

	this->sun = ref new StandardSphere(4, 100, 100, DirectX::XMFLOAT3(0.0, 0.0, 0.0));
	this->sun->textureFileName = L"Assets/SolarSystem/sun.dds";
	this->sun->rotation_reference_type = ROTATION_REFERENCE_ORIGIN;
	this->modelBundle.push_back(sun);

	this->mercury = ref new StandardSphere(0.3, 100, 100, DirectX::XMFLOAT3(4.5, 0.0, 0.0));
	this->mercury->rotation_reference_type = ROTATION_REFERENCE_ROTATE_Y_WITHOUT_REFERENCE_AND_SPIN_Y;
	this->mercury->m_rotate_degreesPerSecond = 225.0;
	this->mercury->textureFileName = L"Assets/SolarSystem/mercury.dds";
	this->modelBundle.push_back(mercury);

	this->venus = ref new StandardSphere(0.8, 100, 100, DirectX::XMFLOAT3(7.5, 0.0, 0.0));
	this->venus->textureFileName = L"Assets/SolarSystem/venus.dds";
	this->venus->rotation_reference_type = ROTATION_REFERENCE_ROTATE_Y_WITHOUT_REFERENCE_AND_SPIN_Y;
	this->venus->m_rotate_degreesPerSecond = 100.0;
	this->venus->m_spin_degreesPerSecond = 0.0;
	this->modelBundle.push_back(venus);

	this->earth = ref new StandardSphere(1.5, 100, 100, DirectX::XMFLOAT3(12.5, 0.0, 0.0));
	this->earth->rotation_reference_type = ROTATION_REFERENCE_ROTATE_ONLY_WITHOUT_POSITION_REFERENCE;
	this->earth->textureFileName = L"Assets/SolarSystem/earth.dds";
	this->earth->m_rotate_angle = { XM_PIDIV4/4, XM_PIDIV4 / 4 };
	this->earth->m_rotate_degreesPerSecond = 45.0f;
	this->earth->m_spin_degreesPerSecond = 20.0;
	this->modelBundle.push_back(earth);

	this->moon = ref new StandardSphere(0.3, 100, 100, DirectX::XMFLOAT3(15.3, 0.0, 0.0));
	this->moon->reference = this->earth;
	this->moon->textureFileName = L"Assets/SolarSystem/moon.dds";
	this->moon->m_spin_degreesPerSecond = 0.0;
	this->moon->m_rotate_degreesPerSecond = 45 - this->moon->reference->m_spin_degreesPerSecond;
	this->moon->m_rotate_position = DirectX::XMFLOAT3(12.5, 0.0, 0.0);
	this->modelBundle.push_back(moon);

	this->moon_satellite = ref new StandardSphere(0.2, 4, 4, DirectX::XMFLOAT3(16.3, 0.0, 0.0));
	this->moon_satellite->reference = this->moon;
	this->moon_satellite->textureFileName = L"Assets/SolarSystem/moon.dds";
	this->moon_satellite->m_spin_degreesPerSecond = 0.0;
	this->moon_satellite->m_rotate_degreesPerSecond = 450;
	this->moon_satellite->m_rotate_position = DirectX::XMFLOAT3(14.9, 0.0, 0.0);
	this->modelBundle.push_back(moon_satellite);

	this->jupiter = ref new StandardSphere(2, 100, 100, DirectX::XMFLOAT3(19.3, 0.0, 0.0));
	this->venus->rotation_reference_type = ROTATION_REFERENCE_ROTATE_Y_WITHOUT_REFERENCE_AND_SPIN_Y;
	this->jupiter->textureFileName = L"Assets/SolarSystem/jupiter.dds";
	this->jupiter->m_spin_degreesPerSecond = 100.0;
	this->jupiter->m_rotate_degreesPerSecond = 15;
	this->modelBundle.push_back(jupiter);

	this->jupiter_s1 = ref new StandardSphere(1.0, 100, 100, DirectX::XMFLOAT3(23.3, 0.0, 0.0));
	this->jupiter_s1->reference = this->jupiter;
	this->jupiter_s1->textureFileName = L"Assets/SolarSystem/jupiter_s1.dds";
	this->jupiter_s1->m_spin_degreesPerSecond = 0.0;
	this->jupiter_s1->m_rotate_degreesPerSecond = 45 - this->jupiter_s1->reference->m_spin_degreesPerSecond;
	this->jupiter_s1->m_rotate_position = DirectX::XMFLOAT3(19.3, 0.0, 0.0);
	this->modelBundle.push_back(jupiter_s1);

	this->jupiter_s2 = ref new StandardSphere(0.3, 100, 100, DirectX::XMFLOAT3(24.9, 0.0, 0.0));
	this->jupiter_s2->reference = this->jupiter;
	this->jupiter_s2->textureFileName = L"Assets/SolarSystem/jupiter_s1.dds";
	this->jupiter_s2->m_spin_degreesPerSecond = 0.0;
	this->jupiter_s2->m_rotate_degreesPerSecond = 40 - this->jupiter_s2->reference->m_spin_degreesPerSecond;
	this->jupiter_s2->m_rotate_position = DirectX::XMFLOAT3(19.3, 0.0, 0.0);
	this->modelBundle.push_back(jupiter_s2);

	this->jupiter_s3 = ref new StandardSphere(0.3, 100, 100, DirectX::XMFLOAT3(25.8, 0.0, 0.0));
	this->jupiter_s3->reference = this->jupiter;
	this->jupiter_s3->textureFileName = L"Assets/SolarSystem/jupiter_s1.dds";
	this->jupiter_s3->m_spin_degreesPerSecond = 0.0;
	this->jupiter_s3->m_rotate_degreesPerSecond = 35 - this->jupiter_s3->reference->m_spin_degreesPerSecond;
	this->jupiter_s3->m_rotate_position = DirectX::XMFLOAT3(19.3, 0.0, 0.0);
	this->modelBundle.push_back(jupiter_s3);
}