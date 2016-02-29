// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <d3d11.h>
#include <d3dx11.h>
// PCH(Precompiled Header)를 통하여 자주 변경되지 않는 헤더 파일에 대한
// 컴파일을 매번 하지 않아도 되도록 하는 기능을 제공

// d3d11.lib - Direct3D 라이브러리
// d3dx11d.lib - D3DX 유틸리티 확장 라이브러리
// winmm.lib - Windows 멀티미디어 라이브러리(타이머 처리에 유용)
// dxguid.lib - DirectX COM 객체를 위한 GUID를 포함
// dinput8.lib – DirectInput 라이브러리(DirectX 입력 처리)
// effects11.lib, effects11d.lib – 이펙트 라이브러리


#include <Mmsystem.h>
#include <math.h>
// 시간과 관련된 멀티미디어 라이브러리 함수를 사용하기 위하여 다음 헤더 파일을 추가한다.
// 수학 라이브러리 함수를 사용하기 위하여 다음 헤더 파일을 추가한다.

#include <d3dcompiler.h>  	//쉐이더 컴파일 함수를 사용하기 위한 헤더 파일
#include <D3DX10Math.h>	//Direct3D 수학 함수를 사용하기 위한 헤더 파일

#include <D3D9Types.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <vector>
#include <map>

#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01		// 상수 버퍼를 연결할 슬롯 번호

#define PS_SLOT_COLOR			0x00
#define PS_SLOT_LIGHT			0x00
#define PS_SLOT_MATERIAL		0x01
#define PS_SLOT_TEXTURE			0x00
#define PS_SLOT_SAMPLER_STATE	0x00

//프레임 버퍼의 크기와 종횡비(Aspect Ratio)를 나타내는 상수를 다음과 같이 선언한다.
#define FRAME_BUFFER_WIDTH	640
#define FRAME_BUFFER_HEIGHT	480
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH)/float(FRAME_BUFFER_HEIGHT))

// messeage input
#define DIR_FORWARD		0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20

