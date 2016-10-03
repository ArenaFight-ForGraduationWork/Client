#include "stdafx.h"
#include "Vertex.h"



CVertex::CVertex()
{
	m_d3dxvPosition = XMFLOAT3(0, 0, 0);
}
CVertex::CVertex(CXMVECTOR position)
{
	XMStoreFloat3(&m_d3dxvPosition, position);
}
CXMVECTOR CVertex::GetPosition()
{
	return XMLoadFloat3(&m_d3dxvPosition);
}





CDiffusedVertex::CDiffusedVertex()
{
	m_d3dxvPosition = XMFLOAT3(0, 0, 0);
	m_d3dxcDiffuse = XMCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
CDiffusedVertex::CDiffusedVertex(CXMVECTOR position, CXMVECTOR color)
{
	XMStoreFloat3(&m_d3dxvPosition, position);
	XMStoreColor(&m_d3dxcDiffuse, color);
}





CNormalVertex::CNormalVertex()
{
	m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
CNormalVertex::CNormalVertex(CXMVECTOR position, CXMVECTOR normal)
{
	XMStoreFloat3(&m_d3dxvPosition, position);
	XMStoreFloat3(&m_d3dxvNormal, normal);
}
void CNormalVertex::SetNormal(CXMVECTOR normal)
{
	XMStoreFloat3(&m_d3dxvNormal, normal);
}





CTexturedVertex::CTexturedVertex()
{
	m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	m_d3dxvTexCoord = XMFLOAT2(0.0f, 0.0f); 
}
CTexturedVertex::CTexturedVertex(CXMVECTOR position, CXMVECTOR uv)
{ 
	XMStoreFloat3(&m_d3dxvPosition, position);
	XMStoreFloat2(&m_d3dxvTexCoord, uv);
}
void CTexturedVertex::SetPosition(float x, float y, float z)
{
	m_d3dxvPosition.x = x;
	m_d3dxvPosition.y = y; 
	m_d3dxvPosition.z = z; 
}
void CTexturedVertex::SetTexCoord(float u, float v)
{
	m_d3dxvTexCoord.x = u; 
	m_d3dxvTexCoord.y = v;
}





CTexturedNormalVertex::CTexturedNormalVertex()
{
	m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvTexCoord = XMFLOAT2(0.0f, 0.0f);
}
CTexturedNormalVertex::CTexturedNormalVertex(CXMVECTOR position, CXMVECTOR normal, CXMVECTOR uv)
{
	XMStoreFloat3(&m_d3dxvPosition, position);
	XMStoreFloat3(&m_d3dxvNormal, normal);
	XMStoreFloat2(&m_d3dxvTexCoord, uv);
}
CXMVECTOR CTexturedNormalVertex::GetPosition()
{
	return XMLoadFloat3(&m_d3dxvPosition);
}
CXMVECTOR CTexturedNormalVertex::GetNormal()
{
	return XMLoadFloat3(&m_d3dxvNormal);
}
CXMVECTOR CTexturedNormalVertex::GetUV()
{
	return XMLoadFloat2(&m_d3dxvTexCoord);
}
void CTexturedNormalVertex::SetPosition(float x, float y, float z)
{
	m_d3dxvPosition = XMFLOAT3(x, y, z);
}
void CTexturedNormalVertex::SetNormal(float x, float y, float z)
{
	m_d3dxvNormal = XMFLOAT3(x, y, z);
}
void CTexturedNormalVertex::SetUV(float u, float v)
{
	m_d3dxvTexCoord = XMFLOAT2(u, v);
}





CAnimationVertex::CAnimationVertex()
{
	m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvTexCoord = XMFLOAT2(0.0f, 0.0f);
	for (int i = 0; i < 8; ++i)
	{
		BoneIndexArr[i] = 0;
		BoneWeightArr[i] = -1.0f;
	}
}
void CAnimationVertex::AddBone(int index, float weight)
{
	for (int i = 0; i < 8; ++i)
	{
		if (BoneWeightArr[i] <= 0.0f)
		{
			BoneIndexArr[i] = index;
			BoneWeightArr[i] = weight;
			return;
		}
	}

	//만약 배열을 벗어나면 첫번째 인덱스에 다 더한다.
	BoneWeightArr[0] += weight;
}



