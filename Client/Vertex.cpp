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



