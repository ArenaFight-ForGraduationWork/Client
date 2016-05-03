#ifndef FOG_H_
#define FOG_H_



struct VS_CB_FOG_CENTER
{
	float m_fEnable;
	D3DXVECTOR3 m_d3dxvCenter;
};
struct VS_CB_FOG_RANGE
{
	float m_fRange;
	float padding1, padding2, padding3;
};



/* �Ȱ��� �����ϴ� Ŭ����. ������ Fog.h ���� �ϴ� ���� */
class CFog
{
public:
	CFog();
	~CFog();

	void Initialize(ID3D11Device *pd3dDevice);
	void Destroy();

	void Update(ID3D11Device *pd3dDevice);
	void Expand(ID3D11Device *pd3dDevice, const D3DXVECTOR3 *pd3dvCenter);
	void Contract(ID3D11Device *pd3dDeivce);

	//void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 pd3dxvCenter);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const float fRange);

	bool IsInUse();

private:
	enum class eState : BYTE{
		START = 0,
		DISABLE = 0,
		KEEP,
		EXPAND,		// Ȯ��
		CONTRACT,	// ����
		END
	};
	eState m_eState;

	D3DXVECTOR3 *m_pd3dxvCenter;

	float m_fNowRange;
	float m_fMinRange;
	float m_fMaxRange;

	ID3D11Buffer *m_pd3dcbFogCenter;
	ID3D11Buffer *m_pd3dcbFogRange;
};






/*
	CFog *pFog = new CFog();
	pFog->Initialize();	// �ʱ� ������ �� ��. ��Ȱ�� ���·� �����ȴ�.

	pFog->Update();	<< ���� ���¿� ���� �Ȱ��� �����Ѵ�.
	�Ȱ��� ���� Ȱ��ȭ ��������, ��Ȱ��ȭ ����������
	pFog->IsInUse()�� ��ȯ�Ѵ�.
	true�� ��ȯ�ϸ� Ȱ��ȭ, false�� ��ȯ�ϸ� ��Ȱ��ȭ.
	�׷��Ƿ� if(pFog->IsInUse()) pFog->Update() �� �Լ��� �ۼ��ϸ�
	�Ȱ��� ��Ȱ��ȭ ������ ���� �Ȱ� ���¸� ������Ʈ ���� �ʴ´�.
	�����Ӹ��� �� ���� ���ָ� �ȴ�.

	�Ȱ��� �����ϰ� ������ �����ϰ� ���� ��ġ�� ���ڷ� �־� pFog->Expand(��ġ);
	������ �Ȱ��� ���ְ� ������ pFog->Contract();
	�Ȱ��� �� �� ������ ���¿��� Expand()�� �ٽ� ȣ���ص� �Ȱ��� �ٸ� ������ �̵����� �ʴ´�. 1���� ����.
	�Ȱ��� �����ǰ� �ִ� ���߿� Contract()�� ȣ���ص� ������� �ʴ´�.
	���������� �Ȱ��� ������� ���߿� Expand()�� ȣ���ص� �������� �ʴ´�.

	�Ȱ� ���� ������ ���̴� ������ �ȼ� ���̴����� fogColor�� �����ϸ� �ȴ�. RGB/255.0�� ������ �ٲ������ �ȴ�.

	CFog *pFog = new CFog();
	pFog->Initialize();

	pFog->Expand(position);
	pFog->Contract();

	if(pFog->IsInUse())
		pFog->Update();
*/



#endif