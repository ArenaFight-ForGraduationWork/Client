#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;





//
//	Diffused Vertex ( Position, color )
//
class CDiffusedVertex
{
public:
	CDiffusedVertex();
	CDiffusedVertex(CXMVECTOR position, CXMVECTOR color);
	CDiffusedVertex(CDiffusedVertex& ref) {}
	~CDiffusedVertex() {}

private:
	XMFLOAT3 m_f3Position;
	XMCOLOR m_cDiffuse;
};





//
//	Normal Vertex ( Position, Normal )
//
class CNormalVertex
{
public:
	CNormalVertex();
	CNormalVertex(CXMVECTOR position, CXMVECTOR normal);
	CNormalVertex(CNormalVertex& ref) {}
	~CNormalVertex() {}

	void SetNormal(CXMVECTOR normal);

private:
	XMFLOAT3 m_f3Position;
	XMFLOAT3 m_f3Normal;
};





//
//	Textured Normal Vertex ( Position, Normal, UV )
//
class CTexturedNormalVertex
{
public:
	CTexturedNormalVertex();
	CTexturedNormalVertex(CXMVECTOR position, CXMVECTOR normal, CXMVECTOR uv);
	CTexturedNormalVertex(CTexturedNormalVertex& ref) {}
	~CTexturedNormalVertex() {}

	void SetPosition(float x, float y, float z);
	void SetNormal(float x, float y, float z);
	void SetUV(float u, float v);

private:
	XMFLOAT3 m_f3Position;
	XMFLOAT3 m_f3Normal;
	XMFLOAT2 m_f2TexCoord;
};





//
//	Animation Vertex ( Position, Normal, UV, BoneIndex, BoneWeight )
//
class CAnimationVertex
{
public:
	XMFLOAT3 m_f3Position;
	XMFLOAT3 m_f3Normal;
	XMFLOAT2 m_f2TexCoord;
	int iBoneIndex[8];
	float fBoneWeight[8];

	CAnimationVertex();
	CAnimationVertex(CAnimationVertex& ref) {}

	~CAnimationVertex() {}

	void AddBone(int index, float Weight);
};



