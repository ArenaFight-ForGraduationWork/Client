#ifndef VERTEX_H_
#define VERTEX_H_

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;


class CVertex
{
public:
	CVertex();
	CVertex(CXMVECTOR position);
	CVertex(CVertex& ref) {}
	~CVertex() {}

	CXMVECTOR GetPosition();

private:
	XMFLOAT3 m_d3dxvPosition;
};

class CDiffusedVertex
{
public:
	CDiffusedVertex();
	CDiffusedVertex(CXMVECTOR position, CXMVECTOR color);
	CDiffusedVertex(CDiffusedVertex& ref) {}
	~CDiffusedVertex() {}

private:
	XMFLOAT3 m_d3dxvPosition;
	XMCOLOR m_d3dxcDiffuse;
};

class CNormalVertex
{
public:
	CNormalVertex();
	CNormalVertex(CXMVECTOR position, CXMVECTOR normal);
	CNormalVertex(CNormalVertex& ref) {}
	~CNormalVertex() {}

	void SetNormal(CXMVECTOR normal);

private:
	XMFLOAT3 m_d3dxvPosition;
	XMFLOAT3 m_d3dxvNormal;
};

class CTexturedVertex
{
public:
	CTexturedVertex();
	CTexturedVertex(CXMVECTOR position, CXMVECTOR uv);
	CTexturedVertex(CTexturedVertex& ref) {}
	~CTexturedVertex() {}

	void SetPosition(float x, float y, float z);
	void SetTexCoord(float u, float v);

private:
	XMFLOAT3 m_d3dxvPosition;
	XMFLOAT2 m_d3dxvTexCoord;
};

class CTexturedNormalVertex
{
public:
	CTexturedNormalVertex();
	CTexturedNormalVertex(CXMVECTOR position, CXMVECTOR normal, CXMVECTOR uv);
	CTexturedNormalVertex(CTexturedNormalVertex& ref) {}
	~CTexturedNormalVertex() {}

	CXMVECTOR GetPosition();
	CXMVECTOR GetNormal();
	CXMVECTOR GetUV();

	void SetPosition(float x, float y, float z);
	void SetNormal(float x, float y, float z);
	void SetUV(float u, float v);

private:
	XMFLOAT3 m_d3dxvPosition;
	XMFLOAT3 m_d3dxvNormal;
	XMFLOAT2 m_d3dxvTexCoord;
};

class CAnimationVertex
{
public:
	XMFLOAT3 m_d3dxvPosition;
	XMFLOAT3 m_d3dxvNormal;
	XMFLOAT2 m_d3dxvTexCoord;
	int BoneIndexArr[8];
	float BoneWeightArr[8];

	CAnimationVertex();
	CAnimationVertex(CAnimationVertex& ref) {}

	//CAnimationVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal)
	//{
	//	m_d3dxvPosition = d3dxvPosition;
	//	m_d3dxvNormal = d3dxvNormal;
	//}
	//CAnimationVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvUV)
	//{
	//	m_d3dxvPosition = d3dxvPosition;
	//	m_d3dxvNormal = d3dxvNormal;
	//	m_d3dxvTexCoord = d3dxvUV;
	//}
	~CAnimationVertex(){}

	void AddBone(int index, float Weight);
};




#endif