#include "stdafx.h"
#include "ObjectManager.h"

#include "Object.h"
#include "Shader.h"





CObjectManager::CObjectManager(ID3D11Device *pd3dDevice)
{
	for (BYTE i = (BYTE)eObjectType::START; i < (BYTE)eObjectType::END; ++i)
	{
		m_mObjects[(eObjectType)i] = std::vector<CObject*>();
	}

	pResourceManager = CResourceManager::GetSingleton(pd3dDevice);
}

CObjectManager::~CObjectManager()
{
	DeleteObjectAll();
}

CObjectManager* CObjectManager::GetSingleton(ID3D11Device *pd3dDevice)
{
	static CObjectManager instance(pd3dDevice);
	return &instance;
}

CObject* CObjectManager::Insert(UINT id, eResourceType eType, int x, int y, int z, int dx, int dy, int dz)
{
	/* id���� ������ ObjectManager������� �� ���� ���� */
	CObject *pObject = new CObject(id);
	pObject->SetMesh(pResourceManager->GetMesh(eType));
	pObject->SetMaterial(pResourceManager->GetMaterial(eType));
	pObject->SetTexture(pResourceManager->GetTexture(eType));
	pObject->MoveAbsolute(x, y, z);
	pObject->RotateAbsolute(x, y, z);
	m_mObjects[(eObjectType)(id / ID_DIVIDE)].push_back(pObject);

	CShader *pShader = pResourceManager->GetShaderByResourceType(eType);
	pShader->InsertObject(pObject);

	return pObject;
}
CObject* CObjectManager::Insert(UINT id, eResourceType eType, D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	/* id���� ������ ObjectManager������� �� ���� ���� */
	CObject *pObject = new CObject(id);
	pObject->SetMesh(pResourceManager->GetMesh(eType));
	pObject->SetMaterial(pResourceManager->GetMaterial(eType));
	pObject->SetTexture(pResourceManager->GetTexture(eType));
	pObject->MoveAbsolute(&position);
	pObject->RotateAbsolute(&direction);
	m_mObjects[(eObjectType)(id / ID_DIVIDE)].push_back(pObject);

	CShader *pShader = pResourceManager->GetShaderByResourceType(eType);
	pShader->InsertObject(pObject);

	return pObject;
}

CObject* CObjectManager::FindObject(UINT id)
{
	for (auto obj : m_mObjects[(eObjectType)(id / ID_DIVIDE)])
	{
		if (id == obj->GetId())
			return obj;
	}
}

const std::vector<CObject*> CObjectManager::FindObjectInCategory(const UINT id)
{
	return m_mObjects[(eObjectType)(id / ID_DIVIDE)];
}

void CObjectManager::DeleteObject(UINT id)
{
	for (short i = 0; i < m_mObjects[(eObjectType)(id / ID_DIVIDE)].size(); ++i)
	{
		if (id == m_mObjects[(eObjectType)(id / ID_DIVIDE)][i]->GetId())
		{
			// 1) ���̴����� ���� ����
			CShader *pShader;
			for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
			{
				pShader = pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i);
				if (pShader->ReleaseObject(id))	break;
			}

			// 2) ������Ʈ�ʿ��� ����
			CObject *pObject = m_mObjects[(eObjectType)(id / ID_DIVIDE)][i];
			m_mObjects[(eObjectType)(id / ID_DIVIDE)].erase(m_mObjects[(eObjectType)(id / ID_DIVIDE)].begin() + i);

			// 3) �ش� ������Ʈ ����
			pObject->~CObject();
		}
	}
}

void CObjectManager::DeleteObjectAll()
{
	// 1) ��� ���̴��� ������Ʈ���� ���� ����
	CShader *pShader;
	for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
	{
		pShader = pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i);
		if (pShader)	pShader->ReleaseAllObjects();
	}

	// 2) �ش� ������Ʈ ���� �� ����
	for (BYTE i = 0; i < m_mObjects.size(); ++i)
	{
		for (auto obj : m_mObjects[(eObjectType)i])
		{
			obj->~CObject();
		}
		m_mObjects[(eObjectType)i].clear();
	}
	m_mObjects.clear();
}