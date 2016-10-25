#include "stdafx.h"
#include "ObjectManager.h"

#include "Object.h"
#include "Shader.h"





CObjectManager::CObjectManager()
{
	for (BYTE i = (BYTE)eObjectType::START; i < (BYTE)eObjectType::END; ++i)
	{
		m_mObjects[(eObjectType)i] = std::vector<CObject*>();
	}
}
CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::GetSingleton()
{
	static CObjectManager instance;
	return &instance;
}

void CObjectManager::Initialize()
{
	pResourceManager = CResourceManager::GetSingleton();
}

CObject* CObjectManager::Insert(UINT id, eResourceType eType, D3DXVECTOR3 position, D3DXVECTOR3 direction, bool isAnimating)
{
	if (isAnimating)
		 return _InsertAnimateT(id, eType, position, direction);
	else
		return _InsertAnimateF(id, eType, position, direction);
}


CObject* CObjectManager::_InsertAnimateF(UINT id, eResourceType eType, D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	/* id관련 설명은 ObjectManager헤더파일 맨 위를 참고 */
	CObject *pObject = new CObject(id);
	pObject->SetMesh(pResourceManager->GetMesh(eType));
	pObject->SetMaterial(pResourceManager->GetMaterial(eType));
	pObject->SetTexture(pResourceManager->GetTexture(eType));
	pObject->SetResourceType(static_cast<int>(eType));

	pObject->SetPositionAbsolute(position.x, position.y, position.z);
	pObject->SetDirectionAbsolute(direction.x, direction.y, direction.z);

	pObject->PlayAnimation(CObject::eAnimationType::None);
	pObject->SetBoundingBox();

	m_mObjects[(eObjectType)static_cast<UINT>(id / ID_DIVIDE)].push_back(pObject);

	CShader *pShader = pResourceManager->GetShaderByResourceType(eType);
	pShader->InsertObject(pObject);

	return pObject;
}
CObject* CObjectManager::_InsertAnimateT(UINT id, eResourceType eType, D3DXVECTOR3 position, D3DXVECTOR3 direction)
{	// 애니메이션 데이터 전용
	CObject *pObject = new CObject(id);

	switch (eType)
	{
	case eResourceType::User:
	case eResourceType::Monster1:
		pObject->SetComponent();
		break;
	case eResourceType::Item_HP:
	case eResourceType::Item_Buff:
	case eResourceType::Floor:
	case eResourceType::Tree:
	case eResourceType::Wall1:
	default:
		break;
	}
	pObject->SetMesh(pResourceManager->GetMesh(eType));
	pObject->SetMaterial(pResourceManager->GetMaterial(eType));
	pObject->SetTexture(pResourceManager->GetTexture(eType));
	pObject->SetResourceType(static_cast<int>(eType));
	
	pObject->SetTime(pResourceManager->GetMesh(eType)->GetAniMaxTime());
	pObject->SetAniIndexCount(pResourceManager->GetMesh(eType)->GetAnimationIndexCnt());
	pObject->SetResult(pResourceManager->GetMesh(eType)->GetResultMatrix());
	pObject->SetConstantBuffer();
	
	pObject->SetPositionAbsolute(position.x, position.y, position.z);
	pObject->SetDirectionAbsolute(direction.x, direction.y, direction.z);

	pObject->SetBoundingBox();
	pObject->PlayAnimation(CObject::eAnimationType::Idle);

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
	return nullptr;
}

UINT* CObjectManager::FindObjectsInCategory(eObjectType eType, int& iSize)
{
	iSize = m_mObjects[eType].size();
	UINT *puiObjects = new UINT[iSize];
	for (int i = 0; i < iSize; ++i)
	{
		puiObjects[i] = m_mObjects[eType][i]->GetId();
	}

	return puiObjects;
}

void CObjectManager::DeleteObject(UINT id)
{
	for (unsigned short i = 0; i < m_mObjects[(eObjectType)(id / ID_DIVIDE)].size(); ++i)
	{
		if (id == m_mObjects[(eObjectType)(id / ID_DIVIDE)][i]->GetId())
		{
			// 1) 쉐이더와의 연결 해제
			CShader *pShader;
			for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
			{
				pShader = pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i);
				if (pShader->ReleaseObject(id))	break;
			}

			// 2) 오브젝트맵에서 삭제
			CObject *pObject = m_mObjects[(eObjectType)(id / ID_DIVIDE)][i];
			m_mObjects[(eObjectType)(id / ID_DIVIDE)].erase(m_mObjects[(eObjectType)(id / ID_DIVIDE)].begin() + i);

			// 3) 해당 오브젝트 제거
			pObject->~CObject();
		}
	}
}

void CObjectManager::DeleteObjectAll()
{
	// 1) 모든 쉐이더의 오브젝트와의 연결 해제
	CShader *pShader;
	for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
	{
		pShader = pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i);
		if (pShader)	pShader->ReleaseAllObjects();
	}

	// 2) 해당 오브젝트 해제 후 삭제
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


bool CObjectManager::CheckCollision()
{
	if (!FindObject(myID))
		return false;

	for (auto monster : m_mObjects[eObjectType::MONSTER])
	{
		XMFLOAT3 myPosition;
		XMStoreFloat3(&myPosition, FindObject(myID)->GetPosition());
		XMFLOAT3 monsterPosition;
		XMStoreFloat3(&monsterPosition, monster->GetPosition());

		float distance = ((myPosition.x - monsterPosition.x)*(myPosition.x - monsterPosition.x))
			+ ((myPosition.z - monsterPosition.z)*(myPosition.z - monsterPosition.z));
		if (sqrt(distance) <= FindObject(myID)->GetRadius() + monster->GetRadius())
		{
			return true;
		}
	}
	 
	return false;
}




