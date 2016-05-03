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
#include <d3d11.h>
#include <d3dx11.h>

#include <Mmsystem.h>
#include <math.h>
#include <d3dcompiler.h>  	//쉐이더 컴파일 함수를 사용하기 위한 헤더 파일
#include <D3DX10Math.h>		//Direct3D 수학 함수를 사용하기 위한 헤더 파일
#include <D3D9Types.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <vector>
#include <map>
#include <xnamath.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <assert.h>

#include <thread>
#include <WinSock2.h>

using namespace std;

#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define VS_SLOT_FOG_CENTER				0x02
#define VS_SLOT_FOG_RANGE				0x03

#define PS_SLOT_LIGHT					0x00
#define PS_SLOT_MATERIAL				0x01

#define PS_SHADERRESOURCE_SLOT_TEXTURE	0x00
#define PS_SAMPLER_SLOT_SAMPLER_STATE	0x00

#define FRAME_BUFFER_WIDTH	640	/* 프레임 버퍼의 너비 */
#define FRAME_BUFFER_HEIGHT	480	/* 프레임 버퍼의 높이 */
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH)/float(FRAME_BUFFER_HEIGHT))	/* 프레임 버퍼의 종횡비 */

#define COLORRGB(x) (x/255.0f)

// messeage input
#define DIR_FORWARD		0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20

#define ANIMATION_COUNT 5		//아직은 안씀, 최대한 5로 통일 시켜야 편리함... 





/* 서버_ 재정의때문에 일단 여기로 옮겨둠*/
#define MAX_BUFF_SIZE	4096
#define MAX_PACKET_SIZE	4096
#define MAX_CHAT_SIZE	100

#define MAX_USER	1000
#define SERVER_PORT	4000

// 서버 -> 클라
#define POS			1
#define PUT_PLAYER	2
#define LOGIN		3

// 클라 -> 서버
#define CREATE_ROOM	1
#define JOIN_ROOM	2
#define PLAYER_MOVE	3






#endif