#include "pch.h"
#include "StandardSphere.h"

using namespace PhysicsEngine;
using namespace DirectX;

StandardSphere::StandardSphere(double radius, int n_latitude, int n_longitude, DirectX::XMFLOAT3 position) : 
StandardEllipsoid(radius, radius, radius, n_latitude, n_longitude, position)
{
	this->radius = radius;

}