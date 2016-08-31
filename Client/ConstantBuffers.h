#ifndef CONTANTBUFFERS_H_
#define CONTANTBUFFERS_H_

#include "stdafx.h"



#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
#define VS_SLOT_WORLD_MATRIX			0x01
#define VS_SLOT_FOG						0x02
#define VS_SLOT_BONE_MATRIX				0x03

struct VS_CB_VIEWPROJECTION_MATRIX
{
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;
};
struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
};
struct VS_CB_FOG
{
	D3DXVECTOR3 m_d3dxvCenter;
	float m_fRange;					// if m_fRange < 0, fog is enable.
};
struct VS_CB_BONE_MATRIX
{
	XMMATRIX m_XMmtxBone[128];
};



#endif