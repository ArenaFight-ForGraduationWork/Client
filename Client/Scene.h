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
#include <SpriteFont.h>

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

	virtual void AnimateObjectsAndRender(float fTimeElapsed);

	virtual void ChangeState() {}

protected:
	DWORD m_OperationMode;

	POINT	m_ptOldCursorPos;
	POINT	m_ptNewCursorPos;

	UCHAR m_pKeyBuffer[256];

	CObjectManager *m_pObjectManager;
	CCameraManager *m_pCameraManager;

	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
	ID3D11ShaderResourceView *m_pTexture;							// 삭제 예정
	std::vector<ID3D11ShaderResourceView*> m_vTextures;				// m_pTexture 대신 쓸 거

	std::unique_ptr<DirectX::SpriteFont> m_pSpriteFont;

private:
	std::vector<CShader*> m_vShaders;

	CLight *m_pLight;
};




class CIntroScene : public CScene
{
public:
	CIntroScene();
	~CIntroScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	virtual void ProcessInput(float fTimeElapsed) {}

	virtual void BuildObjects();
	virtual void AnimateObjectsAndRender(float fTimeElapsed);

	virtual void ChangeState();

private:
	BYTE m_bButton;	// 0=아무것도 안 눌림, 1=방 생성(방이름 입력), 2=방 생성(스테이지 입력), 3=방 입장(방이름 입력)

	std::vector<string> m_vStrings;
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

	void KeyboardMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	void KeyboardMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);

	void MouseMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
	void MouseMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed);
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
	virtual void AnimateObjectsAndRender(float fTimeElapsed);

private:
	DWORD m_dwDirectionPrev;
	DWORD m_dwDirectionNow;

	CFog *m_pFog;
};








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
	std::map<eSceneType, CScene*> m_mScenes;

	eSceneType m_eNow;

	CSceneManager();
};



#endif