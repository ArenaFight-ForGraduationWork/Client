#include "stdafx.h"
#include "ObjectManager.h"

#include "Object.h"
#include "Shader.h"





CObjectManager::CObjectManager()
{
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

	m_mObjects.clear();
}

CObject* CObjectManager::Insert(UINT id, eResourceType eType, CXMVECTOR position, CXMVECTOR direction, bool isAnimating)
{
	if (isAnimating)
		return _InsertAnimateT(id, eType, position, direction);
	else
		return _InsertAnimateF(id, eType, position, direction);
}


CObject* CObjectManager::_InsertAnimateF(UINT id, eResourceType eType, CXMVECTOR position, CXMVECTOR direction)
{
	/* id관련 설명은 ObjectManager헤더파일 맨 위를 참고 */
	float x, y, z;
	CObject *pObject = new CObject(id);

	pObject->SetMesh(pResourceManager->GetMesh(eType));
	pObject->SetMaterial(pResourceManager->GetMaterial(eType));
	pObject->SetTexture(pResourceManager->GetTexture(eType));
	pObject->SetResourceType(static_cast<int>(eType));

	x = XMVectorGetX(position);	y = XMVectorGetY(position);	z = XMVectorGetZ(position);
	pObject->SetPositionAbsolute(x, y, z);
	x = XMVectorGetX(direction); y = XMVectorGetY(direction); z = XMVectorGetZ(direction);
	pObject->SetDirectionAbsolute(x, y, z);

	pObject->PlayAnimation(CObject::eAnimationType::None);
	pObject->SetBoundingBox();

	m_mObjects[id] = pObject;

	CShader *pShader = pResourceManager->GetShaderByResourceType(eType);
	pShader->InsertObject(pObject);

	return pObject;
}
CObject* CObjectManager::_InsertAnimateT(UINT id, eResourceType eType, CXMVECTOR position, CXMVECTOR direction)
{	// 애니메이션 데이터 전용
	float x, y, z;
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

	x = XMVectorGetX(position);	y = XMVectorGetY(position);	z = XMVectorGetZ(position);
	pObject->SetPositionAbsolute(x, y, z);
	x = XMVectorGetX(direction); y = XMVectorGetY(direction); z = XMVectorGetZ(direction);
	pObject->SetDirectionAbsolute(x, y, z);

	pObject->SetBoundingBox();
	pObject->PlayAnimation(CObject::eAnimationType::Idle);

	m_mObjects[id] = pObject;

	CShader *pShader = pResourceManager->GetShaderByResourceType(eType);
	pShader->InsertObject(pObject);

	return pObject;
}

CObject* CObjectManager::FindObject(UINT id)
{
	for (auto obj : m_mObjects)
	{
		if (id == obj.first)
			return m_mObjects[id];
	}
	return nullptr;
}

void CObjectManager::DeleteObject(UINT id)
{
	for (auto obj : m_mObjects)
	{
		if (id == obj.first)
		{
			// 1) 쉐이더와의 연결 해제
			CShader *pShader;
			for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
			{
				pShader = pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i);
				if (pShader->ReleaseObject(id))	break;
			}

			// 2) 오브젝트맵에서 삭제
			CObject *pObject = m_mObjects[id];
			m_mObjects.erase(id);

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
		if (pShader)
			pShader->ReleaseAllObjects();
	}

	// 2) 해당 오브젝트 해제 후 삭제
	for (auto obj : m_mObjects)
	{
		if (obj.second)
			obj.second->~CObject();
	}
	m_mObjects.clear();
}


bool CObjectManager::CheckCollision()
{
	if (!FindObject(myID))
		return false;

	for (auto monster : m_mObjects)
	{
		// 1000 <= monsterID < 2000
		if (monster.first >= 1000)
			if (monster.first < 2000)
			{
				XMFLOAT3 myPosition;
				XMStoreFloat3(&myPosition, FindObject(myID)->GetPosition());
				XMFLOAT3 monsterPosition;
				XMStoreFloat3(&monsterPosition, monster.second->GetPosition());

				float distance = ((myPosition.x - monsterPosition.x)*(myPosition.x - monsterPosition.x))
					+ ((myPosition.z - monsterPosition.z)*(myPosition.z - monsterPosition.z));
				if (sqrt(distance) <= FindObject(myID)->GetRadius() + monster.second->GetRadius())
				{
					return true;
				}
			}
	}

	return false;
}




