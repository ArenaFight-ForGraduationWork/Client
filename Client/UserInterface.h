#ifndef CUSERINTERFACE_H_
#define CUSERINTERFACE_H_
#include "Object.h"


class CUserInterface
{
public:
	CUserInterface();
	CUserInterface(const CUserInterface&);
	~CUserInterface();

	void Initialize(ID3D11Device *pd3dDevice, int screenWidth, int screenHeight);
	void Destroy();

	void SetTexture(ID3D11Device *pd3dDevice, WCHAR *pFilePath, int bitmapWidth, int bitmapHeight);
	CTexture* GetTexture() { return m_pTexture; }

	void ChangeSize(int bitmapWidth, int bitmapHeight);

	void Render(ID3D11DeviceContext *pd3dDeviceContext, int positionX, int positionY);

	int GetIndexCount() { return m_iIndexSize; }

protected:
private:
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;

	int m_iVertexSize;
	int m_iIndexSize;

	CTexture *m_pTexture;

	int m_iScreenWidth;
	int m_iScreenHeight;
	int m_iBitmapWidth;
	int m_iBitmapHeight;

	int m_iPreviousPosX;
	int m_iPreviousPosY;
};



#endif