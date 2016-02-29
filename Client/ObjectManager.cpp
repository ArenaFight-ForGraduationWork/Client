#include "stdafx.h"
#include "ObjectManager.h"

#include "Object.h"
#include "Shader.h"





CObjectManager::CObjectManager(ID3D11Device *pd3dDevice)
{
	for (BYTE i = (BYTE)ObjectType::START; i < (BYTE)ObjectType::END; ++i)
	{
		m_mObjects[(ObjectType)i] = std::vector<CObject*>();
	}

	pResourceManager = CResourceManager::GetSingleton(pd3dDevice);
}

CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::GetSingleton(ID3D11Device *pd3dDevice)
{
	static CObjectManager instance(pd3dDevice);
	return &instance;
}

void CObjectManager::Insert(UINT id, int x, int y, int z, int dx, int dy, int dz)
{
	/* id관련 설명은 ObjectManager헤더파일 맨 위를 참고 */
	CObject *pObject = new CObject(id);
	pObject->SetMesh(pResourceManager->GetMesh(CResourceManager::ResourceType::Cube));
	pObject->SetMaterial(pResourceManager->GetMaterial(CResourceManager::ResourceType::Cube));
	pObject->SetTexture(pResourceManager->GetTexture(CResourceManager::ResourceType::Cube));
	pObject->MoveAbsolute(x, y, z);
	pObject->RotateAbsolute(x, y, z);
	m_mObjects[(ObjectType)(id / 10000)].push_back(pObject);

	CShader *pShader = pResourceManager->GetShaderByResourceType(CResourceManager::ResourceType::Cube);
	pShader->InsertObject(pObject);
}
void CObjectManager::Insert(UINT id, D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	/* id관련 설명은 ObjectManager헤더파일 맨 위를 참고 */
	CObject *pObject = new CObject(id);
	pObject->SetMesh(pResourceManager->GetMesh(CResourceManager::ResourceType::Cube));
	pObject->SetMaterial(pResourceManager->GetMaterial(CResourceManager::ResourceType::Cube));
	pObject->SetTexture(pResourceManager->GetTexture(CResourceManager::ResourceType::Cube));
	pObject->MoveAbsolute(&position);
	pObject->RotateAbsolute(&direction);
	m_mObjects[(ObjectType)(id / 10000)].push_back(pObject);

	CShader *pShader = pResourceManager->GetShaderByResourceType(CResourceManager::ResourceType::Cube);
	pShader->InsertObject(pObject);
}

CObject* CObjectManager::FindObject(UINT id)
{
	for (auto obj : m_mObjects[(ObjectType)(id / 10000)])
	{
		if (id == obj->GetId())
			return obj;
	}
}

const std::vector<CObject*> CObjectManager::FindObjectInCategory(const UINT id)
{
	return m_mObjects[(ObjectType)(id / 10000)];
}

void CObjectManager::DeleteObject(UINT id)
{
	for (short i = 0; i < m_mObjects[(ObjectType)(id / 10000)].size(); ++i)
	{
		if (id == m_mObjects[(ObjectType)(id / 10000)][i]->GetId())
		{
			// 1) 쉐이더와의 연결 해제
			CShader *pShader;
			for (BYTE i = (BYTE)CResourceManager::ShaderType::START; i < (BYTE)CResourceManager::ShaderType::END; ++i)
			{
				pShader = pResourceManager->GetShaderByShaderType((CResourceManager::ShaderType)i);
				pShader->ReleaseObject(id);
				/*
				이 부분 쉐이더 전체를 도니까 이거 어떻게 좀 바꿔야겠다
				bool을 반환하게 해서 확인할까
				*/
			}

			// 2) 오브젝트맵에서 삭제
			CObject *pObject = m_mObjects[(ObjectType)(id / 10000)][i];
			m_mObjects[(ObjectType)(id / 10000)].erase(m_mObjects[(ObjectType)(id / 10000)].begin() + i);

			//// 3) 해당 오브젝트 제거
			//pObject->~CObject();
		}
	}
}

void CObjectManager::DeleteObjectAll()
{
	// 1) 모든 쉐이더의 오브젝트와의 연결 해제
	CShader *pShader;
	for (BYTE i = (BYTE)CResourceManager::ShaderType::START; i < (BYTE)CResourceManager::ShaderType::END; ++i)
	{
		pShader = pResourceManager->GetShaderByShaderType((CResourceManager::ShaderType)i);
		pShader->ReleaseAllObjects();
	}

	// 2) 해당 오브젝트 해제 후 삭제
	for (short i = 0; i < m_mObjects.size(); ++i)
	{
		for (auto obj : m_mObjects[(ObjectType)i])
		{
			obj->~CObject();
		}
		m_mObjects[(ObjectType)i].clear();
	}
	m_mObjects.clear();
}