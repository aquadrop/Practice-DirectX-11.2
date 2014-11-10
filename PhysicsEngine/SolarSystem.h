#pragma once
#include "pch.h"
#include "Content\ShaderStructures.h"
#include "Mass.h"
#include "StandardSphere.h"

namespace PhysicsEngine
{
	ref class SolarSystem
	{
	internal:
		SolarSystem::SolarSystem();
		
		std::vector<Geometry^> modelBundle;
	private:
		StandardSphere^ shell;
		StandardSphere^ sun;
		StandardSphere^ mercury;
		StandardSphere^ venus;
		StandardSphere^ earth;
		StandardSphere^ moon;
		StandardSphere^ moon_satellite;

		StandardSphere^ jupiter;
		StandardSphere^ jupiter_s1;
		StandardSphere^ jupiter_s2;
		StandardSphere^ jupiter_s3;

	private:
		void Initiate();

	};

}