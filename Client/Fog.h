#ifndef FOG_H_
#define FOG_H_



/* �Ȱ��� �����ϴ� Ŭ����. ������ Fog.h ���� �ϴ� ���� */
class CFog
{
public:
	CFog();
	~CFog();

	void Initialize();
	void Destroy();

	/* �ݵ�� IsInUse()�Լ��� ���� ��������� üũ�ϰ� ����� ��. �ڼ��� ���� Fog.h ������� �Ʒ���. */
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
		EXPAND,		// Ȯ��
		CONTRACT,	// ����
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