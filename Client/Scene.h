#ifndef SCENE_H_
#define SCENE_H_



#include "Object.h"
#include "Shader.h"
#include "Light.h"




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
	void AnimateObjects(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);	//수정
	void Render(ID3D11DeviceContext*pd3dDeviceContext);			

	// 애니메이션 + 렌더 합치기
	void AnimateObjectsAndRender(ID3D11DeviceContext* pd3dDeviceContext, float fTimeElapsed);	//추가

private:
	std::vector<CShader*> m_vShaders;

	CLight *m_pLight;
};



#endif