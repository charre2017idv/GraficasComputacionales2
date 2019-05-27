//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Hacer funcion donde se manden los materiales a llamar para no hacerlo con cada mesh
// Como obtener la textura de normals
// Agragar en change every los nuevos aatrivtos y hacer una funcion que los llene, es parecida a la anterior.
//--------------------------------------------------------------------------------------
//#define VER_LIGTH
#define BLINN
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
};

cbuffer cbChangeOnResize : register( b1 )
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
    matrix World;
    float4 vMeshColor;
	float4 LightDir;					// Light Dir (World Space)
	float4 vViewPosition;               // Camera position (World Space)
	float4 SpecularPower;               // Specular pow factor
	float4 SpecularColor;               // Specular color (sRGBA)
	float4 DifuseColor;                 // Difuse color (sRGBA)
	float4 kDiffuse;                     // Diffuse light constant
	float4 kAmbient;                     // ambient light constant
	float4 kSpecular;                    // Specular light constant
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos		: POSITION;			// Vertex position (Model Space)
    float2 Tex		: TEXCOORD0;		// Vertex Texcoord (Model Space)
	float4 Normal	: NORMAL0;			// Vertex normal (Model Space)
	float3 Tangent	: TANGENT0;			// Vertex Tangent (Model Space)
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
#ifdef VER_LIGTH
	float4 Color : COLOR0;
#else
	float3 wsNormal : TEXCOORD1;  // Vertex normal (World Space)
	float3 wsPos    : TEXCOORD2;  // Vertex position (World Space)
	float3 wsTangent   : TEXCOORD3;  // Vertex tangent (World Space)
#endif

};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;

	// Pos proj space
	float4 wsPos = mul(input.Pos, World);

	// Normal world space
	float3 NormalWS = mul(input.Normal, World);  // Normal World Space
	float3 wsTangent = normalize(mul(float4(Input.Tangent.xyz, 0.0), matWorld));
#ifdef VER_LIGTH
	// Lambert light calc
	float3 LightDirWs = normalize(-LightDir);
	float NdL = dot(LightDirWs, NormalWS);
	float3 wsViewDir = -normalize(wsPos.xyz - vViewPosition.xyz);
	//output.Normal = float4(NdL, NdL, NdL, 1);
#ifdef BLINN
	// Blinn specular calc
	float3 wsReflect = normalize(reflect(-LightDirWs.xyz, NormalWS.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, SpecularPower.x) * NdL;
#else // (Blinn Phong)
	float3 wsHalf = normalize(wsViewDir.xyz + wsLightDir.xyz);
	float wsNdH = max(0.0f, dot(wsNormal.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SpecularPower) * wsNdL;
#endif
	// Light aportation
	float3 Difuse = kDiffuse * DifuseColor.xyz * wsNdL;
	float3 Specular = kSpecular * SpecularColor.xyz * SpecularFactor;
	float3 Ambient = kAmbient * (1.0 - wsNdL) * AmbientColor.xyz;
	output.Color.xyz = Ambient.xyz + Difuse.xyz + Specular.xyz;

	//output.Color  = float4(Ndl.xxx * DifuseColor.xyz, 1);
	//output.Color += float4(SpecularFactor.xxx * SpecularColor.xyz, 1);
#else
	output.wsPos = wsPos.xyz;
	output.TexCoord = float2(Input.TexCoord.x, Input.TexCoord.y);
	output.wsNormal = NormalWS;
	output.wsTangent.xyz = wsTangent.xyz;
#endif
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
#ifdef VER_LIGTH
    //return txDiffuse.Sample( samLinear, input.Tex ) * vMeshColor*input.Normal;
    return txDiffuse.Sample( samLinear, input.Tex ) * input.Color;
#else
	// Lambert light calc
	float3 wsLightDir = -normalize(LightDir.xyz);
	float wsNdL = max(0.0f, dot(wsLightDir.xyz, input.wsNormal.xyz));

	// Blinn specular
	float3 wsViewDir = -normalize(input.wsPos.xyz - vViewPosition.xyz);
	float3 wsReflect = normalize(reflect(-wsLightDir.xyz, input.wsNormal.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, SpecularPower.x) * wsNdL;

	return txDiffuse.Sample(samLinear, input.Tex) * float4((wsNdL.xxx * DifuseColor.xyz) + (SpecularFactor.xxx * SpecularColor.xyz),1.0);
#endif
}
