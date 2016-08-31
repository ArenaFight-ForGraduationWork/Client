#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

#define ID_DIVIDE 1000

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

	/* ~데이터를 가진 오브젝트를 추가 */
	CObject* Insert(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0), bool isAnimating = false);

	CObject* FindObject(UINT id);
	/*
	해당 eObjectType카테고리에 속해있는 모든 오브젝트들의 개수와 id를 반환한다.
	eType에 CObjectManager::eObjectType::(필요한 카테고리)를 입력하고,
	piNum에 int변수를 하나 생성해서 인자로 넘겨주면,
	piNum으로 오브젝트들의 개수를 반환하고,
	반환값인 UINT*에 해당 카테고리에 속한 오브젝트들의 id 배열을 넘겨준다.

	ex)
	int iTempSize;
	UINT *puiTempArray = pObjectManager->FindObjectsInCategory(CObjectManager::eObjectType::MONSTER, iTempSize);
	이렇게 호출한 뒤, puiTempArray[idx( 0 ~ iTempSize-1 )]로 사용하면 된다.
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