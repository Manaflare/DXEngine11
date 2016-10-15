//--------------------------------------------------------------------------------------
// File: BasicHLSL11_VS.hlsl
//
// The vertex shader file for the BasicHLSL11 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//cbuffer cbPerObject : register( b0 )
//{
//	matrix		g_mWorldViewProjection	: packoffset( c0 );
//	matrix		g_mWorld				: packoffset( c4 );
//};

cbuffer MatrixBuffer
{
	matrix WVP;
	matrix WorldMatrix;
};


//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 vPosition	: POSITION;
	float2 vTexcoord	: TEXCOORD0;
	float3 vNormal		: NORMAL;
};

struct VS_OUTPUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexcoord	: TEXCOORD0;
	float3 vNormal		: NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain( VS_INPUT Input )
{
	VS_OUTPUT Output;
	
	Output.vPosition = mul(Input.vPosition, WVP);
	Output.vNormal = mul( Input.vNormal, WorldMatrix );
	Output.vTexcoord = Input.vTexcoord;
	
	return Output;
}

