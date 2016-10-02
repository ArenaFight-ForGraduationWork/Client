#ifndef COMMONSTATE_H_
#define COMMONSTATE_H_

#include "stdafx.h"



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

	// ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ�
	ID3D11Device *m_pd3dDevice;
	// ����̽� ���ؽ�Ʈ�� ���� �������̴�. �ַ� ���������� ������ �ϱ� ���Ͽ� �ʿ�
	ID3D11DeviceContext *m_pd3dDeviceContext;

	ID3D11DepthStencilState* m_pd3dDepthStencilDefault;
	ID3D11DepthStencilState* m_pd3dDepthStencilDisable;

	// turn off z buffer after rendering 3d screen, render 2d screen and turn on z buffer finally
	void TurnZBufferOn();
	void TurnZBufferOff();

protected:
private:
	CCommonState();
	CCommonState(CCommonState&& ref) {}
	~CCommonState();
};

extern CCommonState *gpCommonState;


#endif