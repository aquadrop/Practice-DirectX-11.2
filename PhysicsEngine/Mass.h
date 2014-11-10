#pragma once
#include "pch.h"
#include "Content\ShaderStructures.h"

using namespace PhysicsEngine;
using namespace DirectX;

namespace PhysicsEngine
{
	enum ROTATION_REFERENCE_TYPE{
		ROTATION_REFERENCE_ORIGIN = 0,
		ROTATION_REFERENCE_SPIN_ONLY = 0x1,
		ROTATION_REFERENCE_SPIN_ONLY_X = 0x4,
		ROTATION_REFERENCE_SPIN_ONLY_Y = 0x5,
		ROTATION_REFERENCE_SPIN_ONLY_Z = 0x10,

		ROTATION_REFERENCE_ROTATE_ONLY_WITH_POSITION_REFERENCE = 0x11,
		ROTATION_REFERENCE_ROTATE_ONLY_WITHOUT_POSITION_REFERENCE = 0x14,
		ROTATION_REFERENCE_ROTATE_X_ONLY_WITHOUT_REFERENCE = 0x15,
		ROTATION_REFERENCE_ROTATE_Y_ONLY_WITHOUT_REFERENCE = 0x55,
		ROTATION_REFERENCE_ROTATE_Z_ONLY_WITHOUT_REFERENCE = 0x80,

		ROTATION_REFERENCE_ROTATE_Y_WITHOUT_REFERENCE_AND_SPIN_Y = 0X85,

		ROTATION_REFERENCE_GENERAL = 0x1d5
	};

