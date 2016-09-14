#ifndef STDAFX_H_
#define STDAFX_H_

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
//#include <d3d11.h>	// d3d11.h does not include XNAMATH, so I changed d3d11.h to d3d11_1.h
#include <d3d11_1.h>
#include <d3dx11.h>

#include <Mmsystem.h>
#include <math.h>
#include <d3dcompiler.h>  	//쉐이더 컴파일 함수를 사용하기 위한 헤더 파일
#include <D3DX10Math.h>		//Direct3D 수학 함수를 사용하기 위한 헤더 파일
#include <D3D9Types.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
/*
	DirectX Tool Kit uses DirectXMath.h header.
	And If you use xnamath.h and DirectXMath.h at once, They will collide and you can see lots of errors... ;).	So, Remove xnamath.h.
	d3d11_1.h includes XNAMATH in the namespace DirectX. You can use like this( DirectX::XMFLOAT4X4 ).
	Or type using namespace DirectX; (You must write this in .cpp file, NOT a .h file.)
*/
//#include <xnamath.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
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
//#define DIR_UP			0x10	// 0001 0000
//#define DIR_DOWN		0x20	// 0010 0000


#define ANIMATION_COUNT	7		// 오브젝트가 행동하는 애니메이션의 개수 :: idle, run, dead, attack, skill1, skill2 , skill3
#define ATTACK_COUNT	4		// 공격모션 개수 :: attack, skill1, skill2, skill3 , 히트박스 읽어올때 개수좀 쓸라고!!


#endif