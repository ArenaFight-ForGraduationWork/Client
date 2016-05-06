#ifndef SCENE_H_
#define SCENE_H_



#include "Object.h"
#include "Shader.h"
#include "Light.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Camera.h"

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

	void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);	//수정
	void Render(ID3D11DeviceContext*pd3dDeviceContext);			

	void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);	//추가

protected:
	DWORD m_OperationMode;

	POINT	m_ptOldCursorPos;
	POINT	m_ptNewCursorPos;

	CPlayer *m_pPlayer;
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
	//void ReleaseObjects();

	//void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);
	//void Render(ID3D11DeviceContext*pd3dDeviceContext);

	//void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);

private:
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