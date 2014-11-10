#pragma once
#include "pch.h"
#include "Content\ShaderStructures.h"
#include "Mass.h"

namespace PhysicsEngine
{
	ref class StandardEllipsoid : Geometry
	{
	internal:

		StandardEllipsoid::StandardEllipsoid(double a, double b, double c, int n_latitude, int n_longitude, DirectX::XMFLOAT3 position);
		void create_color_mesh() override;
		void create_mesh_color_index() override;

		void create_texture_mesh() override;
		void create_mesh_texture_index() override;

		//void Spin(ModelViewProjectionConstantBuffer &constantBufferData, double radians) override;
		void initiate_mesh_speed(){}
		void update_mesh_speed()
		{

		}
		double a = 0.5;
		double b = 0.8;
		double c = 0.7;

	protected private:
		int n_latitude;
		int n_longitude;
	private:
		void get_n_vertex();
		void get_n_mesh();
		int get_vertex_index(int latitude, int longitude);
		DirectX::XMFLOAT3 get_random_color();
		float randu();

	protected:
		void set_n_color_vertex() override;
		void set_n_texture_vertex() override;
		void set_n_mesh() override;
	};

}