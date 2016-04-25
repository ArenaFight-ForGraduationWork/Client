#ifndef FOG_H_
#define FOG_H_



struct VS_CB_FOG
{
	D3DXVECTOR3 m_d3dxvCenter;
	float m_fStart;
	float m_fEnd;
	float padding1, padding2, padding3;
};



class CFog
{
public:
	CFog();
	~CFog();

	void Initialize(ID3D11Device *pd3dDevice, const D3DXVECTOR3 *pd3dxvCenter, const float fFogStart = 500, const float fFogEnd = 1000);
	//void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 pd3dxvCenter);
	void Destroy();

private:
	float m_fStart;
	float m_fEnd;

	ID3D11Buffer *m_pd3dcbFog;
};



#endif