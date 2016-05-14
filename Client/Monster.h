#ifndef MONSTER_H_
#define MONSTER_H_

#include "stdafx.h"
#include "Object.h"


class CMonster
{
public:
	CMonster();
	~CMonster();

	void SetObject(CObject *pObject);
	CObject* GetObjects() { return m_pObject; }
	void ReleaseObject();

	void Move(const float cameraYaw, const DWORD dwDirection, const float fTimeElapsed);

	void MoveRelative(const float x, const float y, const float z);
	void MoveRelative(const D3DXVECTOR3 *vec);
	void MoveAbsolute(const float x, const float y, const float z);
	void MoveAbsolute(const D3DXVECTOR3 *vec);
	void RotateRelative(const float x, const float y, const float z);
	void RotateRelative(const D3DXVECTOR3 *vec);
	void RotateAbsolute(const float x, const float y, const float z);
	void RotateAbsolute(const D3DXVECTOR3 *vec);

	const D3DXVECTOR3* GetPosition();

	void SetHP()	{ m_fHP -= 1; }
	void SetSpeed()	{ m_fSpeed += 100; }
	int GetHP()	{ return m_fHP; }
	float GetSpeed()	{ return m_fSpeed; }

private:
	CObject *m_pObject;
	float m_fSpeed;
	float m_fHP;
};


#endif