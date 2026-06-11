#include "Common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 lv = In.WorldPosition - Light.Position;
    float4 ld = length(lv);
    lv = normalize(ld);
    
    float ofs = saturate(1.0f - ld / Light.PointLightparam.x);
    ofs = max(0.0f, ofs);
    
    float4 normal = normalize(In.Normal);
    float light = -dot(normal.xyz, lv.xyz);
    light = saturate(light);
    light *= ofs;
    
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    outDiffuse.rgb *= In.Diffuse.rgb * Light.Diffuse.rgb * light + Light.Ambient.rgb;
    outDiffuse.a *= In.Diffuse.a;
    
    float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    float3 halfv = normalize(eyev + lv.xyz);
 
    float specular = saturate(-dot(normal.xyz, halfv));
    specular = pow(specular, 30.0f);

    // Add specular highlight
    outDiffuse.rgb += (specular * ofs);

    // Lim Lighting
    float lit = 1.0f - max(0.0f, dot(lv.xyz, eyev));

    float lim = 1.0f - max(0.0f, dot(normal.xyz, -eyev));

    lim *= lit;

    lim = pow(lim, Light.PointLightparam.y);
    outDiffuse.rgb += lim * 6.0f;

}