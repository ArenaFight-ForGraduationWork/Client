#pragma once

#include "stdafx.h"
#include <DirectXMath.h>
using namespace DirectX;

/*
	#define VS_SLOT_VIEWPROJECTION_MATRIX	0x00
	#define VS_SLOT_WORLD_MATRIX			0x01
	#define VS_SLOT_FOG						0x02
	#define VS_SLOT_BONE_MATRIX				0x03

	#define PS_SLOT_LIGHT					0x00
	#define PS_SLOT_MATERIAL				0x01

	#define PS_SHADERRESOURCE_SLOT_TEXTURE	0x00
	#define PS_SAMPLER_SLOT_SAMPLER_STATE	0x00
*/





//
//	# Variables for Vertex Shader
//
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
	float m_fRange;		// if m_fRange < 0, fog is unable.
};

struct VS_CB_BONE_MATRIX
{
	XMMATRIX m_mtxBone[128];
};





//
//	# Variables for Pixel Shader
//
struct PS_CB_MATERIAL
{
	// R, G, B, A
	float m_fAmbient[4];
	float m_fDiffuse[4];
	float m_fSpecular[4]; // a = power
	float m_fEmissive[4];
};

#define MAX_LIGHTS 4
struct LIGHT
{
	float m_f4Ambient[4];
	float m_f4Diffuse[4];
	float m_f4Specular[4];
	float m_f3Position[3];
	float m_fRange;
	float m_f3Direction[3];
	float m_nType;
	float m_f3Attenuation[3];
	float m_fFalloff;
	float m_fTheta; // cos(m_fTheta)
	float m_fPhi;	// cos(m_fPhi)
	float m_bEnable;
	float padding;
};
struct PS_CB_LIGHT
{
	// 상수 버퍼는 크기가 16의 배수가 되어야 한다. 
	LIGHT m_pLights[MAX_LIGHTS];
	float m_f4GlobalAmbient[4];
	float m_f4CameraPosition[4];
};