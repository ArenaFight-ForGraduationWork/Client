#ifndef COMMONSTATE_H_
#define COMMONSTATE_H_

#include "stdafx.h"
#include "Timer.h"



/*
	CCommonState *gpCommonState;
*/

class CCommonState
{
public:
	static CCommonState* GetSingleton()
	{
		static CCommonState instance;
		return &instance;
	}
	//void Destroy();

	// 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요
	ID3D11Device *m_pd3dDevice;
	// 디바이스 컨텍스트에 대한 포인터이다. 주로 파이프라인 설정을 하기 위하여 필요
	ID3D11DeviceContext *m_pd3dDeviceContext;

	ID3D11DepthStencilState* m_pd3dDepthStencilDefault;
	ID3D11DepthStencilState* m_pd3dDepthStencilDisable;

	CGameTimer *m_pTimer;

	// turn off z buffer after rendering 3d screen, render 2d screen and turn on z buffer finally
	void TurnZBufferOn();
	void TurnZBufferOff();

	float GetTimeElapsed() { return m_pTimer->GetTimeElapsed(); }
	int GetNowTime() { return m_pTimer->GetNowTime(); }
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0) { return m_pTimer->GetFrameRate(lpszString, nCharacters); }
	void SetTick(float fLockFPS = 0.0f) { m_pTimer->Tick(fLockFPS); }

protected:
private:
	CCommonState();
	CCommonState(CCommonState&& ref) {}
	~CCommonState();
};

extern CCommonState *gpCommonState;


#endif