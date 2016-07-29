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
	// 화면 크기를 저장한다
	m_iScreenWidth = screenWidth;
	m_iScreenHeight = screenHeight;

	// 비트맵이 그려져야 하는 곳의 픽셀을 저장한다
	m_iBitmapWidth = bitmapWidth;
	m_iBitmapHeight = bitmapHeight;

	// 이전 렌더 위치를 -1로 초기화
	m_iPreviousPosX = -1;
	m_iPreviousPosY = -1;

	//// 버텍스버퍼와 인덱스버퍼를 초기화한다
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