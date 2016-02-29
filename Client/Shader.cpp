#include "stdafx.h"
#include "Shader.h"

// http://blog.naver.com/snrnsrk06/100202723546 (Effect.fx)


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
}

//void CShader::BuildObjects(ID3D11Device *pd3dDevice)
//{
//}

//void CShader::ReleaseObjects()
//{
//	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
//	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();
//
//	//if (m_ppObjects)
//	//{
//	//	for (int j = 0; j < m_nObjects; j++) delete m_ppObjects[j];
//	//	delete[] m_ppObjects;
//	//}
//}

void CShader::InsertObject(CObject *pObject)
{
	m_vObjects.push_back(pObject);
}

void CShader::ReleaseObject(UINT id)
{
	for (auto p = m_vObjects.begin(); p != m_vObjects.end();)
	{
		if (id == (*p)->GetId())
		{
			p = m_vObjects.erase(p);
			break;
		}
		else
			++p;
	}
}

void CShader::ReleaseAllObjects()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();
	m_vObjects.clear();
}

void CShader::AnimateObjects(float fTimeElapsed)
{
	for (auto obj : m_vObjects)
		obj->Animate(fTimeElapsed);
}

void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 정점-쉐이더를 생성한다. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//컴파일된 쉐이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
}

void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}
void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CMaterial *pMaterial)
{
}
void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CTexture *pTexture)
{
	pd3dDeviceContext->PSSetShaderResources(PS_SLOT_TEXTURE, pTexture->GetNumOfTextures(), pTexture->GetShaderResourceViewTextures());
	pd3dDeviceContext->PSSetSamplers(PS_SLOT_SAMPLER_STATE, pTexture->GetNumOfTextures(), pTexture->GetSamplerState());
}


void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	//정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	for (auto obj : m_vObjects)
	{
		UpdateShaderVariables(pd3dDeviceContext, obj->GetWorldMatrix());
		if (obj->GetMaterial())
			UpdateShaderVariables(pd3dDeviceContext, obj->GetMaterial());
		if (obj->GetTexture())
			UpdateShaderVariables(pd3dDeviceContext, obj->GetTexture());
		obj->Render(pd3dDeviceContext);
	}

}





CDiffusedShader::CDiffusedShader()
{
}

CDiffusedShader::~CDiffusedShader()
{
}

void CDiffusedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSDiffusedColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSDiffusedColor", "ps_4_0", &m_pd3dPixelShader);
}

void CDiffusedShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
}

void CDiffusedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

//void CDiffusedShader::BuildObjects(ID3D11Device *pd3dDevice)
//{
//	CreateShaderVariables(pd3dDevice);
//}
//
//void CDiffusedShader::ReleaseObjects()
//{
//	CShader::ReleaseAllObjects();
//}

void CDiffusedShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}

void CDiffusedShader::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::Render(pd3dDeviceContext);
}





CIlluminatedTexturedShader::CIlluminatedTexturedShader()
{
}

CIlluminatedTexturedShader::~CIlluminatedTexturedShader()
{
}

void CIlluminatedTexturedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSTexturedLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSTexturedLighting", "ps_4_0", &m_pd3dPixelShader);
}
void CIlluminatedTexturedShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);
}

void CIlluminatedTexturedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CIlluminatedTexturedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, CMaterial *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL *pcbMaterial = (MATERIAL *)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial->GetMaterial(), sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
//
//void CIlluminatedTexturedShader::BuildObjects(ID3D11Device *pd3dDevice)
//{
//	CMaterial **ppMaterials = new CMaterial*[3];
//	ppMaterials[0] = new CMaterial();
//	ppMaterials[0]->GetMaterial()->m_d3dxcDiffuse = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
//	ppMaterials[0]->GetMaterial()->m_d3dxcAmbient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
//	ppMaterials[0]->GetMaterial()->m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 5.0f);
//	ppMaterials[0]->GetMaterial()->m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
//	ppMaterials[1] = new CMaterial();
//	ppMaterials[1]->GetMaterial()->m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
//	ppMaterials[1]->GetMaterial()->m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
//	ppMaterials[1]->GetMaterial()->m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 10.0f);
//	ppMaterials[1]->GetMaterial()->m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
//	ppMaterials[2] = new CMaterial();
//	ppMaterials[2]->GetMaterial()->m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
//	ppMaterials[2]->GetMaterial()->m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
//	ppMaterials[2]->GetMaterial()->m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.0f, 0.5f, 10.0f);
//	ppMaterials[2]->GetMaterial()->m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
//
//	ID3D11SamplerState *pd3dSamplerState = NULL;
//	D3D11_SAMPLER_DESC d3dSamplerDesc;
//	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
//	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	d3dSamplerDesc.MinLOD = 0;
//	d3dSamplerDesc.MaxLOD = 0;
//	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);
//
//	ID3D11ShaderResourceView *pd3dTexture = NULL;
//	CTexture **ppTextures = new CTexture*[3];
//	ppTextures[0] = new CTexture(1);
//	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Stone01.jpg"), NULL, NULL, &pd3dTexture, NULL);
//	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);
//	ppTextures[1] = new CTexture(1);
//	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Brick01.jpg"), NULL, NULL, &pd3dTexture, NULL);
//	ppTextures[1]->SetTexture(0, pd3dTexture, pd3dSamplerState);
//	ppTextures[2] = new CTexture(1);
//	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Data\\Wood01.jpg"), NULL, NULL, &pd3dTexture, NULL);
//	ppTextures[2]->SetTexture(0, pd3dTexture, pd3dSamplerState);
//
//	CMesh *pMeshIlluminatedTextured = new CCubeMeshIlluminatedTextured(pd3dDevice, 12.0f, 12.0f, 12.0f);
//
//	int xObjects = 3, yObjects = 3, zObjects = 3, i = 0, nObjectTypes = 2;
//
//	float fxPitch = 12.0f * 1.7f;
//	float fyPitch = 12.0f * 1.7f;
//	float fzPitch = 12.0f * 1.7f;
//	CObject *pObject = nullptr;
//	for (int x = -xObjects; x <= xObjects; x++)
//	{
//		for (int y = -yObjects; y <= yObjects; y++)
//		{
//			for (int z = -zObjects; z <= zObjects; z++)
//			{
//				pObject = new CObject(i);
//				pObject->SetMesh(pMeshIlluminatedTextured);
//				pObject->SetMaterial(ppMaterials[i % 3]);
//				pObject->SetTexture(ppTextures[i % 3]);
//				pObject->MoveAbsolute((x*(fxPitch*nObjectTypes) + 1 * fxPitch), (y*(fyPitch*nObjectTypes) + 1 * fyPitch), (z*(fzPitch*nObjectTypes) + 1 * fzPitch));
//				i += 1;
//				m_vObjects.push_back(pObject);
//			}
//		}
//	}
//
//	CreateShaderVariables(pd3dDevice);
//
//	delete[] ppTextures;
//	delete[] ppMaterials;
//}
//
//void CIlluminatedTexturedShader::ReleaseObjects()
//{
//	CShader::ReleaseAllObjects();
//}

void CIlluminatedTexturedShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}

void CIlluminatedTexturedShader::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::Render(pd3dDeviceContext);
}





CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}
//
//void CPlayerShader::ReleaseObjects()
//{
//	CDiffusedShader::ReleaseObjects();
//}

void CPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CDiffusedShader::CreateShader(pd3dDevice);
}

void CPlayerShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CDiffusedShader::CreateShaderVariables(pd3dDevice);
}

void CPlayerShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CDiffusedShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CPlayerShader::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CDiffusedShader::Render(pd3dDeviceContext);
}

