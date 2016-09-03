#ifndef GAMEFRAMEWORK_H_
#define GAMEFRAMEWORK_H_

#include "Timer.h"
#include "Scene.h"
#include "Camera.h"
#include "ObjectManager.h"



class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	// ����̽�, ���� ü��, ����̽� ���ؽ�Ʈ, ����̽��� ���õ� �並 �����ϴ� �Լ�
	bool CreateRenderTargetDepthStencilView();
	bool CreateRenderTargetView();
	bool CreateDirect3DDisplay();

	// �������� �޽�, ��ü�� �����ϰ� �Ҹ��ϴ� �Լ�
	void ReleaseObjects();

	// �����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ�
	void FrameAdvance();

	//�������� �޽����� ó���ϴ� �Լ��̴�. 
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SetID(UINT id) { m_uiID = id; }
	UINT& GetID() { return m_uiID; }

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	// ���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���Ͽ� �ʿ�
	IDXGISwapChain *m_pDXGISwapChain;
	// ���� Ÿ�� �� �������̽��� ���� ������
	ID3D11RenderTargetView *m_pd3dRenderTargetView;

	// ���� ����(Depth Buffer) ��ü �������̽��� �信 ���� ������
	ID3D11Texture2D *m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView *m_pd3dDepthStencilView;

	ID3D11DepthStencilState* m_pd3dDepthStencilState;
	ID3D11DepthStencilState* m_pd3dDepthDisabledStencilState;

	// ���� �����ӿ�ũ���� ����� Ÿ�̸�
	CGameTimer m_GameTimer;

	// ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ�
	_TCHAR m_pszBuffer[50];

	CObjectManager *m_pObjectManager;
	CSceneManager *m_pSceneManager;
	CCameraManager *m_pCameraManager;

	UINT m_uiID;

	// turn off z buffer after rendering 3d screen, render 2d screen and turn on z buffer finally
	void TurnZBufferOn();
	void TurnZBufferOff();
};



#endif