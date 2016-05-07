#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

/*
*		��� : ObjectCategory
*		1) LAND�� NATURAL_FEATURE�� ������ �ϴ� ������ �����ΰ�
*			�ϴ� �� �� �浹üũO, ����O, ������X
*		2) BUFF_CRYSTAL
*			�̸��� ���� �� ���. �������� �̸��� �ʿ��ϴ�.
*			�浹üũX, ����O�� ��ü
*
*		id ���� ���� �����̶� �� ����� UINT �ϳ��� ���°� ��
*			UINT = 0~4G ����
*		���� 2xx00, ��� 3xx00���� �׳� �س��� id �ϳ��� Ÿ�Ա��� �ѹ濡.
*		���߿� �ٲ�´�� �ٲ��....
*/

#define ID_DIVIDE 1000

class CObjectManager
{
public:
	/* ObjectList�� ī�װ�
	* LAND : ����. �ٴ�, ��, ����, �� ���	=>  �浹üũo, ����x, �̵� ���ϰ� ��.		0~999
	* BUFF_CRYSTAL :							�浹üũo, ����o,							1000~1999
	* MONSTER : ���� = ����					 �浹üũo, ��Ʈ�ڽ�o,					2000~2999
	* PLAYER :										�浹üũo, ��Ʈ�ڽ�o,						3000~399
	*/
	enum class eObjectType :BYTE{
		START = 0,
		LAND = 0,
		BUFF_CRYSTAL,
		MONSTER,
		PLAYER,
		END
	};

	static CObjectManager* GetSingleton();
	~CObjectManager();
	void Initialize(ID3D11Device *pd3dDevice);

	/* ~�����͸� ���� ������Ʈ�� �߰� */
	CObject* Insert(UINT id, eResourceType eType, int x = 0, int y = 0, int z = 0, int dx = 0, int dy = 0, int dz = 0);
	CObject* Insert(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

	//�ִϸ��̼� ����
	CObject* Insert(UINT id, eResourceType eType, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext, int a, int b, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

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
	std::vector<CObject*> FindObjectInCategory(eObjectType eType);

	void DeleteObject(UINT id);
	void DeleteObjectAll();

private:
	std::map<eObjectType, std::vector<CObject*>> m_mObjects;

	CResourceManager *pResourceManager;

	CObjectManager();
};





#endif