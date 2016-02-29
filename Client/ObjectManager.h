#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "stdafx.h"
#include "ResourceManager.h"

/*
*		고민 : ObjectCategory
*		1) LAND와 NATURAL_FEATURE를 나눠야 하는 이유가 무엇인가
*			일단 둘 다 충돌체크O, 존재O, 움직임X
*		2) BUFF_CRYSTAL
*			이름이 맘에 안 든다. 포괄적인 이름이 필요하다.
*			충돌체크X, 존재O인 물체
*
*		id 형식 아직 미정이라 내 맘대로 UINT 하나로 쓰는거 함
*			UINT = 0~4G 정도
*		몬스터 2xx00, 사람 3xx00으로 그냥 해놓음 id 하나로 타입까지 한방에.
*		나중에 바뀌는대로 바꿔라....
*/


class CObjectManager
{
public:
	static CObjectManager* GetSingleton(ID3D11Device *pd3dDevice);
	~CObjectManager();

	/* ~데이터를 가진 오브젝트를 추가 */
	void Insert(UINT id, int x, int y, int z, int dx, int dy, int dz);
	void Insert(UINT id, D3DXVECTOR3 position, D3DXVECTOR3 direction);

	CObject* FindObject(UINT id);
	const std::vector<CObject*> FindObjectInCategory(const UINT id);

	void DeleteObject(UINT id);

	void DeleteObjectAll();

	/* ObjectList의 카테고리
	* LAND : 지형. 바닥, 벽
	* NATURAL_FEATURE : 나무, 돌 같은 잡동사니. 충돌체크O 존재O
	* MONSTER : 몬스터 = 보스
	* PLAYER : 나와 다른 유저들을 구분해야 할까? 고민 좀 해봐야겠음
	* BUFF_CRYSTAL : 일단 이렇게 써놨는데. 충돌체크X 존재O인 오브젝트
	*/
	enum class ObjectType :BYTE{
		START = 0,
		LAND = 0,
		NATURAL_FEATURE,
		MONSTER,
		PLAYER,
		BUFF_CRYSTAL,
		END
	};

private:
	std::map<ObjectType, std::vector<CObject*>> m_mObjects;

	CResourceManager *pResourceManager;

	CObjectManager(ID3D11Device *pd3dDevice);
};





#endif