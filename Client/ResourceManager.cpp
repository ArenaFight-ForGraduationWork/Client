#include "stdafx.h"
#include "ResourceManager.h"


void CResource::SetIDs(BYTE meshID, BYTE textureID, BYTE materialID, BYTE shaderID)
{
	m_MeshId = meshID;
	m_TextureId = textureID;
	m_MaterialId = materialID;
	m_ShaderId = shaderID;
}






CResourceManager::CResourceManager(ID3D11Device *pd3dDevice)
{
	_LoadMesh(pd3dDevice);
	_LoadTextures(pd3dDevice);
	_LoadMaterials();
	_CreateShaders(pd3dDevice);

	for (BYTE i = (BYTE)eResourceType::START; i < (BYTE)eResourceType::END; ++i)
		m_vResources.push_back(new CResource());
	//m_vResources[(int)eResourceType::Cube]->SetIDs(0, 0, 0, 1);
	m_vResources[(int)eResourceType::Cube]->SetIDs(0, 1, 0, 1);
	m_vResources[(int)eResourceType::Airplain]->SetIDs(1, 0, 0, 0);
	m_vResources[(int)eResourceType::MonA]->SetIDs(2, 1, 0, 1);
	m_vResources[(int)eResourceType::Floor]->SetIDs(3, 1, 0, 1);
}

CResourceManager::~CResourceManager()
{
	for (BYTE i = (BYTE)eResourceType::START; i < (BYTE)eResourceType::END; ++i)
		delete m_vResources[i];
	m_vResources.clear();

	m_mMesh.clear();
	m_mTexture.clear();
	m_mMaterial.clear();
	m_mShader.clear();
}

CMesh* CResourceManager::GetMesh(eResourceType type)
{
	BYTE id = m_vResources[(int)type]->GetMeshID();
	return m_mMesh[id];
}
CTexture* CResourceManager::GetTexture(eResourceType type)
{
	BYTE id = m_vResources[(int)type]->GetTextureID();
	return m_mTexture[id];
}
CMaterial* CResourceManager::GetMaterial(eResourceType type)
{
	BYTE id = m_vResources[(int)type]->GetMaterialID();
	return m_mMaterial[id];
}
CShader* CResourceManager::GetShaderByResourceType(eResourceType type)
{
	BYTE id = m_vResources[(int)type]->GetShaderID();
	return m_mShader[id];
}
CShader* CResourceManager::GetShaderByShaderType(eShaderType type)
{
	return m_mShader[(int)type];
}

bool CResourceManager::IsTextureEnable(eResourceType type)
{
	if (m_vResources[(int)type]->GetTextureID() > -1)
		return true;
	return false;
}
bool CResourceManager::IsMaterialEnable(eResourceType type)
{
	if (m_vResources[(int)type]->GetMaterialID() > -1)
		return true;
	return false;
}

CResourceManager* CResourceManager::GetSingleton(ID3D11Device *pd3dDevice)
{
	static CResourceManager instance(pd3dDevice);
	return &instance;
}

void CResourceManager::_LoadMesh(ID3D11Device *pd3dDevice)
{
	// 0 : 聽粽
	m_mMesh[0] = new CCubeMeshIlluminatedTextured(pd3dDevice, 100.0f, 100.0f, 100.0f);

	// 1 : 綠ч晦
	m_mMesh[1] = new CAirplaneMesh(pd3dDevice, 20.0f, 20.0f, 4.0f, D3DCOLOR_XRGB(0, 255, 0));

	// 2: monA
	m_mMesh[2] = new CMyModel(pd3dDevice, "Data\\MonA_Data_Info.txt", D3DXVECTOR3(1, 1, 1));

	// 3: 夥款
	m_mMesh[3] = new CCubeMeshIlluminatedTextured(pd3dDevice, 2000.0f, 1.0f, 2000.0f);
}
void CResourceManager::_LoadTextures(ID3D11Device *pd3dDevice)
{
	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);
	ID3D11ShaderResourceView *pd3dTexture = NULL;

	CTexture *tempTexture;
	WCHAR *tempTextureAddress;

	// 0: 說除 漁給
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/Brick01.jpg";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[0] = tempTexture;

	// 1: monA
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/Monkey_monA.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[1] = tempTexture;

	// 2: monB
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/Forest_monster.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[2] = tempTexture;
}
void CResourceManager::_LoadMaterials()
{
	CMaterial *tempMaterial;

	tempMaterial = new CMaterial();
	tempMaterial->GetMaterial()->m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	tempMaterial->GetMaterial()->m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	tempMaterial->GetMaterial()->m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 5.0f);
	tempMaterial->GetMaterial()->m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_mMaterial[0] = tempMaterial;
}
void CResourceManager::_CreateShaders(ID3D11Device *pd3dDevice)
{
	CShader *pShader;

	// 0 : DiffusedShader
	pShader = new CDiffusedShader();
	pShader->CreateShader(pd3dDevice);
	pShader->CreateShaderVariables(pd3dDevice);
	m_mShader[0] = pShader;

	// 1 : IlluminatedTextured
	pShader = new CIlluminatedTexturedShader();
	pShader->CreateShader(pd3dDevice);
	pShader->CreateShaderVariables(pd3dDevice);
	m_mShader[1] = pShader;
}


