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

	/* 
		Mesh, Texture, Material, Shader
											1. object  2. Animation
	*/
	for (BYTE i = (BYTE)eResourceType::START; i < (BYTE)eResourceType::END; ++i)
		m_vResources.push_back(new CResource());

	m_vResources[(int)eResourceType::User]->SetIDs(0, 0, 0, 1);
	m_vResources[(int)eResourceType::Monster1]->SetIDs(1, 1, 0, 1);
	m_vResources[(int)eResourceType::Item_HP]->SetIDs(2, 2, 0, 0);
	m_vResources[(int)eResourceType::Item_Buff]->SetIDs(2, 7, 0, 0);		//텍스쳐 번호만 바꿔주면됨
	m_vResources[(int)eResourceType::Floor]->SetIDs(3, 3, 0, 0);
	m_vResources[(int)eResourceType::Tree]->SetIDs(4, 4, 0, 0);
	m_vResources[(int)eResourceType::Wall1]->SetIDs(5, 5, 0, 0);
	m_vResources[(int)eResourceType::MakeWall]->SetIDs(6, 6, 0, 0);

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
	// 0. 메인 캐릭터
	m_mMesh[0] = new CImportedAnimatingMesh(pd3dDevice, 0, 7);

	// 1. 몬스터 - 슬라임
	m_mMesh[1] = new CImportedAnimatingMesh(pd3dDevice, 1, 7);

	// 2. 아이템 오브젝트
	m_mMesh[2] = new CImportedMesh(pd3dDevice, "Data\\ItemObject_Info.txt", D3DXVECTOR3(1, 1, 1));

	// 3. 바닥
	m_mMesh[3] = new CCubeMeshIlluminatedTextured(pd3dDevice, 5000.0f, 1.0f, 5000.0f);

	// 4. 나무
	m_mMesh[4] = new CImportedMesh(pd3dDevice, "Data\\tree_info.txt", D3DXVECTOR3(0.8, 0.8, 0.8));

	// 5. 만든 벽
	m_mMesh[5] = new CCubeMeshIlluminatedTextured(pd3dDevice, 2500.0f, 1500.0f, 200.0f);
	
	// 6. 만들어준 벽
	m_mMesh[6] = new CImportedMesh(pd3dDevice, "Data\\wall.txt", D3DXVECTOR3(1,1, 1));		//만들어준 벽

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


	// 0 : 인간
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/human.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[0] = tempTexture;

	// 1 : 슬라임
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/monster.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[1] = tempTexture;

	// 2: Item_HP
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/Item_RED.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[2] = tempTexture;

	// 3 : 바닥
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/ground.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[3] = tempTexture;

	// 4: 나무
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/tree.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[4] = tempTexture;

	// 5 : 만든 벽
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/벽돌.jpg";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[5] = tempTexture;


	// 6 : 만들어준 벽
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/wall.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[6] = tempTexture;

	// 7. Item_buff
	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/파란보석.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[7] = tempTexture;

	tempTexture = new CTexture(1);
	tempTextureAddress = L"./Data/흰보석.png";
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, tempTextureAddress, NULL, NULL, &pd3dTexture, NULL);
	tempTexture->SetTexture(0, pd3dTexture, pd3dSamplerState);
	m_mTexture[8] = tempTexture;


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

	// 0 : CIlluminatedTexturedShader
	pShader = new CIlluminatedTexturedShader();
	pShader->CreateShader(pd3dDevice);
	pShader->CreateShaderVariables(pd3dDevice);
	m_mShader[0] = pShader;

	// 1 : PlayerShader
	pShader = new CAnimatingShader();
	pShader->CreateShader(pd3dDevice);
	pShader->CreateShaderVariables(pd3dDevice);
	m_mShader[1] = pShader;
}


