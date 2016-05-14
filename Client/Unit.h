#ifndef UNIT_H_
#define UNIT_H_

#include "stdafx.h"
#include "Object.h"



class CUnit
{
public:
	CUnit();
	CUnit(CUnit&);
	~CUnit();

	void SetObject(CObject *pObject);
	CObject* GetObjects() { return m_pObject; }
	void ReleaseObject();

	void Move(const float cameraYaw, const DWORD dwDirection, const float fTimeElapsed);
	void boundingBoxMove(const float, const DWORD, const float);

	void MoveRelative(const float x, const float y, const float z);
	void MoveRelative(const D3DXVECTOR3 *vec);
	void MoveAbsolute(const float x, const float y, const float z);
	void MoveAbsolute(const D3DXVECTOR3 *vec);

	void RotateRelative(const float x, const float y, const float z);
	void RotateRelative(const D3DXVECTOR3 *vec);
	void RotateAbsolute(const float x, const float y, const float z);
	void RotateAbsolute(const D3DXVECTOR3 *vec);
	void AnimateRender(int StateNum, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);

	const D3DXVECTOR3* GetPosition();

	void SetHP()	{ m_fHP -= 1; }
	void SetSpeed()	{ m_fSpeed += 100; }
	int GetHP()	{ return m_fHP; }
	float GetSpeed()	{ return m_fSpeed; }
	bool GetIsAttack()	{ return isAttack; }						//공격중인가 아닌가를 판단함
	void SetIsAttack(bool mode)	{ isAttack = mode; }
protected:

private:
	CObject *m_pObject;

	float m_fSpeed;
	int m_fHP;

	bool isAttack = false;

	float m_fStrikingPower;		/* 공격력 */
	float m_fDefensivePower;	/* 방어력 */

	float m_fMovingSpeed;		/* 이동속도. m/s */

	float m_fHp;				/* 체력 */
};


class CPlayer : public CUnit
{
public:
	CPlayer();
	CPlayer(CPlayer&);
	~CPlayer();

protected:
private:
};



class CMonster : public CUnit
{
public:
	CMonster();
	CMonster(CMonster&);
	~CMonster();

protected:
private:
};



#endif