#ifndef COMMONSTATE_H_
#define COMMONSTATE_H_

#include "stdafx.h"



class CCommonState
{
public:
	static CCommonState* GetSingleton()
	{
		static CCommonState instance;
		return &instance;
	}
	void Destroy();

	// 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요
	ID3D11Device *m_pd3dDevice;
	// 디바이스 컨텍스트에 대한 포인터이다. 주로 파이프라인 설정을 하기 위하여 필요
	ID3D11DeviceContext *m_pd3dDeviceContext;

protected:
private:
	CCommonState();
	CCommonState(CCommonState&& ref) {}
	~CCommonState();
};

extern CCommonState *gpCommonState;


#endif