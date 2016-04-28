#ifndef FOG_H_
#define FOG_H_



struct VS_CB_FOG_CENTER
{
	float m_fEnable;
	D3DXVECTOR3 m_d3dxvCenter;
};
struct VS_CB_FOG_RANGE
{
	float m_fRange;
	float padding1, padding2, padding3;
};
struct PS_CB_FOG_COLOR
{
	D3DXVECTOR3 m_d3dxvColor;
	float padding;
};



class CFog
{
public:
	CFog();
	~CFog();

	void Initialize(ID3D11Device *pd3dDevice);
	void Destroy();

	void Update(ID3D11Device *pd3dDevice);
	void Expand(ID3D11Device *pd3dDevice, const D3DXVECTOR3 *pd3dvCenter);
	void Contract(ID3D11Device *pd3dDeivce);

	//void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const float fRange);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvColor);

private:
	enum class eState : BYTE{
		START = 0,
		DISABLE = 0,
		KEEP,
		EXPAND,		// 확장
		CONTRACT,	// 수축
		END
	};
	eState m_eState;

	D3DXVECTOR3 *m_pd3dxvCenter;

	float m_fNowRange;
	float m_fMinRange;
	float m_fMaxRange;

	ID3D11Buffer *m_pd3dcbFogCenter;
	ID3D11Buffer *m_pd3dcbFogRange;

	ID3D11Buffer *m_pd3dcbFogColor;
};



#endif