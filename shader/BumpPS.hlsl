#include "Common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 lv = In.WorldPosition - Light.Position;
    float4 ld = length(lv);
    lv = normalize(lv);
    
    float ofs = saturate(1.0f - ld / Light.PointLightparam.x);
    ofs = max(0.0f, ofs);
    
    //-----------------------NEW!
    //法線マップ取得
    float4 tmpnormal = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    //RGB値をスケーリング
    tmpnormal = normalize((tmpnormal * 2.0f) - 1.0f);
     
    float3 normal;
    normal.x = tmpnormal.r; //Normal.png
    normal.y = tmpnormal.g;
    normal.z = tmpnormal.b;

    float3 N = float3(0.0f, 1.0f, 0.0f); //Debug
    float3 T = float3(1.0f, 0.0f, 0.0f); //Debug
    float3 B = float3(0.0f, 0.0f, 1.0f); //Debug
    float3x3 Rot = float3x3(T, B, N); //TBN回転行列
    normal = normalize(mul(normal, Rot)); //法線マップのベクトルをTBNで回転
    //-----------------------

    //明るさを計算
    float light = -dot(normal.xyz, lv.xyz);
    light = saturate(light);
    light *= ofs; //明るさを減衰させる
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= Light.Diffuse.rgb * In.Diffuse.rgb * light + Light.Ambient.rgb;
    outDiffuse.a *= In.Diffuse.a;
    

    float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    float3 halfv = normalize(eyev + lv.xyz);
 
    float specular = saturate(-dot(normal.xyz, halfv));
    specular = pow(specular, 30.0f);
    
    outDiffuse.rgb += (specular * ofs);
}