	enum INVERSE_TYPE{
		INVERSE_TYPE_OUTSIDE = 0,
		INVERSE_TYPE_INSIDE = 0x1
	};
	ref class Geometry
	{
	internal:

		Geometry(){}
		int n_color_vertex;
		int n_texture_vertex;
		int n_mesh;
		double mass;

		ModelViewProjectionConstantBuffer	m_constantBufferData;

		Platform::String^ textureFileName = L"Assets/reftexture.dds";

		int rotation_reference_type = ROTATION_REFERENCE_GENERAL;
		int inverse_type = INVERSE_TYPE_OUTSIDE;

		virtual void create_color_mesh() {}
		virtual void create_mesh_color_index() {}

		virtual void create_texture_mesh() {}
		virtual void create_mesh_texture_index(){}

		void Position(DirectX::XMFLOAT3 position){ m_position = position; }
		void Position(DirectX::XMVECTOR position);
		void Velocity(DirectX::XMFLOAT3 velocity);
		void Velocity(DirectX::XMVECTOR velocity);

		Geometry^ reference;
		//in reference mode, the m_rotate_degreesPerSecond is reference to the reference object's spin rate
		double m_spin_degreesPerSecond = 200;
		double m_rotate_degreesPerSecond = 45;
		//reference spin vector is (0, 1, 0)
		DirectX::XMFLOAT2 m_spin_angle = { 0.0f, 0.0f }; //clockwise on theta, clockwise on phi{theta, phi}//rotate x theta;then rotate y phi
		DirectX::XMFLOAT2 m_rotate_angle = { 0.0f, 0.0f }; //counterclockwise on theta, counterclockwise on phi{theta, phi}//rotate x theta;then rotate y phi
		DirectX::XMFLOAT3 m_rotate_position = { 0.0f, 0.0f, 0.0f };
		void setSpinRate(double degreesPerSecond) { this->m_spin_degreesPerSecond = degreesPerSecond; }
		void setRotateRate(double degreesPerSecond) { this->m_rotate_degreesPerSecond = degreesPerSecond; }
		void setSpinAngle(DirectX::XMFLOAT2 spin_angle){ this->m_spin_angle = spin_angle; }
		void setRotateAngle(DirectX::XMFLOAT2 rotate_angle){ this->m_rotate_angle = rotate_angle; }
		void setRotatePosition(DirectX::XMFLOAT3 rotate_position){ this->m_rotate_position = rotate_position; }

		virtual void Rotate(ModelViewProjectionConstantBuffer &constantBufferData, double spin_radians, double rotate_radians)
		{
			XMMATRIX m;
			if (this->rotation_reference_type == ROTATION_REFERENCE_GENERAL)
			{
				if (!this->reference)
				{
					XMMATRIX a = this->Spin(spin_radians, m_spin_angle);
					XMMATRIX b = this->Rotate(rotate_radians, m_rotate_angle, m_rotate_position);
					m = XMMatrixMultiply(a, b);
				}
				else
				{
					//transform to the reference coordinates and than rotate and spin R2->R0->R1->Rotate->R0->R2
					XMMATRIX M2 = XMMatrixTranspose(DirectX::XMLoadFloat4x4(&this->reference->m_constantBufferData.model));
					XMMATRIX R = XMMatrixMultiply(this->Spin(spin_radians, m_spin_angle), this->Rotate(rotate_radians, m_rotate_angle, m_rotate_position));

					m = XMMatrixMultiply(R, M2);
				}
				
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_ROTATE_Y_WITHOUT_REFERENCE_AND_SPIN_Y)
			{
				XMMATRIX a = this->SpinY(spin_radians);
				XMMATRIX b = XMMatrixRotationY(rotate_radians);
				m = XMMatrixMultiply(a, b);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_ROTATE_ONLY_WITH_POSITION_REFERENCE)
			{
				m = this->Rotate(rotate_radians, m_rotate_angle, m_rotate_position);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_ROTATE_ONLY_WITHOUT_POSITION_REFERENCE)
			{
				m= this->Rotate(rotate_radians, m_rotate_angle);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_ROTATE_X_ONLY_WITHOUT_REFERENCE)
			{
				m = XMMatrixRotationX(rotate_radians);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_ROTATE_Y_ONLY_WITHOUT_REFERENCE || this->rotation_reference_type == ROTATION_REFERENCE_ORIGIN)
			{
				m = XMMatrixRotationY(rotate_radians);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_ROTATE_Z_ONLY_WITHOUT_REFERENCE)
			{
				m = XMMatrixRotationZ(rotate_radians);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_SPIN_ONLY)
			{
				m = this->Spin(spin_radians, m_spin_angle);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_SPIN_ONLY_X)
			{
				m = this->SpinX(spin_radians);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_SPIN_ONLY_Y)
			{
				m = this->SpinY(spin_radians);
			}
			else if (this->rotation_reference_type == ROTATION_REFERENCE_SPIN_ONLY_Z)
			{
				m = this->SpinZ(spin_radians);
			}
			else
			{
				m = XMMatrixRotationX(0);
			}
			XMStoreFloat4x4(&constantBufferData.model, XMMatrixTranspose(m));
		}

		void setTextureFilename(Platform::String^ filename) { this->textureFileName = textureFileName; }
		VertexPositionColor* getVertexPositionColor(){ return this->vertexPositionColor; }
		VertexPositionTexture* getVertexPositionTexture(){ return this->vertexPositionTexture; }
		unsigned short* getMeshColorIndex(){ return this->meshColorIndex; }
		unsigned short* getMeshTextureIndex(){ return this->meshTextureIndex; }
	protected:
		virtual void set_n_color_vertex() {}
		virtual void set_n_texture_vertex() {}
		virtual void set_n_mesh() {}

	private protected:
		//Spin is rotate against the straight line(spin axis) passing the center of the object
		XMMATRIX SpinX(double radians)
		{
			XMMATRIX m = XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z);
			m = XMMatrixMultiply(m, XMMatrixRotationX(radians));
			m = XMMatrixMultiply(m, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
			return m;
		}

		XMMATRIX SpinY(double radians)
		{
			XMMATRIX m = XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z);
			m = XMMatrixMultiply(m, XMMatrixRotationY(radians));
			m = XMMatrixMultiply(m, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
			return m;
		}

		XMMATRIX SpinZ(double radians)
		{
			XMMATRIX m = XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z);
			m = XMMatrixMultiply(m, XMMatrixRotationZ(radians));
			m = XMMatrixMultiply(m, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
			return m;
		}

		//Spin is rotate against the straight line(spin axis) passing the center of the object
		XMMATRIX Spin(double radians, DirectX::XMFLOAT2 spin_angle)
		{
			XMMATRIX m = XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z);
			m = XMMatrixMultiply(m, XMMatrixRotationX(spin_angle.x));
			m = XMMatrixMultiply(m, XMMatrixRotationY(spin_angle.y));
			m = XMMatrixMultiply(m, XMMatrixRotationY(radians));
			m = XMMatrixMultiply(m, XMMatrixRotationY(-spin_angle.y));
			m = XMMatrixMultiply(m, XMMatrixRotationX(-spin_angle.x));
			m = XMMatrixMultiply(m, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
			return m;
		}

		//Rotate is general spin, against any straight line(rotate axis)
		XMMATRIX Rotate(double radians, DirectX::XMFLOAT2 rotate_angle)
		{
			XMMATRIX m = XMMatrixRotationX(rotate_angle.x);
			m = XMMatrixMultiply(m, XMMatrixRotationY(rotate_angle.y));
			m = XMMatrixMultiply(m, XMMatrixRotationY(radians));
			m = XMMatrixMultiply(m, XMMatrixRotationY(-rotate_angle.y));
			m = XMMatrixMultiply(m, XMMatrixRotationX(-rotate_angle.x));
			return m;
		}

		XMMATRIX Rotate(double radians, DirectX::XMFLOAT2 rotate_angle, DirectX::XMFLOAT3 rotate_position)
		{
			XMMATRIX m = XMMatrixTranslation(-rotate_position.x, -rotate_position.y, -rotate_position.z);
			m = XMMatrixMultiply(m, XMMatrixRotationX(rotate_angle.x));
			m = XMMatrixMultiply(m, XMMatrixRotationY(rotate_angle.y));
			m = XMMatrixMultiply(m, XMMatrixRotationY(radians));
			m = XMMatrixMultiply(m, XMMatrixRotationY(-rotate_angle.y));
			m = XMMatrixMultiply(m, XMMatrixRotationX(-rotate_angle.x));
			m = XMMatrixMultiply(m, XMMatrixTranslation(rotate_position.x, rotate_position.y, rotate_position.z));
			return m;
		}

	protected private:

		VertexPositionColor* vertexPositionColor;
		VertexPositionTexture* vertexPositionTexture;
		unsigned short* meshColorIndex;
		unsigned short* meshTextureIndex;
		DirectX::XMFLOAT3   m_position;
		DirectX::XMFLOAT3   m_velocity;
		DirectX::XMFLOAT4X4 m_modelMatrix;

		DirectX::XMFLOAT3   m_defaultXAxis;
		DirectX::XMFLOAT3   m_defaultYAxis;
		DirectX::XMFLOAT3   m_defaultZAxis;


	};
}