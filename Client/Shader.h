#ifndef SHADER_H_
#define SHADER_H_

#include "Object.h"



struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
};

struct VS_CB_FOG
{
	float fogStart;
	float fogEnd;
	float padding1, padding2;
};





class CShader
{
public:
	CShader();
	~CShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = nullptr);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CMaterial *pMaterial = nullptr);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture);

	void InsertObject(CObject *pObject);
	bool ReleaseObject(UINT id);
	void ReleaseAllObjects();

	virtual void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);	//수정
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	//애니메이션+ 렌더
	virtual void AnimateObjectAndRender(ID3D11DeviceContext*, float time);

protected:
	std::vector<CObject*> m_vObjects;

	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	ID3D11PixelShader *m_pd3dPixelShader;

	ID3D11GeometryShader *m_pd3dGeometryShader;

	ID3D11Buffer *m_pd3dcbWorldMatrix;
	ID3D11Buffer *m_pd3dcbMaterial;

	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);
};







// 텍스쳐 맵핑과 조명을 사용하여 렌더링하기 위한 CIlluminatedTexturedShader 클래스
class CIlluminatedTexturedShader : public CShader
{
public:
	CIlluminatedTexturedShader();
	~CIlluminatedTexturedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CMaterial *pMaterial = nullptr);

	virtual void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext,float fTimeElapsed);	//수정
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

};






class CFogShader : public CShader
{
public:
	CFogShader();
	~CFogShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CMaterial *pMaterial = nullptr);

	//virtual void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext,float fTimeElapsed);	
	//virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

private:
	ID3D11Buffer *m_pd3dcbFog;

};



class CPlayerShader : public CIlluminatedTexturedShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	
	virtual void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

#endif