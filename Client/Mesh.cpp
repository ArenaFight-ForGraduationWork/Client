#include "stdafx.h"
#include "Mesh.h"
#include "Fbx.h"





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

	m_pMaxVer = new D3DXVECTOR3();
	m_pMinVer = new D3DXVECTOR3();
	
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
	if (m_pd3dVertexBuffer) gpCommonState->m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//�ε��� ���۰� ������ �ε��� ���۸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dIndexBuffer) gpCommonState->m_pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	gpCommonState->m_pd3dDeviceContext->IASetPrimitiveTopology(
		m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) gpCommonState->m_pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	// �ε��� ���۰� ������ �ε��� ���۸� ����Ͽ� �޽��� �������ϰ�
	// ������ ���� ���۸��� ����Ͽ� �޽��� �������Ѵ�.
	if (m_pd3dIndexBuffer)
		gpCommonState->m_pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		gpCommonState->m_pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}

void CMesh::CreateRasterizerState()
{
}

CCubeMeshIlluminatedTextured::CCubeMeshIlluminatedTextured(float fWidth, float fHeight, float fDepth) : CMesh()
{
	m_nVertices = 36;
	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pMaxVer->x = fx; m_pMaxVer->y = fy; m_pMaxVer->z = fz;
	m_pMinVer->x = -fx; m_pMinVer->y = -fy; m_pMinVer->z = -fz;

	CTexturedNormalVertex pVertices[36];
	int i = 0;
	//XMLoadFloat3(&XMFLOAT3
	//XMLoadFloat2(&XMFLOAT2
	//������ü�� �� �鿡 �ؽ��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
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

	//�� ������ �������͸� ����Ѵ�.
	CalculateVertexNormal((BYTE *)pVertices, NULL);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	gpCommonState->m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	SetRasterizerState();
}

CCubeMeshIlluminatedTextured::~CCubeMeshIlluminatedTextured()
{
}

void CCubeMeshIlluminatedTextured::SetRasterizerState()
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	gpCommonState->m_pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCubeMeshIlluminatedTextured::Render()
{
	if (m_pd3dVertexBuffer) gpCommonState->m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//�ε��� ���۰� ������ �ε��� ���۸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dIndexBuffer) gpCommonState->m_pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	gpCommonState->m_pd3dDeviceContext->IASetPrimitiveTopology(
		m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) gpCommonState->m_pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	// �ε��� ���۰� ������ �ε��� ���۸� ����Ͽ� �޽��� �������ϰ�
	// ������ ���� ���۸��� ����Ͽ� �޽��� �������Ѵ�.
	if (m_pd3dIndexBuffer)
		gpCommonState->m_pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		gpCommonState->m_pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}

void CCubeMeshIlluminatedTextured::CalculateVertexNormal(BYTE *pVertices, WORD *pIndices)
{	// ���� �����Ϳ� �ε��� �����͸� ����Ͽ� ������ ���� ���͸� ���
	switch (m_d3dPrimitiveTopology)
	{
		/*������Ƽ�갡 �ﰢ�� ����Ʈ�� �� �ε��� ���۰� �ִ� ���� ���� ��츦 �����Ͽ� ������ ���� ���͸� ����Ѵ�.
		�ε��� ���۸� ������� �ʴ� ��� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ���� ���� ���ͷ� ����Ѵ�.
		�ε��� ���۸� ����ϴ� ��� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ������ ���� ������ �������(���Ͽ�) ����Ѵ�.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!pIndices)
			SetTriAngleListVertexNormal(pVertices);
		else
			SetAverageVertexNormal(pVertices, pIndices, (m_nIndices / 3), 3, false);
		break;
		/*������Ƽ�갡 �ﰢ�� ��Ʈ���� �� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ������ ���� ������ �������(���Ͽ�) ����Ѵ�.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pVertices, pIndices, (pIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

void CCubeMeshIlluminatedTextured::SetTriAngleListVertexNormal(BYTE *pVertices)
{	// �ε��� ���۸� ������� �ʴ� �ﰢ�� ����Ʈ�� ���Ͽ� ������ ���� ���͸� ���
	XMVECTOR d3dxvNormal;
	CNormalVertex *pVertex = NULL;
	/*�ﰢ��(������Ƽ��)�� ������ ���ϰ� �� �ﰢ���� ���� ���͸� ����ϰ� �ﰢ���� �����ϴ� �� ������ ���� ���ͷ� �����Ѵ�.*/
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
	}
}

void CCubeMeshIlluminatedTextured::SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR d3dxvSumOfNormal = DirectX::XMVectorZero();
	CNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = DirectX::XMVectorZero();
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j))
			{
				d3dxvSumOfNormal = DirectX::XMVectorAdd(d3dxvSumOfNormal, CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2));
			}
		}
		DirectX::XMVector3Normalize(d3dxvSumOfNormal);
		pVertex = (CNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->SetNormal(d3dxvSumOfNormal);
	}
}

XMVECTOR CCubeMeshIlluminatedTextured::CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{	// �ﰢ���� �� ������ ����Ͽ� �ﰢ���� ���� ���͸� ���
	XMVECTOR d3dxvNormal = DirectX::XMVectorZero();
	XMVECTOR d3dxvP0 = *((XMVECTOR *)(pVertices + (m_nStride * nIndex0)));
	XMVECTOR d3dxvP1 = *((XMVECTOR *)(pVertices + (m_nStride * nIndex1)));
	XMVECTOR d3dxvP2 = *((XMVECTOR *)(pVertices + (m_nStride * nIndex2)));
	XMVECTOR d3dxvEdge1 = DirectX::XMVectorSubtract(d3dxvP1, d3dxvP0);
	XMVECTOR d3dxvEdge2 = DirectX::XMVectorSubtract(d3dxvP2, d3dxvP0);
	d3dxvNormal = DirectX::XMVector3Cross(d3dxvEdge1, d3dxvEdge2);
	DirectX::XMVector3Normalize(d3dxvNormal);

	return d3dxvNormal;
}










CImportedMesh::CImportedMesh(char* ptxtName) : CMesh()
{
	CFbx *pFbx = new CFbx();

	pFbx->ReadTextFile_Mesh(ptxtName, ppVertices);

	m_nVertices = pFbx->GetSize();

	m_pMaxVer = &(pFbx->GetMaxVer());
	m_pMinVer = &(pFbx->GetMinVer());

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

	gpCommonState->m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

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
	gpCommonState->m_pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CImportedMesh::Render()
{
	CMesh::Render();
}









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

	m_pMaxVer = &(pFbx->GetMaxVer());
	m_pMinVer = &(pFbx->GetMinVer());

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
	gpCommonState->m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

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
	gpCommonState->m_pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CImportedAnimatingMesh::Render()
{
	CMesh::Render();
}




