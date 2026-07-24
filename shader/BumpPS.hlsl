#include "Common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
Texture2D g_TextureNormalBlob : register(t2);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 lv = In.WorldPosition - Light.Position;
    float4 ld = length(lv);
    lv = normalize(lv);
    
    float ofs = saturate(1.0f - ld / Light.PointLightparam.x);
    ofs = max(0.0f, ofs);
    
    //細かい凹凸(既存)
    float4 tmpnormal = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    tmpnormal = normalize((tmpnormal * 2.0f) - 1.0f);

    //大きい塊用(タイリングを粗くサンプリングして、床全体に大きく散らばるように)
    float4 tmpnormalBlob = g_TextureNormalBlob.Sample(g_SamplerState, In.TexCoord * 0.3f);
    tmpnormalBlob = normalize((tmpnormalBlob * 2.0f) - 1.0f);

    //2枚の法線をブレンド(細かい粒 + 大きい盛り上がり)
    float3 normal = normalize(tmpnormal.xyz + tmpnormalBlob.xyz);

    float3 N = float3(0.0f, 1.0f, 0.0f);
    float3 T = float3(1.0f, 0.0f, 0.0f);
    float3 B = float3(0.0f, 0.0f, 1.0f);
    float3x3 Rot = float3x3(T, B, N);
    normal = normalize(mul(normal, Rot));

    float light = -dot(normal.xyz, lv.xyz);
    light = saturate(light);
    light *= ofs;
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= Light.Diffuse.rgb * In.Diffuse.rgb * light + Light.Ambient.rgb;
    outDiffuse.a *= In.Diffuse.a;
    
    float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    float3 halfv = normalize(eyev + lv.xyz);
 
    float specular = saturate(-dot(normal.xyz, halfv));
    specular = pow(specular, 30.0f);
    
    outDiffuse.rgb += (specular * ofs);
}