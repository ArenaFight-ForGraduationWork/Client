#include "stdafx.h"
#include "Mesh.h"
#include "Fbx.h"


CMesh::CMesh(ID3D11Device *pd3dDevice)
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

	for (int i = 0; i < 5; ++i)
		m_ppResult[i] = nullptr;
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

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//�ε��� ���۰� ������ �ε��� ���۸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dIndexBuffer) pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pd3dDeviceContext->IASetPrimitiveTopology(
		m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	// �ε��� ���۰� ������ �ε��� ���۸� ����Ͽ� �޽��� �������ϰ�
	// ������ ���� ���۸��� ����Ͽ� �޽��� �������Ѵ�.
	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}

void CMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
}





CAirplaneMesh::CAirplaneMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor) : CMesh(pd3dDevice)
{
	m_nVertices = 24 * 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex pVertices[24 * 3];

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;

	//����� �޽��� ���� ��ġ�� �����Ѵ�.
	//����� �޽��� ���� ���
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	//����� �޽��� �Ʒ��� ���
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);

	//����� �޽��� ������ ���
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	//����� �޽��� ����/������ ���
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);

	//����� �޽��� ���� ���
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);

	//����� �޽��� ����/���� ���
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState(pd3dDevice);
}

CAirplaneMesh::~CAirplaneMesh()
{
}

void CAirplaneMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CAirplaneMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}










CCubeMeshIlluminatedTextured::CCubeMeshIlluminatedTextured(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMesh(pd3dDevice)
{
	m_nVertices = 36;
	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	CTexturedNormalVertex pVertices[36];
	int i = 0;
	//������ü�� �� �鿡 �ؽ��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

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
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	SetRasterizerState(pd3dDevice);
}

CCubeMeshIlluminatedTextured::~CCubeMeshIlluminatedTextured()
{
}

void CCubeMeshIlluminatedTextured::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCubeMeshIlluminatedTextured::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//�ε��� ���۰� ������ �ε��� ���۸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dIndexBuffer) pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pd3dDeviceContext->IASetPrimitiveTopology(
		m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	// �ε��� ���۰� ������ �ε��� ���۸� ����Ͽ� �޽��� �������ϰ�
	// ������ ���� ���۸��� ����Ͽ� �޽��� �������Ѵ�.
	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
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
	D3DXVECTOR3 d3dxvNormal;
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
	D3DXVECTOR3 d3dxvSumOfNormal(0.0f, 0.0f, 0.0f);
	CNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2);
		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pVertex = (CNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->SetNormal(d3dxvSumOfNormal);
	}
}

D3DXVECTOR3 CCubeMeshIlluminatedTextured::CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{	// �ﰢ���� �� ������ ����Ͽ� �ﰢ���� ���� ���͸� ���
	D3DXVECTOR3 d3dxvNormal(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 d3dxvP0 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex0)));
	D3DXVECTOR3 d3dxvP1 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex1)));
	D3DXVECTOR3 d3dxvP2 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex2)));
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);
	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	return(d3dxvNormal);
}

//===============================================================
//
//
//
//===============================================================

CMyModel::CMyModel(ID3D11Device *pd3dDevice, char* ptxtName, D3DXVECTOR3 d3dxvScale) : CMesh(pd3dDevice)
{

	CFbx::GetInstance()->Fbx_ReadTextFile_Mesh(ptxtName, pVertices);

	m_nVertices = pVertices.size();
	std::cout << " ��obj ������: " << m_nVertices << std::endl;

	ppVertices = new CTexturedNormalVertex[m_nVertices];

	for (int i = 0; i < pVertices.size(); ++i)
	{

		ppVertices[i].SetPosition(pVertices.at(i)->GetPosition());
		ppVertices[i].SetNormal(pVertices.at(i)->GetNormal());
		ppVertices[i].SetUV(pVertices.at(i)->GetUV());

		/*ppVertices[i].GetPosition() = pVertices.at(i)->GetPosition();
		ppVertices[i].GetNormal() = pVertices.at(i)->GetNormal();
		ppVertices[i].GetUV() = pVertices.at(i)->GetUV();*/

		/*ppVertices[i].m_d3dxvPosition = pVertices.at(i)->m_d3dxvPosition;
		ppVertices[i].m_d3dxvNormal = pVertices.at(i)->m_d3dxvNormal;
		ppVertices[i].m_d3dxvTexCoord = pVertices.at(i)->m_d3dxvTexCoord;*/
	}


	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CalculateVertexNormal((BYTE *)ppVertices, NULL);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = ppVertices;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	CreateRasterizerState(pd3dDevice);

	delete[] ppVertices;
}

CMyModel::~CMyModel()
{
	CMesh::~CMesh();
}

void CMyModel::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CMyModel::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

