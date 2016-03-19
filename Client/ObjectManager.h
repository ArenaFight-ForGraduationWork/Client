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

#define ID_DIVIDE 10000

class CObjectManager
{
public:
	static CObjectManager* GetSingleton(ID3D11Device *pd3dDevice);
	~CObjectManager();

	/* ~�����͸� ���� ������Ʈ�� �߰� */
	CObject* Insert(UINT id, eResourceType eType, int x = 0, int y = 0, int z = 0, int dx = 0, int dy = 0, int dz = 0);
	CObject* Insert(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

	//�ִϸ��̼� ����
	CObject* Insert(UINT id, eResourceType eType, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

	CObject* FindObject(UINT id);
	const std::vector<CObject*> FindObjectInCategory(const UINT id);

	void DeleteObject(UINT id);

	void DeleteObjectAll();

	/* ObjectList�� ī�װ�
	* LAND : ����. �ٴ�, ��
	* NATURAL_FEATURE : ����, �� ���� �⵿���. �浹üũO ����O
	* MONSTER : ���� = ����
	* PLAYER : ���� �ٸ� �������� �����ؾ� �ұ�? ��� �� �غ��߰���
	* BUFF_CRYSTAL : �ϴ� �̷��� ����µ�. �浹üũX ����O�� ������Ʈ
	*/
	enum class eObjectType :BYTE{
		START = 0,
		LAND = 0,
		NATURAL_FEATURE,
		MONSTER,
		PLAYER,
		BUFF_CRYSTAL,
		END
	};

private:
	std::map<eObjectType, std::vector<CObject*>> m_mObjects;

	CResourceManager *pResourceManager;

	CObjectManager(ID3D11Device *pd3dDevice);
};





#endif