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
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};


//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 vPosition	: POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexcoord	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float3 vNormal		: NORMAL;
	float2 vTexcoord	: TEXCOORD0;
	float4 vPosition	: SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain( VS_INPUT Input )
{
	VS_OUTPUT Output;
	
	Output.vPosition = mul(Input.vPosition, WorldMatrix);
	Output.vPosition = mul(Output.vPosition, ViewMatrix);
	Output.vPosition = mul(Output.vPosition, ProjectionMatrix);

	Output.vNormal = mul( Input.vNormal, (float3x3)WorldMatrix );
	Output.vTexcoord = Input.vTexcoord;
	
	return Output;
}

