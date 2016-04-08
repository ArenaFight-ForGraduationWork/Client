#ifndef FOG_H_
#define FOG_H_



struct VS_CB_FOG
{
	float fogStart;
	float fogEnd;
	float padding1, padding2;
};



class CFog
{
public:
	CFog();
	~CFog();

	void Initialize(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

private:
	ID3D11Buffer *m_pd3dcbFog;
};



#endif