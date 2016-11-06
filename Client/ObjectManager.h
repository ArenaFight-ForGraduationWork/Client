#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

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

	static CObjectManager* GetSingleton();
	~CObjectManager();
	void Initialize();

	CObject* Insert(UINT id, eResourceType eType, CXMVECTOR position, CXMVECTOR direction, bool isAnimating = false);
	CObject* FindObject(UINT id);

	void DeleteObject(UINT id);
	void DeleteObjectAll();

	bool CheckCollision();

private:
	CObjectManager();

	map<UINT, CObject*> m_mObjects;

	CResourceManager *pResourceManager;

	CObject* _InsertAnimateF(UINT id, eResourceType eType, CXMVECTOR position, CXMVECTOR direction);
	CObject* _InsertAnimateT(UINT id, eResourceType eType, CXMVECTOR position, CXMVECTOR direction);
};





#endif