#pragma once

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "targetver.h"



#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Mmsystem.h> 

#include <d3d11_1.h>
#include <d3dcompiler.h>  	//쉐이더 컴파일 함수를 사용하기 위한 헤더 파일
#include <DirectXMath.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <vector>
#include <map>
#include <string>
#include <iostream> 
#include <fstream>
#include <assert.h>

#include <thread>
#include <WinSock2.h>

#include "CommonState.h"

using namespace std;

#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define VS_SLOT_FOG						0x02
#define VS_SLOT_BONE_MATRIX				0x03

#define PS_SLOT_LIGHT					0x00
#define PS_SLOT_MATERIAL				0x01

#define PS_SHADERRESOURCE_SLOT_TEXTURE	0x00
#define PS_SAMPLER_SLOT_SAMPLER_STATE	0x00

//#define FRAME_BUFFER_WIDTH	800	/* 프레임 버퍼의 너비 */
//#define FRAME_BUFFER_HEIGHT	600	/* 프레임 버퍼의 높이 */
#define FRAME_BUFFER_WIDTH	1280	/* 프레임 버퍼의 너비 */
#define FRAME_BUFFER_HEIGHT	960	/* 프레임 버퍼의 높이 */
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH)/float(FRAME_BUFFER_HEIGHT))	/* 프레임 버퍼의 종횡비 */

#define COLORRGB(x) (x/255.0f)

// messeage input
#define DIR_FORWARD		0x01	// 0000 0001
#define DIR_BACKWARD	0x02	// 0000 0010
#define DIR_LEFT		0x04	// 0000 0100
#define DIR_RIGHT		0x08	// 0000 1000

// animation
#define ANIMATION_COUNT	7		// 오브젝트가 행동하는 애니메이션의 개수 :: idle, run, dead, attack, skill1, skill2 , skill3
#define ATTACK_COUNT	4		// 공격모션 개수 :: attack, skill1, skill2, skill3 , 히트박스 읽어올때 개수좀 쓸라고!!

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)			                                       \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(WFILE, (DWORD)__LINE__, hr, L#x, true);	   \
		}                                                      \
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 