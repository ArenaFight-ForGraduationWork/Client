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

	ID3D11Device* GetDevice() { return m_pd3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }

	//�������� �޽����� ó���ϴ� �Լ��̴�. 
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SetID(UINT id) { m_uiID = id; }
	UINT& GetID() { return m_uiID; }

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	//����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	ID3D11Device *m_pd3dDevice;
	//���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	IDXGISwapChain *m_pDXGISwapChain;
	//����̽� ���ؽ�Ʈ�� ���� �������̴�. �ַ� ���������� ������ �ϱ� ���Ͽ� �ʿ��ϴ�.
	ID3D11DeviceContext *m_pd3dDeviceContext;
	//���� Ÿ�� �� �������̽��� ���� �������̴�. 
	ID3D11RenderTargetView *m_pd3dRenderTargetView;

	// ���� �����ӿ�ũ���� ����� Ÿ�̸�
	CGameTimer m_GameTimer;

	// ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ�
	_TCHAR m_pszBuffer[50];

	// ���� ����(Depth Buffer) ��ü �������̽��� �信 ���� ������
	ID3D11Texture2D *m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView *m_pd3dDepthStencilView;

	ID3D11DepthStencilState* m_pd3dDepthStencilState;
	ID3D11DepthStencilState* m_pd3dDepthDisabledStencilState;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	CObjectManager *m_pObjectManager;
	CSceneManager *m_pSceneManager;
	CCameraManager *m_pCameraManager;

	UINT m_uiID;

	// turn off z buffer after rendering 3d screen, render 2d screen and turn on z buffer finally
	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
};



#endif