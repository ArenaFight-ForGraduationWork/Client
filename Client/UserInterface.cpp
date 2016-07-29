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

void CUserInterface::Initialize(ID3D11Device *pd3dDevice, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	// ȭ�� ũ�⸦ �����Ѵ�
	m_iScreenWidth = screenWidth;
	m_iScreenHeight = screenHeight;

	// ��Ʈ���� �׷����� �ϴ� ���� �ȼ��� �����Ѵ�
	m_iBitmapWidth = bitmapWidth;
	m_iBitmapHeight = bitmapHeight;

	// ���� ���� ��ġ�� -1�� �ʱ�ȭ
	m_iPreviousPosX = -1;
	m_iPreviousPosY = -1;

	//// ���ؽ����ۿ� �ε������۸� �ʱ�ȭ�Ѵ�
	//InitializeBuffers(device);

	//WCHAR* textureFilename
	//// Load the texture for this model.
	//LoadTexture(pd3dDevice, textureFilename);
}

void CUserInterface::Destroy()
{
	ReleaseTexture();
}

void CUserInterface::Render(ID3D11DeviceContext *pd3dDeviceContext)
{

}