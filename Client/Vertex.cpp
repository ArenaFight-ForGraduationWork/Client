#include "stdafx.h"
#include "Vertex.h"





//
//	Diffused Vertex
//
CDiffusedVertex::CDiffusedVertex()
{
	m_f3Position = XMFLOAT3(0, 0, 0);
	m_cDiffuse = XMCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
CDiffusedVertex::CDiffusedVertex(CXMVECTOR position, CXMVECTOR color)
{
	XMStoreFloat3(&m_f3Position, position);
	XMStoreColor(&m_cDiffuse, color);
}





//
//	Normal Vertex
//
CNormalVertex::CNormalVertex()
{
	m_f3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_f3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
CNormalVertex::CNormalVertex(CXMVECTOR position, CXMVECTOR normal)
{
	XMStoreFloat3(&m_f3Position, position);
	XMStoreFloat3(&m_f3Normal, normal);
}
void CNormalVertex::SetNormal(CXMVECTOR normal)
{
	XMStoreFloat3(&m_f3Normal, normal);
}





//
//	Textured Normal Vertex
//
CTexturedNormalVertex::CTexturedNormalVertex()
{
	m_f3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_f3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_f2TexCoord = XMFLOAT2(0.0f, 0.0f);
}
CTexturedNormalVertex::CTexturedNormalVertex(CXMVECTOR position, CXMVECTOR normal, CXMVECTOR uv)
{
	XMStoreFloat3(&m_f3Position, position);
	XMStoreFloat3(&m_f3Normal, normal);
	XMStoreFloat2(&m_f2TexCoord, uv);
}
void CTexturedNormalVertex::SetPosition(float x, float y, float z)
{
	m_f3Position = XMFLOAT3(x, y, z);
}
void CTexturedNormalVertex::SetNormal(float x, float y, float z)
{
	m_f3Normal = XMFLOAT3(x, y, z);
}
void CTexturedNormalVertex::SetUV(float u, float v)
{
	m_f2TexCoord = XMFLOAT2(u, v);
}





//
//	Animation Vertex
//
CAnimationVertex::CAnimationVertex()
{
	m_f3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_f3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_f2TexCoord = XMFLOAT2(0.0f, 0.0f);
	for (int i = 0; i < 8; ++i)
	{
		iBoneIndex[i] = 0;
		fBoneWeight[i] = -1.0f;
	}
}
void CAnimationVertex::AddBone(int index, float weight)
{
	for (int i = 0; i < 8; ++i)
	{
		if (fBoneWeight[i] <= 0.0f)
		{
			iBoneIndex[i] = index;
			fBoneWeight[i] = weight;
			return;
		}
	}

	//만약 배열을 벗어나면 첫번째 인덱스에 다 더한다.
	fBoneWeight[0] += weight;
}



