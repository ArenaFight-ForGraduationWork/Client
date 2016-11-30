#include "stdafx.h"
#include "Mesh.h"
#include "Fbx.h"





//
//	Mesh
//
CMesh::CMesh()
{
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nReferences = 1;
	m_pd3dRasterizerState = NULL;

	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;

	m_pf3MaxVer = new XMFLOAT3(0, 0, 0);
	m_pf3MinVer = new XMFLOAT3(0, 0, 0);

	for (int i = 0; i < ANIMATION_COUNT; ++i)
	{
		m_ppResult[i] = nullptr;
		m_iAnimationMaxTime[i] = 0;
	}
	m_uiAnimationIndexCnt = 0;
}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
}

void CMesh::AddRef()
{
	m_nReferences++;
}

void CMesh::Release()
{
	m_nReferences--;
	if (m_nReferences == 0) delete this;
}

void CMesh::Render()
{
	if (m_pd3dVertexBuffer) gpCommonState->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//인덱스 버퍼가 있으면 인덱스 버퍼를 디바이스 컨텍스트에 연결한다.
	if (m_pd3dIndexBuffer) gpCommonState->GetDeviceContext()->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	gpCommonState->GetDeviceContext()->IASetPrimitiveTopology(
		m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) gpCommonState->GetDeviceContext()->RSSetState(m_pd3dRasterizerState);

	// 인덱스 버퍼가 있으면 인덱스 버퍼를 사용하여 메쉬를 렌더링하고
	// 없으면 정점 버퍼만을 사용하여 메쉬를 렌더링한다.
	if (m_pd3dIndexBuffer)
		gpCommonState->GetDeviceContext()->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		gpCommonState->GetDeviceContext()->Draw(m_nVertices, m_nOffset);
}





//
//	CubeMesh
//
CCubeMesh::CCubeMesh(float fWidth, float fHeight, float fDepth) : CMesh()
{
	m_nVertices = 36;
	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	*m_pf3MaxVer = XMFLOAT3(fx, fy, fz);
	*m_pf3MinVer = XMFLOAT3(-fx, -fy, -fz);

	CTexturedNormalVertex pVertices[36];
	int i = 0;
	//직육면체의 한 면에 텍스쳐 전체가 맵핑되도록 텍스쳐 좌표를 설정한다.
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(-fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));

	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, +fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, +fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(1.0f, 1.0f)));
	pVertices[i++] = CTexturedNormalVertex(XMLoadFloat3(&XMFLOAT3(+fx, -fy, -fz)), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), XMLoadFloat2(&XMFLOAT2(0.0f, 1.0f)));

	//각 정점의 법선벡터를 계산한다.
	_CalculateVertexNormal((BYTE *)pVertices, NULL);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	gpCommonState->GetDevice()->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	SetRasterizerState();
}

CCubeMesh::~CCubeMesh()
{
}

void CCubeMesh::SetRasterizerState()
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	gpCommonState->GetDevice()->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCubeMesh::Render()
{
	if (m_pd3dVertexBuffer) gpCommonState->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//인덱스 버퍼가 있으면 인덱스 버퍼를 디바이스 컨텍스트에 연결한다.
	if (m_pd3dIndexBuffer) gpCommonState->GetDeviceContext()->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	gpCommonState->GetDeviceContext()->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) gpCommonState->GetDeviceContext()->RSSetState(m_pd3dRasterizerState);

	// 인덱스 버퍼가 있으면 인덱스 버퍼를 사용하여 메쉬를 렌더링하고
	// 없으면 정점 버퍼만을 사용하여 메쉬를 렌더링한다.
	if (m_pd3dIndexBuffer)
		gpCommonState->GetDeviceContext()->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		gpCommonState->GetDeviceContext()->Draw(m_nVertices, m_nOffset);
}

