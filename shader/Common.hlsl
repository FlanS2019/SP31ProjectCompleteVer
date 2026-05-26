cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

struct VS_IN
{
    float4 Position : POSITION;
    float4 Normal : NORMAL;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_IN
{
    float4 Position : SV_Position;
    float4 WorldPosition : TEXCOORD1;
    float4 Normal : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct LIGHT
{
    bool Enable;
    uint3 Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
    float4 Position;
    float4 PointLightParam;
    float4 SkyColor;
    float4 GroundColor;
    float4 GroundNormal;
    float4 Range;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}
cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
}
cbuffer ParameterBuffer : register(b6)
{
    float4 Parameter;
}