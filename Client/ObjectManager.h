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

#define ID_DIVIDE 10000

class CObjectManager
{
public:
	/* ObjectList의 카테고리
	* LAND : 지형. 바닥, 벽
	* NATURAL_FEATURE : 나무, 돌 같은 잡동사니. 충돌체크O 존재O
	* MONSTER : 몬스터 = 보스
	* PLAYER : 나와 다른 유저들을 구분해야 할까? 고민 좀 해봐야겠음
	* BUFF_CRYSTAL : 일단 이렇게 써놨는데. 충돌체크X 존재O인 오브젝트
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

	static CObjectManager* GetSingleton();
	~CObjectManager();
	void Initialize(ID3D11Device *pd3dDevice);

	/* ~데이터를 가진 오브젝트를 추가 */
	CObject* Insert(UINT id, eResourceType eType, int x = 0, int y = 0, int z = 0, int dx = 0, int dy = 0, int dz = 0);
	CObject* Insert(UINT id, eResourceType eType, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

	//애니메이션 전용
	CObject* Insert(UINT id, eResourceType eType, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext, int a, int b, D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0));

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
	UINT* FindObjectsInCategory(eObjectType eType, int& piNum);

	void DeleteObject(UINT id);
	void DeleteObjectAll();

private:
	std::map<eObjectType, std::vector<CObject*>> m_mObjects;

	CResourceManager *pResourceManager;

	CObjectManager();
};





#endif