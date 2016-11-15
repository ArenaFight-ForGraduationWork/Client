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

	HWND GetWindowHandle() { return m_hWnd; }

	IDXGISwapChain* GetSwapChain() { return m_pDXGISwapChain; }

	ID3D11DepthStencilState* GetDepthStencilDefault() { return m_pd3dDepthStencilDefault; }
	ID3D11DepthStencilState* GetDepthStencilDisable() { return m_pd3dDepthStencilDisable; }

	CGameTimer* GetTimer() { return m_pTimer; }

	/*
		for 2d Rendering
		1) Render 3d screen
		2) Turn off z-buffer ( TurnZBufferOff()	)
		3) Render 2d screen
		4) Turn on z-buffer ( TurnZBufferOn() )
	*/
	void TurnZBufferOn();
	void TurnZBufferOff();

	/*
		for Alpha Blending
		1) Turn on alpha blending ( EnableAlphaBlending() )
		2) Render particle system
		3) Turn off alpha blending ( DisableAlphaBlending() )
	*/
	void EnableAlphaBlending();
	void DisableAlphaBlending();

protected:
private:
	CCommonState();
	CCommonState(CCommonState&& ref) {}
	~CCommonState();

	ID3D11Device *m_pd3dDevice;	// 주로 리소스 생성할 때에 사용
	ID3D11DeviceContext *m_pd3dDeviceContext;	// 주로 파이프라인 설정할 때에 사용

	HWND m_hWnd;

	IDXGISwapChain *m_pDXGISwapChain;	// 주로 디스플레이 제어할 때 사용

	ID3D11DepthStencilState* m_pd3dDepthStencilDefault;
	ID3D11DepthStencilState* m_pd3dDepthStencilDisable;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	CGameTimer *m_pTimer;

	bool _CreateDevices();
	bool _CreateDepthStencilState();
	bool _CreateBlendState();
};

extern CCommonState *gpCommonState;