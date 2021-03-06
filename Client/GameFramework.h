#pragma once

#include "ObjectManager.h"
#include "Scene.h"
#include "Camera.h"





//
//	Game Framework
//
class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	// 디바이스, 스왑 체인, 디바이스 컨텍스트, 디바이스와 관련된 뷰를 생성하는 함수
	bool CreateRenderTargetDepthStencilView();
	bool CreateRenderTargetView();
	bool CreateDirect3DDisplay();

	// 프레임워크의 핵심(사용자 입력, 애니메이션, 렌더링)을 구성하는 함수
	void FrameAdvance();

	//윈도우의 메시지를 처리하는 함수이다. 
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	// 렌더 타겟 뷰 인터페이스에 대한 포인터
	ID3D11RenderTargetView *m_pd3dRenderTargetView;

	// 깊이 버퍼(Depth Buffer) 객체 인터페이스와 뷰에 대한 포인터
	ID3D11Texture2D *m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView *m_pd3dDepthStencilView;

	// 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열
	_TCHAR m_pszBuffer[50];

	CObjectManager *m_pObjectManager;
	CSceneManager *m_pSceneManager;
	CCameraManager *m_pCameraManager;
};


