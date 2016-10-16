cbuffer MatrixBuffer
{
	matrix WVP;
};


struct VS_INPUT
{
	float4 position	: POSITION;
	float2 texCoord	: TEXCOORD0;
	float3 normal	: NORMAL;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD0;
};

VS_OUTPUT SkyVSMain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.position = mul(float4(input.position.xyz, 1.f), WVP).xyww;
	output.tex = input.position;

	return output;
}