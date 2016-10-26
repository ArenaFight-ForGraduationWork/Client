#pragma once



#include "stdafx.h"
#include <DirectXMath.h>
using namespace DirectX;



#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define VS_SLOT_FOG						0x02
#define VS_SLOT_BONE_MATRIX				0x03



struct VS_CB_VIEWPROJECTION_MATRIX
{
	XMMATRIX m_mtxView;
	XMMATRIX m_mtxProjection;
};
struct VS_CB_WORLD_MATRIX
{
	XMMATRIX m_mtxWorld;
};
struct VS_CB_FOG
{
	float m_fCenter[3];	// x, y, z
	float m_fRange;		// if m_fRange < 0, fog is enable.
};
struct VS_CB_BONE_MATRIX
{
	XMMATRIX m_mtxBone[128];
};


