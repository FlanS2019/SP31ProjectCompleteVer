#include "Common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //PointLight
    float4 lv = In.WorldPosition - Light.Position;
    float4 ld = length(lv);
    
    lv = normalize(lv);
    
    float ofs = saturate(1.0f - ld / Light.PointLightParam.x);

    ofs = max(ofs, 0.0f);
    
    float4 normal = normalize(In.Normal);
    float light = -dot(normal.xyz, lv.xyz);
    
    light = saturate(light);
    //ñæÇÈÇ≥å∏êäÇÃåvéZÇÕÇ±Ç±Ç≈çsÇ§Å@ãóó£Ç…âûÇ∂Çƒå∏êäÇ≥ÇπÇÈ
    light *= ofs;
    
    outDiffuse.rgba = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    outDiffuse.rgb *= In.Diffuse.rgb*Light.Diffuse.rgb*light + Light.Ambient;
    outDiffuse.a = In.Diffuse.a;
    
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    float3 halfv = eyev + Light.Direction.xyz;
    halfv = normalize(halfv);
    
    float specular = -dot(halfv, normal.xyz);
    specular = saturate(specular);
    specular = pow(specular, 30);
    
    outDiffuse.rgb += (specular * ofs);
}