void CMyModel::CalculateVertexNormal(BYTE *pVertices, WORD *pIndices)
{
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

//�ε��� ���۸� ������� �ʴ� �ﰢ�� ����Ʈ�� ���Ͽ� ������ ���� ���͸� ����Ѵ�.
void CMyModel::SetTriAngleListVertexNormal(BYTE *pVertices)
{
	D3DXVECTOR3 d3dxvNormal;
	CTexturedNormalVertex *pVertex = NULL;
	/*�ﰢ��(������Ƽ��)�� ������ ���ϰ� �� �ﰢ���� ���� ���͸� ����ϰ� �ﰢ���� �����ϴ� �� ������ ���� ���ͷ� �����Ѵ�.*/
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pVertex = (CTexturedNormalVertex *)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
		pVertex = (CTexturedNormalVertex *)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
		pVertex = (CTexturedNormalVertex *)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
	}
}

//�ﰢ���� �� ������ ����Ͽ� �ﰢ���� ���� ���͸� ����Ѵ�.
D3DXVECTOR3 CMyModel::CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{
	D3DXVECTOR3 d3dxvNormal(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 d3dxvP0 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex0)));
	D3DXVECTOR3 d3dxvP1 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex1)));
	D3DXVECTOR3 d3dxvP2 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex2)));
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);
	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	return(d3dxvNormal);
}

/*������Ƽ�갡 �ε��� ���۸� ����ϴ� �ﰢ�� ����Ʈ �Ǵ� �ﰢ�� ��Ʈ���� ��� ������ ���� ���ʹ� �� ������ �����ϴ� �ﰢ���� ���� ���͵��� ������� ����Ѵ�.*/
void CMyModel::SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip)
{
	D3DXVECTOR3 d3dxvSumOfNormal(0.0f, 0.0f, 0.0f);
	CTexturedNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2);
		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pVertex = (CTexturedNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->SetNormal(d3dxvSumOfNormal);
	}
}

//=========================================
//
// �ִϸ��̼��� ���� Ŭ����
//
//=========================================


CMyAni::CMyAni(ID3D11Device *pd3dDevice, int CharNum, int StateCnt) : CMesh(pd3dDevice)
{
	CFbx::GetInstance()->Fbx_ReadTextFile_Info(CharNum);
	m_nVertices = CFbx::GetInstance()->GetSize();
	//m_nVertices = *CFbx::GetInstance()->ReturnSize();
	cout << " �ִ�obj ������ : " << m_nVertices << endl;
	
	ppVertices = new CAnimationVertex[m_nVertices];

	CFbx::GetInstance()->Fbx_ReadTextFile_Mesh(CharNum, ppVertices);

	for (int i = 0; i < StateCnt; ++i)
	{
		CFbx::GetInstance()->Fbx_ReadTextFile_Ani(CharNum, i);
		m_ppResult[i] = CFbx::GetInstance()->GetResult(i);								
		m_AniMaxTime[i] = CFbx::GetInstance()->GetAnimationMaxTime();
		cout << i << "��° maxtime*_*:" << m_AniMaxTime[i] << endl;
		SetAnimationMaxTime(m_AniMaxTime[i]);
	}
	
	m_AnimationIndexCnt = CFbx::GetInstance()->GetAnimationIndexCount();		
	SetAnimationIndexCnt(m_AnimationIndexCnt);		// Cmesh�� AnimationIndex�� �־���. object���� ������ �� �ֵ���
	SetMaxVer(CFbx::GetInstance()->GetMaxVer());	// ���� �����ͼ� CMesh�� m_MaxVer�� �־���. object���� ������ �� �ֵ���
	SetMinVer(CFbx::GetInstance()->GetMinVer());		// ���� �����ͼ� CMesh�� m_MinVer�� �־���. object���� ������ �� �ֵ���

	/*D3DXVECTOR3 tempa = CFbx::GetInstance()->GetMaxVer();
	D3DXVECTOR3 tempb = CFbx::GetInstance()->GetMinVer();

	printf("max x : %f, max y : %f, ,max z : %f\n", tempa.x, tempa.y, tempa.z);
	printf("min x : %f, min y : %f, ,min z : %f\n", tempb.x, tempb.y, tempb.z);*/

	CFbx::GetInstance()->Fbx_ReadTextFile_Weight(CharNum, ppVertices);

	m_nStride = sizeof(CAnimationVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	CalculateVertexNormal((BYTE *)ppVertices, NULL);
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = ppVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);
	CreateRasterizerState(pd3dDevice);

}

CMyAni::~CMyAni()
{
	CMesh::~CMesh();
}

void CMyAni::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CMyAni::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

void CMyAni::CalculateVertexNormal(BYTE *pVertices, WORD *pIndices)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!pIndices)
			SetTriAngleListVertexNormal(pVertices);
		else
			SetAverageVertexNormal(pVertices, pIndices, (m_nIndices / 3), 3, false);
		break;

	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pVertices, pIndices, (pIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

void CMyAni::SetTriAngleListVertexNormal(BYTE *pVertices)
{
	D3DXVECTOR3 d3dxvNormal;
	CTexturedNormalVertex *pVertex = NULL;

	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pVertex = (CTexturedNormalVertex *)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
		pVertex = (CTexturedNormalVertex *)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
		pVertex = (CTexturedNormalVertex *)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->SetNormal(d3dxvNormal);
	}
}

