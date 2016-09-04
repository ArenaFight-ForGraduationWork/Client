#ifndef SCENE_H_
#define SCENE_H_

#include "Object.h"
#include "Shader.h"
#include "Light.h"
#include "ObjectManager.h"
#include "Fog.h"
#include "Camera.h"
#include "Server.h"
#include <SpriteBatch.h>

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

	virtual void BuildObjects();
	void ReleaseObjects();

	virtual void AnimateObjectsAndRender3D(float fTimeElapsed);
	virtual void AnimateObjectsAndRender2D(float fTimeElapsed);

protected:
	DWORD m_OperationMode;

	POINT	m_ptOldCursorPos;
	POINT	m_ptNewCursorPos;

	CObjectManager *m_pObjectManager;
	CCameraManager *m_pCameraManager;

	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
	ID3D11ShaderResourceView *m_pTexture;

	ID3D11Buffer *m_pd3dcbCamera;

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

	virtual void BuildObjects();
	virtual void AnimateObjectsAndRender3D(float fTimeElapsed);
	virtual void AnimateObjectsAndRender2D(float fTimeElapsed);

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

	virtual void BuildObjects();
	virtual void AnimateObjectsAndRender(float fTimeElapsed);

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