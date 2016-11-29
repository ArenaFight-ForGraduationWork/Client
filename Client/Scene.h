#pragma once

#include "Shader.h"
#include "Light.h"
#include "ObjectManager.h"
#include "Fog.h"
#include "Camera.h"
#include "Server.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Particle.h"





//
//	Scene ( base class )
//
class CScene
{
public:
	CScene();
	~CScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ProcessInput() = 0;

	virtual void BuildObjects();
	void ReleaseObjects();

	virtual void AnimateObjectsAndRender();

	virtual void ChangeState() {}

protected:
	POINT	m_ptOldCursorPos;
	POINT	m_ptNewCursorPos;

	UCHAR m_pKeyBuffer[256];

	CObjectManager *m_pObjectManager;
	CCameraManager *m_pCameraManager;

	unique_ptr<SpriteBatch> m_pSpriteBatch;
	vector<ID3D11ShaderResourceView*> m_vTextures;
	unique_ptr<SpriteFont> m_pSpriteFont;

private:
	vector<CShader*> m_vShaders;

	CLight *m_pLight;
};





//
//	Intro Scene
//
class CIntroScene : public CScene
{
public:
	CIntroScene();
	~CIntroScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput() {}

	virtual void BuildObjects();
	virtual void AnimateObjectsAndRender();

	virtual void ChangeState();

private:
	BYTE m_bButton;	// 0=아무것도 안 눌림, 1=방 생성(방이름 입력), 2=방 생성(스테이지 입력), 3=방 입장(방이름 입력)

	vector<string> m_vStrings;
	string *m_pTempString;

	const RECT rFramePos = { 0,0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT - 20 };

	const RECT rButton1pos = { FRAME_BUFFER_WIDTH / 16 * 9, FRAME_BUFFER_HEIGHT / 16 * 13,  FRAME_BUFFER_WIDTH / 16 * 11, FRAME_BUFFER_HEIGHT / 16 * 15 };
	const POINT pButton1Size = { FRAME_BUFFER_WIDTH / 8 , FRAME_BUFFER_HEIGHT / 8 };	// w/16*2, h/16*2
	const RECT rButton2pos = { FRAME_BUFFER_WIDTH / 16 * 13, FRAME_BUFFER_HEIGHT / 16 * 13, FRAME_BUFFER_WIDTH / 16 * 15, FRAME_BUFFER_HEIGHT / 16 * 15 };
	const POINT pButton2Size = { FRAME_BUFFER_WIDTH / 8 , FRAME_BUFFER_HEIGHT / 8 };	// w/16*2, h/16*2

	const RECT rInputWindowPos = { FRAME_BUFFER_WIDTH / 4 , FRAME_BUFFER_HEIGHT / 4 , FRAME_BUFFER_WIDTH / 4 * 3, FRAME_BUFFER_HEIGHT / 4 * 3 };
	const POINT pInputWindowSize = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
	const RECT rInputWindowButtonPos = { FRAME_BUFFER_WIDTH / 8 * 5, FRAME_BUFFER_HEIGHT / 8 * 5, FRAME_BUFFER_WIDTH / 4 * 3, FRAME_BUFFER_HEIGHT / 4 * 3 };
	const POINT pInputWindowButtonSize = { FRAME_BUFFER_WIDTH / 8, FRAME_BUFFER_HEIGHT / 8 };

	bool _CheckDestination(POINT * pMousePos, const RECT* pDestination);

	void _KeyboardMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void _KeyboardMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void _MouseMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void _MouseMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};





//
//	First Scene
//
class CFirstScene : public CScene
{
public:
	CFirstScene();
	~CFirstScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput();

	virtual void BuildObjects();
	virtual void AnimateObjectsAndRender();

private:
	DWORD m_dwDirectionPrev;
	DWORD m_dwDirectionNow;

	CFog *m_pFog;

	const RECT rFramePos = { 0,0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT - 20 };
	const RECT rMyHpBarBackground = { FRAME_BUFFER_WIDTH / 16 - 5, FRAME_BUFFER_HEIGHT / 32 * 28 - 5, FRAME_BUFFER_WIDTH / 16 * 7 + 5, FRAME_BUFFER_HEIGHT / 32 * 29 + 5 };
	RECT rHpPos = { FRAME_BUFFER_WIDTH / 16, FRAME_BUFFER_HEIGHT / 32 * 28, FRAME_BUFFER_WIDTH / 16 * 7, FRAME_BUFFER_HEIGHT / 32 * 29 };
	const RECT rMonsterHpBarBackground = { FRAME_BUFFER_WIDTH / 4 - 10, FRAME_BUFFER_HEIGHT / 32 * 3 - 10, FRAME_BUFFER_WIDTH / 4 * 3 + 10, FRAME_BUFFER_HEIGHT / 32 * 4 + 10 };
	RECT rMonsterHpPos = { FRAME_BUFFER_WIDTH / 4, FRAME_BUFFER_HEIGHT / 32 * 3, FRAME_BUFFER_WIDTH / 4 * 3, FRAME_BUFFER_HEIGHT / 32 * 4 };

	bool isFireParticle;
	float FireParticleTime = 0.0f;

	vector<CParticle*> m_vParticles;
	unsigned int m_uiParticleNum = 8;
	void RenderParticle();
};





//
// Scene Manager
//
class CSceneManager
{
public:
	static CSceneManager* GetSingleton();
	~CSceneManager();

	enum class eSceneType : BYTE {
		START = 0,
		INTRO = 0,
		FIRST,
		END
	};

	void Initialize();
	void Destroy();

	void Change(eSceneType eType);
	CScene* GetNowScene();

private:
	map<eSceneType, CScene*> m_mScenes;

	eSceneType m_eNow;

	CSceneManager();
};


