#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

extern UINT myID;

class CObjectManager
{
public:
	/* ObjectList의 카테고리
	* PLAYER :									충돌체크o, 히트박스o,					0~999
	* MONSTER : 몬스터 = 보스					충돌체크o, 히트박스o,					1000~1999
	* BUFF_CRYSTAL :							충돌체크o, 삭제o,						2000~2999
	* LAND : 바닥								충돌체크x, 삭제x                        3000~3999
	* NATURAL_FEATURE : 벽, 나무, 돌 등등	    충돌체크o, 삭제x, 이동 못하게 함.		4000~5999
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