void CCubeMesh::_SetTriAngleListVertexNormal(BYTE *pVertices)
{	// 인덱스 버퍼를 사용하지 않는 삼각형 리스트에 대하여 정점의 법선 벡터를 계산
	XMVECTOR vNormal;
	CNormalVertex *pVertex = nullptr;
	/*삼각형(프리미티브)의 개수를 구하고 각 삼각형의 법선 벡터를 계산하고 삼각형을 구성하는 각 정점의 법선 벡터로 지정한다.*/
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		vNormal = XMLoadFloat3(_SetTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2)));
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->SetNormal(vNormal);
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->SetNormal(vNormal);
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->SetNormal(vNormal);
	}
}
void CCubeMesh::_SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR vSumOfNormal = XMVectorZero();
	CNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		vSumOfNormal = XMVectorZero();
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j))
			{
				vSumOfNormal = XMVectorAdd(vSumOfNormal, XMLoadFloat3(_SetTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2)));
			}
		}
		XMVector3Normalize(vSumOfNormal);
		pVertex = (CNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->SetNormal(vSumOfNormal);
	}
}
XMFLOAT3* CCubeMesh::_SetTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{	// 삼각형의 세 정점을 사용하여 삼각형의 법선 벡터를 계산
	XMVECTOR vNormal = XMVectorZero();
	XMVECTOR vP0 = *((XMVECTOR *)(pVertices + (m_nStride * nIndex0)));
	XMVECTOR vP1 = *((XMVECTOR *)(pVertices + (m_nStride * nIndex1)));
	XMVECTOR vP2 = *((XMVECTOR *)(pVertices + (m_nStride * nIndex2)));
	XMVECTOR vEdge1 = XMVectorSubtract(vP1, vP0);
	XMVECTOR vEdge2 = XMVectorSubtract(vP2, vP0);
	vNormal = XMVector3Cross(vEdge1, vEdge2);
	XMVector3Normalize(vNormal);

	XMFLOAT3 *pf3Normal = new XMFLOAT3();
	XMStoreFloat3(pf3Normal, vNormal);

	return pf3Normal;
}
void CCubeMesh::_CalculateVertexNormal(BYTE *pVertices, WORD *pIndices)
{	// 정점 데이터와 인덱스 데이터를 사용하여 정점의 법선 벡터를 계산
	switch (m_d3dPrimitiveTopology)
	{
		/*프리미티브가 삼각형 리스트일 때 인덱스 버퍼가 있는 경우와 없는 경우를 구분하여 정점의 법선 벡터를 계산한다.
		인덱스 버퍼를 사용하지 않는 경우 각 정점의 법선 벡터는 그 정점이 포함된 삼각형의 법선 벡터로 계산한다.
		인덱스 버퍼를 사용하는 경우 각 정점의 법선 벡터는 그 정점이 포함된 삼각형들의 법선 벡터의 평균으로(더하여) 계산한다.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!pIndices)
			_SetTriAngleListVertexNormal(pVertices);
		else
			_SetAverageVertexNormal(pVertices, pIndices, (m_nIndices / 3), 3, false);
		break;
		/*프리미티브가 삼각형 스트립일 때 각 정점의 법선 벡터는 그 정점이 포함된 삼각형들의 법선 벡터의 평균으로(더하여) 계산한다.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		_SetAverageVertexNormal(pVertices, pIndices, (pIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}





//
//	ImportedMesh
//
CImportedMesh::CImportedMesh(char* ptxtName) : CMesh()
{
	CFbx *pFbx = new CFbx();

	pFbx->ReadTextFile_Mesh(ptxtName, ppVertices);

	m_nVertices = pFbx->GetSize();

	m_pf3MaxVer = pFbx->GetMaxVer();
	m_pf3MinVer = pFbx->GetMinVer();

	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = ppVertices;

	gpCommonState->GetDevice()->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState();

	delete[] ppVertices;
}

CImportedMesh::~CImportedMesh()
{
	CMesh::~CMesh();
}

void CImportedMesh::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	gpCommonState->GetDevice()->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}





//
//	ImportedAnimatingMesh
//
CImportedAnimatingMesh::CImportedAnimatingMesh(int CharNum, int StateCnt) : CMesh()
{
	CFbx *pFbx = new CFbx();

	pFbx->ReadTextFile_Mesh(CharNum, ppVertices);
	m_nVertices = pFbx->GetSize();

	for (int i = 0; i < StateCnt; ++i)
	{
		pFbx->ReadTextFile_Ani(CharNum, i);
		m_ppResult[i] = pFbx->GetResult(i);
		m_iAnimationMaxTime[i] = pFbx->GetAnimationMaxTime();
	}

	m_uiAnimationIndexCnt = pFbx->GetAnimationIndexCount();

	m_pf3MaxVer = pFbx->GetMaxVer();
	m_pf3MinVer = pFbx->GetMinVer();

	pFbx->ReadTextFile_Weight(CharNum, ppVertices);

	m_nStride = sizeof(CAnimationVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = ppVertices;
	gpCommonState->GetDevice()->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState();
}

CImportedAnimatingMesh::~CImportedAnimatingMesh()
{
	CMesh::~CMesh();
}

void CImportedAnimatingMesh::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	gpCommonState->GetDevice()->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}



