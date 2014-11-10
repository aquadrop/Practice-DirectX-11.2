#pragma once
#include "pch.h"
#include "Content\ShaderStructures.h"
#include "StandardEllipsoid.h"
#include "Mass.h"

namespace PhysicsEngine
{
	ref class StandardSphere : StandardEllipsoid
	{
	internal:

		StandardSphere::StandardSphere(double radius, int n_latitude, int n_longitude, DirectX::XMFLOAT3 position);
		double radius = 0.5;
	};

}