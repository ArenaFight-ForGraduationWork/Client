#pragma once

#include "Object.h"
#include "d3dx11effect.h"


class CShader
{
public:
	CShader();
	~CShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables(CXMMATRIX pd3dxmtxWorld);
	virtual void UpdateShaderVariables(CMaterial *pMaterial = nullptr);
	virtual void UpdateShaderVariables(CTexture *pTexture);

	void InsertObject(CObject *pObject);
	bool ReleaseObject(UINT id);
	void ReleaseAllObjects();

	virtual void AnimateObjectAndRender(float time);

protected:
	std::vector<CObject*> m_vObjects;

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





/* 애니메이션이 없는 물체를 위한 셰이더.
	텍스처 매핑, 빛 사용						*/
class CIlluminatedTexturedShader : public CShader
{
public:
	CIlluminatedTexturedShader();
	~CIlluminatedTexturedShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables(CMaterial *pMaterial = nullptr);
};




/* 애니메이션이 있는 물체를 위한 셰이더.
	텍스처 매핑, 빛, 애니메이션 사용			*/
class CAnimatingShader : public CShader
{
public:
	CAnimatingShader();
	virtual ~CAnimatingShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
};



class CTextureShader : public CShader
{
public:
	CTextureShader();
	CTextureShader(const CTextureShader& ref) { }
	~CTextureShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
	virtual void AnimateObjectAndRender(float time);
};


class CParticleEffect
{
public:
	CParticleEffect(char *pFilename);
	CParticleEffect(const CParticleEffect &ref) {}
	~CParticleEffect();

	ID3DX11Effect* mFX;

	ID3DX11EffectTechnique* StreamOutTech;
	ID3DX11EffectTechnique* DrawTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectScalarVariable* GameTime;
	ID3DX11EffectScalarVariable* TimeStep;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* EmitPosW;
	ID3DX11EffectVectorVariable* EmitDirW;
	ID3DX11EffectShaderResourceVariable* TexArray;
	ID3DX11EffectShaderResourceVariable* RandomTex;

	void SetViewProj(CXMMATRIX M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetGameTime(float f) { GameTime->SetFloat(f); }
	void SetTimeStep(float f) { TimeStep->SetFloat(f); }

	void SetEyePosW(CXMVECTOR v) { EyePosW->SetRawValue(&v, 0, sizeof(XMVECTOR)); }
	void SetEmitPosW(CXMVECTOR v) { EmitPosW->SetRawValue(&v, 0, sizeof(XMVECTOR)); }
	void SetEmitDirW(CXMVECTOR v) { EmitDirW->SetRawValue(&v, 0, sizeof(XMVECTOR)); }

	void SetTexArray(ID3D11ShaderResourceView* tex) { TexArray->SetResource(tex); }
	void SetRandomTex(ID3D11ShaderResourceView* tex) { RandomTex->SetResource(tex); }
};
class CParticleShader : public CShader
{
public:
	CParticleShader(char *pFilename);
	CParticleShader(const CParticleShader &ref) {}
	virtual ~CParticleShader();

	virtual void CreateShader();
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables(CXMMATRIX worldMatrix = XMMatrixIdentity());
	virtual void UpdateShaderVariables(CTexture *pTexture);

	virtual void BuildObjects() {}
	virtual void ReleaseObjects() {}
	virtual void Render();

	void SetInputLayout();

	CParticleEffect *m_particle;
};