D3DXVECTOR3 CMyAni::CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{
	D3DXVECTOR3 d3dxvNormal(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 d3dxvP0 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex0)));
	D3DXVECTOR3 d3dxvP1 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex1)));
	D3DXVECTOR3 d3dxvP2 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex2)));
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);
	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	return(d3dxvNormal);
}

void CMyAni::SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip)
{
	D3DXVECTOR3 d3dxvSumOfNormal(0.0f, 0.0f, 0.0f);
	CTexturedNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2);
		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pVertex = (CTexturedNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->SetNormal(d3dxvSumOfNormal);
	}
}

//===============================================
//
// bounding box mesh
//
//===============================================

CBoundingBoxMesh::CBoundingBoxMesh(ID3D11Device *pd3dDevice, CMesh *pMesh) : CMesh(pd3dDevice)
{
	D3DXVECTOR3 vMin = pMesh->GetMinVer();
	D3DXVECTOR3 vMax = pMesh->GetMaxVer();

	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	//������Ƽ�� ������ �ﰢ�� ��Ʈ��(Triangle Strip)���� �����Ѵ�.
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(vMin.x, vMax.y, vMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(vMax.x, vMax.y, vMax.z), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(vMin.x, vMax.y, vMin.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[3] = CDiffusedVertex(D3DXVECTOR3(vMax.x, vMax.y, vMin.z), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pVertices[4] = CDiffusedVertex(D3DXVECTOR3(vMin.x, vMin.y, vMax.z), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pVertices[5] = CDiffusedVertex(D3DXVECTOR3(vMax.x, vMin.y, vMax.z), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	pVertices[6] = CDiffusedVertex(D3DXVECTOR3(vMin.x, vMin.y, vMin.z), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	pVertices[7] = CDiffusedVertex(D3DXVECTOR3(vMax.x, vMin.y, vMin.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	/*�ﰢ�� ��Ʈ��(Triangle Strip)�� ��� �� �ﰢ���� ������ 2���� ������ ���ο� ���� 1���� ����Ͽ� �����ȴ�. �׸��� �ﰢ���� ���ε� ������ �ð����� �ݽð������ ������ ��Ÿ���� �Ѵ�. �ݽð������ �������� Direct3D���� ���������� �ð�������� �ٲ�� �ȴ�. ������ü�� �ﰢ�� ��Ʈ������ ǥ���� �� ������ ���� ���� �ε����� ����ϴ� �ϳ��� ����� <�׸� 2>�� ���� 16���� �ε����� ����ϴ� ���̴�. Direct3D���� �ﰢ���� �׸� �� �ﰢ���� �� �������� ���������� �ﰢ���� ��Ÿ�� �� ������ �׷����� �ʰ� �ȴ�. ���� ���, �� �� �̻��� ���� ������ ���̰ų� �� ���� ���� �ϳ��� ���� ���� �ִ� ��쿡�� �ﰢ���� �׷����� �ʴ´�(Degenerated Triangle). ���� �ﰢ�� ��Ʈ������ �ð������ �ﰢ���� �׸��� ������ �ݽð������ �������� ������ �ﰢ���� �־����� �׷����� �ʴ´�. �ݽð������ ��쿡 �ð������ �������� ������ �ﰢ���� �־��� ���� �׷����� ���� ���̴�.*/
	m_nIndices = 18;
	WORD pIndices[18];
	pIndices[0] = 5; //5,6,4 - cw
	pIndices[1] = 6; //6,4,7 - ccw
	pIndices[2] = 4; //4,7,0 - cw
	pIndices[3] = 7; //7,0,3 - ccw
	pIndices[4] = 0; //0,3,1 - cw
	pIndices[5] = 3; //3,1,2 - ccw
	pIndices[6] = 1; //1,2,2 - cw 
	pIndices[7] = 2; //2,2,3 - ccw
	pIndices[8] = 2; //2,3,3 - cw  - Degenerated Index(2)
	pIndices[9] = 3; //3,3,7 - ccw - Degenerated Index(3)
	pIndices[10] = 3;//3,7,2 - cw  - Degenerated Index(3)
	pIndices[11] = 7;//7,2,6 - ccw
	pIndices[12] = 2;//2,6,1 - cw
	pIndices[13] = 6;//6,1,5 - ccw
	pIndices[14] = 1;//1,5,0 - cw
	pIndices[15] = 5;//5,0,4 - ccw
	pIndices[16] = 0;
	pIndices[17] = 4;

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD)* m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	CreateRasterizerState(pd3dDevice);
}

CBoundingBoxMesh::~CBoundingBoxMesh()
{

}

void CBoundingBoxMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{

	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}
void CBoundingBoxMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}