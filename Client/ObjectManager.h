#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

#define ID_DIVIDE 1000

extern UINT myID;

class CObjectManager
{
public:
	/* ObjectList�� ī�װ�
	* PLAYER :									�浹üũo, ��Ʈ�ڽ�o,					0~999
	* MONSTER : ���� = ����					�浹üũo, ��Ʈ�ڽ�o,					1000~1999
	* BUFF_CRYSTAL :							�浹üũo, ����o,						2000~2999
	* LAND : �ٴ�								�浹üũx, ����x                        3000~3999
	* NATURAL_FEATURE : ��, ����, �� ���	    �浹üũo, ����x, �̵� ���ϰ� ��.		4000~5999
	*/
	enum class eObjectType :BYTE {
		START = 0,
		PLAYER = 0,
		MONSTER,
		BUFF_CRYSTAL,
		LAND,
		NATURAL_FEATURE,
		END
	};

	static CObjectManager* GetSingleton();
	~CObjectManager();
	void Initialize();

	/* ~�����͸� ���� ������Ʈ�� �߰� */
	CObject* Insert(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0), bool isAnimating = false);

	CObject* FindObject(UINT id);
	/*
	�ش� eObjectTypeī�װ��� �����ִ� ��� ������Ʈ���� ������ id�� ��ȯ�Ѵ�.
	eType�� CObjectManager::eObjectType::(�ʿ��� ī�װ�)�� �Է��ϰ�,
	piNum�� int������ �ϳ� �����ؼ� ���ڷ� �Ѱ��ָ�,
	piNum���� ������Ʈ���� ������ ��ȯ�ϰ�,
	��ȯ���� UINT*�� �ش� ī�װ��� ���� ������Ʈ���� id �迭�� �Ѱ��ش�.

	ex)
	int iTempSize;
	UINT *puiTempArray = pObjectManager->FindObjectsInCategory(CObjectManager::eObjectType::MONSTER, iTempSize);
	�̷��� ȣ���� ��, puiTempArray[idx( 0 ~ iTempSize-1 )]�� ����ϸ� �ȴ�.
	*/
	UINT* FindObjectsInCategory(eObjectType eType, int& iSize);

	void DeleteObject(UINT id);
	void DeleteObjectAll();

	bool CheckCollision();

private:
	std::map<eObjectType, std::vector<CObject*>> m_mObjects;

	CResourceManager *pResourceManager;

	CObjectManager();

	CObject* _InsertAnimateF(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));
	CObject* _InsertAnimateT(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));
};





#endif