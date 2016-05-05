#ifndef SCENE_H_
#define SCENE_H_



#include "Object.h"
#include "Shader.h"
#include "Light.h"
#include "ObjectManager.h"


#define MODE_MOUSE		0x01
#define MODE_KEYBOARD	0x02

class CScene
{
public:
	CScene();
	~CScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ProcessInput() = 0;

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);	//수정
	void Render(ID3D11DeviceContext*pd3dDeviceContext);			

	void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);	//추가

protected:
	DWORD m_OperationMode;

	POINT	m_ptOldCursorPos;
	POINT	m_ptNewCursorPos;

	CObjectManager *m_pObjectManager;

private:
	std::vector<CShader*> m_vShaders;

	CLight *m_pLight;
};







class CFirstScene : public CScene
{
public:
	CFirstScene();
	~CFirstScene();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput();

	//void BuildObjects(ID3D11Device *pd3dDevice);
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
		FISRT = 0,
		SECOND,
		END
	};

	void Initialize();
	void Destroy();

	void Change(eSceneType eType);

private:
	std::map<eSceneType, CScene*> m_mScenes;

	eSceneType m_eNow;

	CSceneManager();
};



#endif