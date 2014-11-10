#pragma once
#include "pch.h"
#include "Content\ShaderStructures.h"
#include "Mass.h"

namespace PhysicsEngine
{
	ref class StandardTriangle : Geometry
	{
	internal:

		StandardTriangle::StandardTriangle(DirectX::XMFLOAT3 position);

		void create_color_mesh() override;
		void create_mesh_color_index() override;

		void create_texture_mesh() override;
		void create_mesh_texture_index() override;

		void initiate_mesh_speed(){}
		void update_mesh_speed()
		{

		}
		double a = 0.5;
		double b = 0.5;

	protected:
		void set_n_color_vertex() override;
		void set_n_texture_vertex() override;
		void set_n_mesh() override;
	};

}