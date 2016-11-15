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

	// 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요
	ID3D11Device *m_pd3dDevice;
	// 디바이스 컨텍스트에 대한 포인터이다. 주로 파이프라인 설정을 하기 위하여 필요
	ID3D11DeviceContext *m_pd3dDeviceContext;

	CGameTimer *m_pTimer;

	IDXGISwapChain* GetSwapChain() { return m_pDXGISwapChain; }

	ID3D11DepthStencilState* GetDepthStencilDefault() { return m_pd3dDepthStencilDefault; }
	ID3D11DepthStencilState* GetDepthStencilDisable() { return m_pd3dDepthStencilDisable; }

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

	IDXGISwapChain *m_pDXGISwapChain;	// 주로 디스플레이 제어할 때 사용

	ID3D11DepthStencilState* m_pd3dDepthStencilDefault;
	ID3D11DepthStencilState* m_pd3dDepthStencilDisable;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	bool _CreateDevices(HWND hWnd);
	bool _CreateDepthStencilState();
	bool _CreateBlendState();
};

extern CCommonState *gpCommonState;