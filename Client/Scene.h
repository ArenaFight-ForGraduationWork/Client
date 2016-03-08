#ifndef SCENE_H_
#define SCENE_H_



#include "Shader.h"

#define MAX_LIGHTS			4 
#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f





struct LIGHT
{
	//상수 버퍼는 크기가 16의 배수가 되어야 한다. 
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular;
	D3DXVECTOR3 m_d3dxvPosition;
	float m_fRange;
	D3DXVECTOR3 m_d3dxvDirection;
	float m_nType;
	D3DXVECTOR3 m_d3dxvAttenuation;
	float m_fFalloff;
	float m_fTheta; //cos(m_fTheta)
	float m_fPhi; //cos(m_fPhi)
	float m_bEnable;
	float padding;
};

struct LIGHTS
{
	LIGHT m_pLights[MAX_LIGHTS];
	D3DXCOLOR m_d3dxcGlobalAmbient;
	D3DXVECTOR4 m_d3dxvCameraPosition;
};


class CScene
{
public:
	CScene();
	~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D11DeviceContext*pd3dDeviceContext);

private:
	std::vector<CShader*> m_vShaders;

	LIGHTS *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;

	void BuildLights(ID3D11Device *pd3dDevice);
	void UpdateLights(ID3D11DeviceContext *pd3dDeviceContext);
	void ReleaseLights();
};








class CSceneManager
{
public:
	static CSceneManager* GetSingleton();
	~CSceneManager();

	enum class eSceneMode : BYTE{
		START = 0,
		FIRST = 0,
		END
	};

	void Render(ID3D11DeviceContext *pd3dDeviceContext);
	void BuildObjects(ID3D11Device *pd3dDevice);

	void AnimateObjects(float fTimeElapsed);

	void ReleaseObjectsInNowScene();
	void ReleaseObjectsInAllScene();
	void Destroy();

private:
	std::map<eSceneMode, CScene*> m_mScenes;

	eSceneMode m_eNowPlayingScene;

	CSceneManager();
};



#endif