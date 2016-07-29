#ifndef CUSERINTERFACE_H_
#define CUSERINTERFACE_H_
#include "Object.h"


class CUserInterface
{
public:
	CUserInterface();
	CUserInterface(const CUserInterface&);
	~CUserInterface();

	void Initialize(ID3D11Device *pd3dDevice, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight);
	void Destroy();

	void Render(ID3D11DeviceContext *pd3dDeviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();



	/* pos + tex = CDiffusedVertex */
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

	void Update();
	void LoadTexture();
	void ReleaseTexture();
};



#endif