#pragma once

#include "Object.h"





//
//	Shader - base class
//
class CShader
{
public:
	CShader();
	~CShader();

	virtual void CreateShader() {}
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables(const XMFLOAT4X4 *pd3dxmtxWorld);
	virtual void UpdateShaderVariables(CMaterial *pMaterial = nullptr);
	virtual void UpdateShaderVariables(CTexture *pTexture);

	void InsertObject(CObject *pObject);
	bool ReleaseObject(UINT id);
	void ReleaseAllObjects();

	virtual void AnimateObjectAndRender();

protected:
	vector<CObject*> m_vObjects;

	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	ID3D11PixelShader *m_pd3dPixelShader;

	ID3D11GeometryShader *m_pd3dGeometryShader;

	ID3D11Buffer *m_pd3dcbWorldMatrix;
	ID3D11Buffer *m_pd3dcbMaterial;

	void CreateVertexShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	void CreatePixelShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);
};





//
//	Illuminated Textured Shader
//	애니메이션이 없는 물체를 위한 셰이더. 텍스처 매핑, 빛 사용
//
class CIlluminatedTexturedShader : public CShader
{
public:
	CIlluminatedTexturedShader();
	~CIlluminatedTexturedShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables(const XMFLOAT4X4 *pd3dxmtxWorld);
	virtual void UpdateShaderVariables(CMaterial *pMaterial = nullptr);
	virtual void UpdateShaderVariables(CTexture *pTexture);
};





//
//	Animating Shader
//	애니메이션이 있는 물체를 위한 셰이더. 텍스처 매핑, 빛, 애니메이션 사용
//
class CAnimatingShader : public CShader
{
public:
	CAnimatingShader();
	virtual ~CAnimatingShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
};






