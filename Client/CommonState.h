#pragma once

#include "Timer.h"



//	CCommonState *gpCommonState;


//
//	Common State
//
class CCommonState
{
public:
	static CCommonState* GetSingleton()
	{
		static CCommonState instance;
		return &instance;
	}
	void Initialize(HWND hWnd);
	//void Destroy();

	ID3D11Device* GetDevice() { return m_pd3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }

	IDXGISwapChain* GetSwapChain() { return m_pDXGISwapChain; }

	ID3D11DepthStencilState* GetDepthStencilDefault() { return m_pd3dDepthStencilDefault; }
	ID3D11DepthStencilState* GetDepthStencilDisable() { return m_pd3dDepthStencilDisable; }

	CGameTimer* GetTimer() { return m_pTimer; }

	// turn off z buffer after rendering 3d screen, render 2d screen and turn on z buffer finally
	void TurnZBufferOn();
	void TurnZBufferOff();

	// Alpha Blending
	void EnableAlphaBlending();
	void DisableAlphaBlending();

protected:
private:
	CCommonState();
	CCommonState(CCommonState&& ref) {}
	~CCommonState();

	ID3D11Device *m_pd3dDevice;	// �ַ� ���ҽ� ������ ���� ���
	ID3D11DeviceContext *m_pd3dDeviceContext;	// �ַ� ���������� ������ ���� ���

	IDXGISwapChain *m_pDXGISwapChain;	// �ַ� ���÷��� ������ �� ���

	ID3D11DepthStencilState* m_pd3dDepthStencilDefault;
	ID3D11DepthStencilState* m_pd3dDepthStencilDisable;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	CGameTimer *m_pTimer;

	bool _CreateDevices(HWND hWnd);
	bool _CreateDepthStencilState();
	bool _CreateBlendState();
};

extern CCommonState *gpCommonState;