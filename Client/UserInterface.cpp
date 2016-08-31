#include "stdafx.h"
#include "UserInterface.h"


CUserInterface::CUserInterface()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_pTexture = nullptr;
}
CUserInterface::CUserInterface(const CUserInterface& ref)
{
}
CUserInterface::~CUserInterface()
{
	Destroy();
}

void CUserInterface::Initialize(int screenWidth, int screenHeight)
{
	// ȭ�� ũ�⸦ �����Ѵ�
	m_iScreenWidth = screenWidth;
	m_iScreenHeight = screenHeight;

	// ���� ���� ��ġ�� -1�� �ʱ�ȭ
	m_iPreviousPosX = -1;
	m_iPreviousPosY = -1;

	// ���ؽ����ۿ� �ε������۸� �ʱ�ȭ�Ѵ�
	// Initialize Buffers
	{
		CTexturedVertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		m_iVertexSize = 6;
		m_iIndexSize = m_iVertexSize;
		vertices = new CTexturedVertex[m_iVertexSize];
		indices = new unsigned long[m_iIndexSize];
		memset(vertices, 0, (sizeof(CTexturedVertex) * m_iVertexSize));
		for (int i = 0; i < m_iIndexSize; ++i)
		{
			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(CTexturedVertex) * m_iVertexSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		gpCommonState->m_pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexSize;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		gpCommonState->m_pd3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;
	}

	// initialize texture
	m_pTexture = new CTexture(1);
}

void CUserInterface::Destroy()
{
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
}

void CUserInterface::SetTexture(WCHAR *pFilePath, int bitmapWidth, int bitmapHeight)
{
	// ��Ʈ���� �׷����� �ϴ� ���� �ȼ� ũ�� ����
	m_iBitmapWidth = bitmapWidth;
	m_iBitmapHeight = bitmapHeight;

	m_pTexture->SetTexture(0, pFilePath);
}

void CUserInterface::ChangeSize(int bitmapWidth, int bitmapHeight)
{
	m_iBitmapWidth = bitmapWidth;
	m_iBitmapHeight = bitmapHeight;
}

void CUserInterface::Render(int positionX, int positionY)
{
	// 1) buffer update
	for (;;)
	{
		float left, right, top, bottom;
		CTexturedVertex *pVertices;

		//// �̹����� ��ġ�� ������ ���Ͽ� �޶����°� ��. �� �ٲ����� update�κ��� �׳� ����ģ��.
		//if ((positionX == m_iPreviousPosX) && (positionY == m_iPreviousPosY))	break;

		// �̹����� ��ġ�� �ٲ����� ���ο� ��ġ�� ����Ѵ�
		m_iPreviousPosX = positionX;
		m_iPreviousPosY = positionY;

		// �̹����� �� ���� ��ġ ���
		left = static_cast<float>((m_iScreenWidth / 2) * -1) + static_cast<float>(positionX);
		right = left + static_cast<float>(m_iBitmapWidth);
		top = static_cast<float>((m_iScreenHeight / 2)) - static_cast<float>(positionY);
		bottom = top - static_cast<float>(m_iBitmapHeight);

		// �ӽ÷� ���� �迭�� ����� �� ���� ��ġ�� ä���ִ´�
		pVertices = new CTexturedVertex[m_iVertexSize];

		// �ﰢ��1
		pVertices[0].SetPosition(left, top, 0.0f); // Top Left
		pVertices[0].SetTexCoord(0.0f, 0.0f);
		pVertices[1].SetPosition(right, bottom, 0.0f);	// Bottom Right
		pVertices[1].SetTexCoord(1.0f, 1.0f);
		pVertices[2].SetPosition(left, bottom, 0.0f);	// Bottome Left
		pVertices[2].SetTexCoord(0.0f, 1.0f);

		// �ﰢ��2
		pVertices[3].SetPosition(left, top, 0.0f);	// Top Left
		pVertices[3].SetTexCoord(0.0f, 0.0f);
		pVertices[4].SetPosition(right, top, 0.0f);	// Top Right
		pVertices[4].SetTexCoord(1.0f, 0.0f);
		pVertices[5].SetPosition(right, bottom, 0.0f);	// Bottome Right
		pVertices[5].SetTexCoord(1.0f, 1.0f);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		gpCommonState->m_pd3dDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CTexturedVertex *pVerticesPtr = (CTexturedVertex*)mappedResource.pData;
		memcpy(pVerticesPtr, (void*)pVertices, (sizeof(CTexturedVertex) * m_iVertexSize));
		gpCommonState->m_pd3dDeviceContext->Unmap(m_pVertexBuffer, 0);

		delete[] pVertices;
		pVertices = 0;

		break;
	}

	// 2) buffer render
	{
		unsigned int stride = sizeof(CTexturedVertex);
		unsigned int offset = 0;

		gpCommonState->m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		gpCommonState->m_pd3dDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		gpCommonState->m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}