#ifndef FOG_H_
#define FOG_H_



struct VS_CB_FOG_CENTER
{
	D3DXVECTOR3 m_d3dxvCenter;
	float padding;
};
struct VS_CB_FOG_RANGE
{
	float m_fRange;
	float padding1, padding2, padding3;
};



class CFog
{
public:
	CFog();
	~CFog();

	void Initialize(ID3D11Device *pd3dDevice, const D3DXVECTOR3 *pd3dxvCenter, const float fRange);
	void Destroy();

	//void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const float fRange);

private:
	ID3D11Buffer *m_pd3dcbFogCenter;
	ID3D11Buffer *m_pd3dcbFogRange;
};



#endif