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

	// ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ�
	ID3D11Device *m_pd3dDevice;
	// ����̽� ���ؽ�Ʈ�� ���� �������̴�. �ַ� ���������� ������ �ϱ� ���Ͽ� �ʿ�
	ID3D11DeviceContext *m_pd3dDeviceContext;

protected:
private:
	CCommonState();
	CCommonState(CCommonState&& ref) {}
	~CCommonState();
};

extern CCommonState *gpCommonState;


#endif