#include "pch.h"
#include "StandardEllipsoid.h"

using namespace PhysicsEngine;
using namespace DirectX;

StandardEllipsoid::StandardEllipsoid(double a, double b, double c, int n_latitude, int n_longitude, DirectX::XMFLOAT3 position)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->n_latitude = n_latitude;
	this->n_longitude = n_longitude;

	this->set_n_mesh();
	this->set_n_color_vertex();
	this->set_n_texture_vertex();

	this->Position(position);

	this->create_color_mesh();
	this->create_mesh_color_index();

	this->create_texture_mesh();
	this->create_mesh_texture_index();

}

//void StandardEllipsoid::Spin(ModelViewProjectionConstantBuffer &constantBufferData, double radians)
//{
//	XMStoreFloat4x4(&constantBufferData.view, XMMatrixTranspose(XMMatrixTranslation(m_position.x, m_position.y, m_position.z)));
//	XMStoreFloat4x4(&constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
//	XMStoreFloat4x4(&constantBufferData.view, XMMatrixTranspose(XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z)));
//}

void StandardEllipsoid::set_n_mesh()
{
	this->n_mesh = 3 * 2 * n_longitude * n_latitude;
}

void StandardEllipsoid::set_n_color_vertex()
{
	this->n_color_vertex = n_latitude * n_longitude + 2;
}

void StandardEllipsoid::set_n_texture_vertex()
{
	//longitude is half circle
	this->n_texture_vertex = (n_latitude * n_longitude + n_latitude + 2);
}

void StandardEllipsoid::create_color_mesh()
{
	this->vertexPositionColor = new VertexPositionColor[this->n_color_vertex];
	vertexPositionColor[0].pos = DirectX::XMFLOAT3(+this->m_position.x, this->m_position.y, c + this->m_position.z);
	vertexPositionColor[0].color = this->get_random_color();
	vertexPositionColor[this->n_color_vertex - 1].pos = DirectX::XMFLOAT3(+this->m_position.x, this->m_position.y, -c + this->m_position.z);
	vertexPositionColor[this->n_color_vertex - 1].color = this->get_random_color();
	int index = 1;
	for (int i = 1; i <= this->n_latitude; i++)
	{
		for (int j = 1; j <= this->n_longitude; j++)
		{
			float x = this->a*sin((float)i*DirectX::XM_PI / (this->n_latitude + 1)) * cos((float)(j - 1)*DirectX::XM_2PI / this->n_longitude) + this->m_position.x;
			float y = this->b*sin((float)i*DirectX::XM_PI / (this->n_latitude + 1)) * sin((float)(j - 1)*DirectX::XM_2PI / this->n_longitude) + this->m_position.y;
			float z = this->c*cos((float)i*DirectX::XM_PI / (this->n_latitude + 1)) + this->m_position.z;
			vertexPositionColor[index].pos = DirectX::XMFLOAT3(x, y, z);
			vertexPositionColor[index].color = DirectX::XMFLOAT3(sin(x), sin(y), sin(z));
			index++;
		}
	}
}

void StandardEllipsoid::create_mesh_color_index()
{
	std::vector <int> _indices;

	//north pole
	for (int j = 1; j <= this->n_longitude - 1; j++)
	{
		_indices.push_back(0);
		_indices.push_back(j);
		_indices.push_back(j + 1);
	}

	//close the texute
	_indices.push_back(0);
	_indices.push_back(this->n_longitude);
	//this is the extension
	_indices.push_back(this->n_longitude + 1);

	//core grid
	for (int i = 1; i <= this->n_latitude - 1; i++)
	{
		for (int j = 1; j <= this->n_longitude - 1; j++)
		{
			_indices.push_back(this->get_vertex_index(i, j));
			_indices.push_back(this->get_vertex_index(i + 1, j));
			_indices.push_back(this->get_vertex_index(i, j + 1));

			_indices.push_back(this->get_vertex_index(i, j + 1));
			_indices.push_back(this->get_vertex_index(i + 1, j));
			_indices.push_back(this->get_vertex_index(i + 1, j + 1));
		}

		_indices.push_back(this->get_vertex_index(i, this->n_longitude));
		_indices.push_back(this->get_vertex_index(i + 1, this->n_longitude));
		_indices.push_back(this->get_vertex_index(i, this->n_longitude + 1));

		_indices.push_back(this->get_vertex_index(i, this->n_longitude + 1));
		_indices.push_back(this->get_vertex_index(i + 1, this->n_longitude));
		_indices.push_back(this->get_vertex_index(i + 1, this->n_longitude + 1));

	}

	//south pole
	for (int j = 1; j <= this->n_longitude - 1; j++)
	{
		_indices.push_back(this->get_vertex_index(this->n_latitude, j));
		_indices.push_back(this->n_color_vertex - 1);
		_indices.push_back(this->get_vertex_index(this->n_latitude, j + 1));
	}

	_indices.push_back(this->get_vertex_index(this->n_latitude, this->n_longitude));
	_indices.push_back(this->n_color_vertex - 1);
	_indices.push_back(this->get_vertex_index(this->n_latitude, this->n_longitude + 1));
	//|||| total 6*n_longitude*n_latitude

	//convert to normal array
	this->meshColorIndex = new unsigned short[this->n_mesh];
	for (int i = 0; i < _indices.size(); i++)
	{
		this->meshColorIndex[i] = _indices[i];
	}

}

