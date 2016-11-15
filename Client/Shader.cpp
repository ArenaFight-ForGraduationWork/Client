#include "stdafx.h"
#include "Shader.h"
#include "ConstantBuffers.h"

// http://blog.naver.com/snrnsrk06/100202723546 (Effect.fx)





//
//	Shader
//
CShader::CShader()
{
	m_pd3dVertexShader = nullptr;
	m_pd3dPixelShader = nullptr;
	m_pd3dVertexLayout = nullptr;

	m_pd3dcbWorldMatrix = nullptr;
	m_pd3dcbMaterial = nullptr;
}

CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();

	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();
}

void CShader::InsertObject(CObject *pObject)
{
	m_vObjects.push_back(pObject);
}

bool CShader::ReleaseObject(UINT id)
{
	//for (auto p = m_vObjects.begin(); p != m_vObjects.end();)
	//{
	//	if (id == (*p)->GetId())
	//	{
	//		p = m_vObjects.erase(p);
	//		return TRUE;
	//	}
	//	else
	//		++p;
	//}
	//return FALSE;
	return TRUE;
}

void CShader::ReleaseAllObjects()
{
	m_vObjects.clear();
}

void CShader::CreateVertexShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, NULL, NULL, &pd3dShaderBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 정점-쉐이더를 생성한다. 
		gpCommonState->GetDevice()->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//컴파일된 쉐이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다. 
		gpCommonState->GetDevice()->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreatePixelShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, NULL, NULL, &pd3dShaderBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		gpCommonState->GetDevice()->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreateGeometryShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, NULL, NULL, &pd3dShaderBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		gpCommonState->GetDevice()->CreateGeometryShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dGeometryShader);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreateShaderVariables()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	gpCommonState->GetDevice()->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);
}

void CShader::UpdateShaderVariables(const XMFLOAT4X4 *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->GetDeviceContext()->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	pcbWorldMatrix->m_mtxWorld = XMMatrixTranspose(XMLoadFloat4x4(pd3dxmtxWorld));
	gpCommonState->GetDeviceContext()->Unmap(m_pd3dcbWorldMatrix, 0);

	gpCommonState->GetDeviceContext()->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}
void CShader::UpdateShaderVariables(CMaterial *pMaterial)
{
}
void CShader::UpdateShaderVariables(CTexture *pTexture)
{
	gpCommonState->GetDeviceContext()->PSSetShaderResources(PS_SHADERRESOURCE_SLOT_TEXTURE, pTexture->GetNumOfTextures(), pTexture->GetShaderResourceViewTextures());
	gpCommonState->GetDeviceContext()->PSSetSamplers(PS_SAMPLER_SLOT_SAMPLER_STATE, pTexture->GetNumOfTextures(), pTexture->GetSamplerState());
}

void CShader::AnimateObjectAndRender()
{
	// 정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexLayout)
		gpCommonState->GetDeviceContext()->IASetInputLayout(m_pd3dVertexLayout);
	// 정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexShader)
		gpCommonState->GetDeviceContext()->VSSetShader(m_pd3dVertexShader, NULL, 0);
	// 픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dPixelShader)
		gpCommonState->GetDeviceContext()->PSSetShader(m_pd3dPixelShader, NULL, 0);
	// 기하-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dGeometryShader)
		gpCommonState->GetDeviceContext()->GSSetShader(m_pd3dGeometryShader, NULL, 0);;

	for (auto obj : m_vObjects)
	{
		UpdateShaderVariables(obj->GetWorldMatrix());
		if (obj->GetMaterial())
			UpdateShaderVariables(obj->GetMaterial());
		if (obj->GetTexture())
			UpdateShaderVariables(obj->GetTexture());
		obj->Animate();
		obj->Render();
	}
}





//
//	IllluminatedTexturedShader
//
CIlluminatedTexturedShader::CIlluminatedTexturedShader()
{
}
CIlluminatedTexturedShader::~CIlluminatedTexturedShader()
{
}

void CIlluminatedTexturedShader::CreateShader()
{
	CShader::CreateShader();

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(L"FX/NonAnimating.fx", "VS", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(L"FX/NonAnimating.fx", "PS", "ps_4_0", &m_pd3dPixelShader);
}

void CIlluminatedTexturedShader::CreateShaderVariables()
{
	CShader::CreateShaderVariables();

	// MATERIAL
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(PS_CB_MATERIAL);
	gpCommonState->GetDevice()->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);
}

void CIlluminatedTexturedShader::UpdateShaderVariables(const XMFLOAT4X4 *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dxmtxWorld);
}
void CIlluminatedTexturedShader::UpdateShaderVariables(CMaterial *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->GetDeviceContext()->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	PS_CB_MATERIAL *pcbMaterial = (PS_CB_MATERIAL *)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial->GetMaterial(), sizeof(PS_CB_MATERIAL));
	gpCommonState->GetDeviceContext()->Unmap(m_pd3dcbMaterial, 0);
	gpCommonState->GetDeviceContext()->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
void CIlluminatedTexturedShader::UpdateShaderVariables(CTexture *pTexture)
{
	CShader::UpdateShaderVariables(pTexture);
}





//
//	Animating Shader
//
CAnimatingShader::CAnimatingShader()
{
}
CAnimatingShader::~CAnimatingShader()
{
}

void CAnimatingShader::CreateShader()
{
	CShader::CreateShader();

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 24, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 1, DXGI_FORMAT_R32G32B32A32_UINT, 0, 40, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(L"FX/Animating.fx", "VS", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(L"FX/Animating.fx", "PS", "ps_4_0", &m_pd3dPixelShader);
}

void CAnimatingShader::CreateShaderVariables()
{
	CShader::CreateShaderVariables();

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(PS_CB_MATERIAL);
	gpCommonState->GetDevice()->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);
}




