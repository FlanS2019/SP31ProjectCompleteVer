/*==============================================================================
[SmokePS.hlsl]
==============================================================================*/
cbuffer MaterialBuffer : register(b3)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmissive;
    float MaterialShininess;
    float3 _pad0;
}

Texture2D DiffuseTexture : register(t0);
SamplerState SamplerState0 : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, MaterialDiffuse.a); // 赤だが、アルファはマテリアル依存
}