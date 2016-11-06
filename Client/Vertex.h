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
	XMFLOAT3 m_f3Position;
};

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
	XMFLOAT3 m_f3Position;
	XMFLOAT2 m_f2TexCoord;
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
	XMFLOAT3 m_f3Position;
	XMFLOAT3 m_f3Normal;
	XMFLOAT2 m_f2TexCoord;
};

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




#endif