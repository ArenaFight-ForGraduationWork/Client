#ifndef SHADER_H_
#define SHADER_H_

#include "Object.h"



struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
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
	void ReleaseObject(UINT id);
	void ReleaseAllObjects();

	//virtual void BuildObjects(ID3D11Device *pd3dDevice);
	//virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

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
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = nullptr);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CMaterial *pMaterial = nullptr);

	//virtual void BuildObjects(ID3D11Device *pd3dDevice);
	//virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};





// 조명을 사용하지 않고 렌더링하기 위한 CDiffusedShader 클래스
class CDiffusedShader : public CShader
{
public:
	CDiffusedShader();
	~CDiffusedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = nullptr);

	//virtual void BuildObjects(ID3D11Device *pd3dDevice);
	//virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};



class CPlayerShader : public CDiffusedShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = nullptr);

	//virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};




#endif