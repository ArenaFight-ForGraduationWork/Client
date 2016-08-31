#ifndef FOG_H_
#define FOG_H_



/* 안개를 관리하는 클래스. 사용법은 Fog.h 파일 하단 참조 */
class CFog
{
public:
	CFog();
	~CFog();

	void Initialize();
	void Destroy();

	/* 반드시 IsInUse()함수로 현재 사용중인지 체크하고 사용할 것. 자세한 것은 Fog.h 헤더파일 아래에. */
	void Update();
	void Expand(D3DXVECTOR3 *pd3dvCenter);
	void Contract();

	void UpdateShaderVariables();

	bool IsInUse();

private:
	enum class eState : BYTE{
		START = 0,
		DISABLE = 0,
		KEEP,
		EXPAND,		// 확장
		CONTRACT,	// 수축
		END
	};
	eState m_eState;

	D3DXVECTOR3 *m_pd3dxvCenter;

	float m_fNowRange;
	float m_fMinRange;
	float m_fMaxRange;

	ID3D11Buffer *m_pd3dcbFog;
};






/*
	CFog *pFog = new CFog();
	pFog->Initialize();	// 초기 생성시 한 번. 비활성 상태로 생성된다.

	pFog->Update();	<< 현재 상태에 따라 안개를 변형한다.
	안개가 현재 활성화 상태인지, 비활성화 상태인지는
	pFog->IsInUse()가 반환한다.
	true를 반환하면 활성화, false를 반환하면 비활성화.
	그러므로 if(pFog->IsInUse()) pFog->Update() 로 함수를 작성하면
	안개가 비활성화 상태일 때엔 안개 상태를 업데이트 하지 않는다.
	프레임마다 한 번씩 해주면 된다.

	안개를 생성하고 싶으면 생성하고 싶은 위치를 인자로 넣어 pFog->Expand(위치);
	생성된 안개를 없애고 싶으면 pFog->Contract();
	안개가 한 번 생성된 상태에서 Expand()를 다시 호출해도 안개는 다른 쪽으로 이동하지 않는다. 1개만 존재.
	안개가 생성되고 있는 도중에 Contract()를 호출해도 사라지지 않는다.
	마찬가지로 안개가 사라지는 도중에 Expand()를 호출해도 생성되지 않는다.

	안개 색상 변경은 쉐이더 파일의 픽셀 쉐이더에서 fogColor를 변경하면 된다. RGB/255.0의 값으로 바꿔넣으면 된다.

	CFog *pFog = new CFog();
	pFog->Initialize();

	pFog->Expand(position);
	pFog->Contract();

	if(pFog->IsInUse())
		pFog->Update();
*/



#endif