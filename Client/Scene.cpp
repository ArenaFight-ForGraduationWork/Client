#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"

CScene::CScene()
{
	m_pLight = new CLight();
}

CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	CResourceManager *pResourceManager = CResourceManager::GetSingleton(pd3dDevice);
	for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
	{
		m_vShaders.push_back(pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i));
	}

	m_pLight->BuildLights(pd3dDevice);
}

void CScene::ReleaseObjects()
{
	m_vShaders.clear();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::ProcessInput()
{
	return false;
}

void CScene::AnimateObjects(int StateCnt, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed)
{
	for (auto shader : m_vShaders)
	{
		shader->AnimateObjects(StateCnt, pd3dDeviceContext, fTimeElapsed);
	}
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext)
{
	m_pLight->UpdateLights(pd3dDeviceContext);

	for (auto shader : m_vShaders)
	{
		shader->Render(pd3dDeviceContext);
	}
}

void CScene::AnimateObjectsAndRender(ID3D11DeviceContext *pd3dDeviceContext, float time)
{
	m_pLight->UpdateLights(pd3dDeviceContext);

	for (auto shader : m_vShaders)
	{
		shader->AnimateObjectAndRender(pd3dDeviceContext, time);
	}
}




