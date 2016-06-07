#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

#define ID_DIVIDE 1000



class CObjectManager
{
public:
	/* ObjectList�� ī�װ�
	* NATURAL_FEATURE : ��, ����, �� ���	    �浹üũo, ����x, �̵� ���ϰ� ��.		0~999
	* BUFF_CRYSTAL :							�浹üũo, ����o,						1000~1999
	* MONSTER : ���� = ����					�浹üũo, ��Ʈ�ڽ�o,					2000~2999
	* PLAYER :									�浹üũo, ��Ʈ�ڽ�o,					3000~3999
	* LAND : �ٴ�.								�浹üũx, ����x                        4000~
	*/
	enum class eObjectType :BYTE{
		START = 0,
		NATURAL_FEATURE = 0,
		BUFF_CRYSTAL,
		MONSTER,
		PLAYER,
		LAND,
		END
	};

	static CObjectManager* GetSingleton();
	~CObjectManager();
	void Initialize(ID3D11Device *pd3dDevice);

	/* ~�����͸� ���� ������Ʈ�� �߰� */
	CObject* Insert(UINT id, eResourceType eType, float x = 0, float y = 0, float z = 0, float dx = 0, float dy = 0, float dz = 0);
	CObject* Insert(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

	//�ִϸ��̼� ����
	CObject* Insert(UINT id, eResourceType eType, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

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
	std::vector<CObject*> FindObjectInCategory(eObjectType eType);

	void DeleteObject(UINT id);
	void DeleteObjectAll();

private:
	std::map<eObjectType, std::vector<CObject*>> m_mObjects;

	CResourceManager *pResourceManager;

	CObjectManager();
};





#endif