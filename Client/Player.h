#ifndef PLAYER_H_
#define PLAYER_H_

#include "stdafx.h"
#include "Object.h"



class CPlayer
{
public:
	CPlayer();
	~CPlayer();

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
\
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

};



#endif