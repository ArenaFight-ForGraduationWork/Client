#include "stdafx.h"
#include "Vertex.h"


CDiffusedVertex::CDiffusedVertex()
{
	m_d3dxvPosition = D3DXVECTOR3(0, 0, 0);
	m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
CDiffusedVertex::CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse)
{
	m_d3dxvPosition = D3DXVECTOR3(x, y, z);
	m_d3dxcDiffuse = d3dxcDiffuse;
}
CDiffusedVertex::CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse)
{
	m_d3dxvPosition = d3dxvPosition;
	m_d3dxcDiffuse = d3dxcDiffuse;
}
CDiffusedVertex::~CDiffusedVertex()
{
}




