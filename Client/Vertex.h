#ifndef VERTEX_H_
#define VERTEX_H_

#include <DirectXMath.h>
using namespace DirectX;


class CVertex
{
public:
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
	D3DXVECTOR3 GetPosition()
	{
		return m_d3dxvPosition;
	}

private:
	D3DXVECTOR3 m_d3dxvPosition;
};

class CDiffusedVertex
{
public:
	CDiffusedVertex();
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse);
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse);
	~CDiffusedVertex();

private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXCOLOR m_d3dxcDiffuse;
};

class CNormalVertex
{
public:
	CNormalVertex(float x, float y, float z, float nx, float ny, float nz) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); }
	CNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; }
	CNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~CNormalVertex() { }

	void SetNormal(D3DXVECTOR3 d3dxvNormal) { m_d3dxvNormal = d3dxvNormal; }

private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
};

class CTexturedVertex
{
public:
	CTexturedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f); }
	CTexturedVertex(float x, float y, float z, float u, float v) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvTexCoord = D3DXVECTOR2(u, v); }
	CTexturedVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR2 d3dxvTexture) { m_d3dxvPosition = d3dxvPosition; m_d3dxvTexCoord = d3dxvTexture; }
	~CTexturedVertex() { }

	void SetPosition(float x, float y, float z) { m_d3dxvPosition.x = x; m_d3dxvPosition.y = y; m_d3dxvPosition.z = z; }
	void SetTexCoord(float u, float v) { m_d3dxvTexCoord.x = u; m_d3dxvTexCoord.y = v; }

private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR2 m_d3dxvTexCoord;
};

class CTexturedNormalVertex
{
private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;

public:
	CTexturedNormalVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); m_d3dxvTexCoord = D3DXVECTOR2(u, v); }
	CTexturedNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvTexCoord) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; m_d3dxvTexCoord = d3dxvTexCoord; }
	CTexturedNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f); }
	~CTexturedNormalVertex() { }

	D3DXVECTOR3 GetPosition(){ return m_d3dxvPosition; }
	D3DXVECTOR3 GetNormal()  { return m_d3dxvNormal; }
	D3DXVECTOR2 GetUV()	{ return m_d3dxvTexCoord; }

	void SetPosition(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	void SetNormal(D3DXVECTOR3 d3dxvNormal) { m_d3dxvNormal = d3dxvNormal; }
	void SetUV(D3DXVECTOR2 d3dxvTexCoord) { m_d3dxvTexCoord = d3dxvTexCoord; }
};

class CAnimationVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;

	int BoneIndexArr[8];
	float BoneWeightArr[8];

	CAnimationVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 8; ++i)
		{
			BoneIndexArr[i] = 0;
			BoneWeightArr[i] = -1.0f;
		}
	}

	CAnimationVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvNormal = d3dxvNormal;
	}

	CAnimationVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvUV)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvNormal = d3dxvNormal;
		m_d3dxvTexCoord = d3dxvUV;
	}
	~CAnimationVertex(){}
	void AddBone(int index, float Weight);
};






namespace Vertex
{
	// Basic 32-byte vertex structure.
	struct Basic32
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct Terrain
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT2 BoundsY;
	};

	struct Particle
	{
		XMFLOAT3 InitialPos;
		XMFLOAT3 InitialVel;
		XMFLOAT2 Size;
		float Age;
		unsigned int Type;
	};
}

class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC Pos[1];
	static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
	static const D3D11_INPUT_ELEMENT_DESC Terrain[3];
	static const D3D11_INPUT_ELEMENT_DESC Particle[5];
};

class InputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* Basic32;
	static ID3D11InputLayout* Terrain;
	static ID3D11InputLayout* Particle;
};



#endif