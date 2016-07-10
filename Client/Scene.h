#ifndef SCENE_H_
#define SCENE_H_

#include "Object.h"
#include "Shader.h"
#include "Light.h"
#include "ObjectManager.h"
#include "Fog.h"
#include "Camera.h"
#include "Server.h"

#define MODE_MOUSE		0x01
#define MODE_KEYBOARD	0x02

class CScene
{
public:
	CScene();
	~CScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed) = 0;
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed) = 0;
	virtual void ProcessInput(float fTimeElapsed) = 0;

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	virtual void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);	//추가

protected:
	DWORD m_OperationMode;

	POINT	m_ptOldCursorPos;
	POINT	m_ptNewCursorPos;

	CObjectManager *m_pObjectManager;
	CCameraManager *m_pCameraManager;

private:
	std::vector<CShader*> m_vShaders;

	CLight *m_pLight;
};







class CFirstScene : public CScene
{
public:
	CFirstScene();
	~CFirstScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	virtual void ProcessInput(float fTimeElapsed);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);

private:
	UCHAR m_pKeyBuffer[256];
	DWORD m_dwDirectionPrev;
	DWORD m_dwDirectionNow;


	CFog *m_pFog;
};



class CSecondScene : public CScene
{
public:
	CSecondScene();
	~CSecondScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	virtual void ProcessInput(float fTimeElapsed);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);

private:
	int PressSkillNum;					//누른 스킬 번호
	int Player_Attack_number = 3;		//3: 평타, 4,5,6 : 1~3 스킬		타입을 보내는 용도.. 
	bool iscoll = false;
};






class CSceneManager
{
public:
	static CSceneManager* GetSingleton();
	~CSceneManager();

	enum class eSceneType : BYTE{
		START = 0,
		FIRST = 0,
		SECOND,
		END
	};

	void Initialize();
	void Destroy();

	void Change(eSceneType eType);
	CScene* GetNowScene();

private:
	std::map<eSceneType, CScene*> m_mScenes;

	eSceneType m_eNow;

	CSceneManager();
};



#endif