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

	void MoveRelative(const float x, const float y, const float z);
	void MoveRelative(const D3DXVECTOR3 *vec);
	void MoveAbsolute(const float x, const float y, const float z);
	void MoveAbsolute(const D3DXVECTOR3 *vec);

	void RotateRelative(const float x, const float y, const float z);
	void RotateRelative(const D3DXVECTOR3 *vec);
	void RotateAbsolute(const float x, const float y, const float z);
	void RotateAbsolute(const D3DXVECTOR3 *vec);

	void AnimateRender(int StateNum, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);		//필요없음. 지울꺼야!
	const D3DXVECTOR3* GetPosition();

	void SetCoolTime();
	void SetHP()	{ m_HP -= 1; }
	void SetSpeed()	{ m_fSpeed += 100; }
	int GetHP()	{ return m_HP; }
	float GetSpeed()	{ return m_fSpeed; }

protected:

private:
	CObject *m_pObject;

	/* m/s */
	float m_fSpeed;
	int m_HP;

	static float cooldown;
	// atk, def, hp, mp...

};



#endif