int StandardEllipsoid::get_vertex_index(int latitude, int longitude)
{
	//each latitude circle has n_longitude + 1 cross points
	return (latitude - 1)*(this->n_longitude + 1) + longitude;
}

DirectX::XMFLOAT3 StandardEllipsoid::get_random_color()
{
	return DirectX::XMFLOAT3(this->randu(), this->randu(), this->randu());
}

float StandardEllipsoid::randu()
{
	return ((float)rand() / (RAND_MAX));
}

void StandardEllipsoid::create_texture_mesh()
{
	//first point is (0,b,0)
	this->vertexPositionTexture = new VertexPositionTexture[this->n_texture_vertex];
	vertexPositionTexture[0].pos = DirectX::XMFLOAT3(this->m_position.x, b + this->m_position.y, this->m_position.z);
	vertexPositionTexture[0].norm = DirectX::XMFLOAT3(0, 1, 0);
	vertexPositionTexture[0].tex = DirectX::XMFLOAT2(0.5, 0.0);
	vertexPositionTexture[this->n_texture_vertex - 1].pos = DirectX::XMFLOAT3(this->m_position.x, -b + this->m_position.y, this->m_position.z);
	vertexPositionTexture[this->n_texture_vertex - 1].norm = DirectX::XMFLOAT3(0, -1, 0);
	vertexPositionTexture[this->n_texture_vertex - 1].tex = DirectX::XMFLOAT2(0.5, 1.0);
	int index = 1;
	for (int i = 1; i <= this->n_latitude; i++)
	{
		//close the texture
		for (int j = 1; j <= this->n_longitude + 1; j++)
		{
			float theta = (float)i*DirectX::XM_PI / (this->n_latitude + 1);
			float phi = (float)(j - 1)*DirectX::XM_2PI / this->n_longitude;
			float z = this->c*sin(theta) * cos(phi) + this->m_position.z;
			float x = this->a*sin(theta) * sin(phi) + this->m_position.x;
			float y = this->b*cos(theta) + this->m_position.y;

			float ty = float(i) / (this->n_latitude + 1);
			float tx = float(j - 1) / (this->n_longitude);
			vertexPositionTexture[index].pos = DirectX::XMFLOAT3(x, y, z);
			vertexPositionTexture[index].norm = DirectX::XMFLOAT3(2 * (x - this->m_position.x) / (this->b*this->b), 
				2 * (y - this->m_position.y) / (this->b*this->b), 
				2 * (z - this->m_position.z) / (this->c*this->c));
			vertexPositionTexture[index].tex = DirectX::XMFLOAT2(tx, ty);
			index++;
		}
	}
}

void StandardEllipsoid::create_mesh_texture_index()
{
	std::vector <int> _indices;

	//north pole
	for (int j = 1; j <= this->n_longitude - 1; j++)
	{
		_indices.push_back(0);
		_indices.push_back(j);
		_indices.push_back(j + 1);
	}

	_indices.push_back(0);
	_indices.push_back(this->n_longitude);
	_indices.push_back(1);

	//normal grid
	for (int i = 1; i <= this->n_latitude - 1; i++)
	{
		for (int j = 1; j <= this->n_longitude - 1; j++)
		{
			_indices.push_back(this->get_vertex_index(i, j));
			_indices.push_back(this->get_vertex_index(i + 1, j));
			_indices.push_back(this->get_vertex_index(i, j + 1));

			_indices.push_back(this->get_vertex_index(i, j + 1));
			_indices.push_back(this->get_vertex_index(i + 1, j));
			_indices.push_back(this->get_vertex_index(i + 1, j + 1));
		}

		_indices.push_back(this->get_vertex_index(i, this->n_longitude));
		_indices.push_back(this->get_vertex_index(i + 1, this->n_longitude));
		_indices.push_back(this->get_vertex_index(i, 1));

		_indices.push_back(this->get_vertex_index(i, 1));
		_indices.push_back(this->get_vertex_index(i + 1, this->n_longitude));
		_indices.push_back(this->get_vertex_index(i + 1, 1));

	}

	//south pole
	for (int j = 1; j <= this->n_longitude - 1; j++)
	{
		_indices.push_back(this->get_vertex_index(this->n_latitude, j));
		_indices.push_back(this->n_color_vertex - 1);
		_indices.push_back(this->get_vertex_index(this->n_latitude, j + 1));
	}

	_indices.push_back(this->get_vertex_index(this->n_latitude, this->n_longitude));
	_indices.push_back(this->n_color_vertex - 1);
	_indices.push_back(this->get_vertex_index(this->n_latitude, 1));

	//convert to normal array
	this->meshTextureIndex = new unsigned short[this->n_mesh];
	for (int i = 0; i < _indices.size(); i++)
	{
		this->meshTextureIndex[i] = _indices[i];
	}
}