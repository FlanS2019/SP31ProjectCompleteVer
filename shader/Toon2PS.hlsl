#include "Common.hlsl"
#include "CookTorranceSub.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureRamp : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 lv = Light.Position - In.WorldPosition; //光源へのベクトル
    //float4 lv = In.WorldPosition - Light.Position; //光源へのベクトル
    float4 ld = length(lv);
    lv = normalize(lv);
    
    //float ofs = 1.0f - (1.0f / Light.PointLightparam.x) * ld; //<-- not related to other comment but the first one is pdf ver
    float ofs = saturate(1.0f - ld / Light.PointLightparam.x);
    ofs = max(0, ofs);
    
    float4 normal = normalize(In.Normal);
    
    float light = 0.5f + 0.5f * dot(normal.xyz, lv.xyz);
    //float light = 0.5f + 0.5f * (-dot(normal.xyz, lv.xyz));
    light = clamp(light, 0.01f, 0.99f);
    
    float texv = Parameter.x;
    texv = clamp(texv, 0.01f, 0.99f);
    
    float4 toon = g_TextureRamp.Sample(g_SamplerState, float2(light, texv));
    toon *= ofs;

    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= toon.rgb * In.Diffuse.rgb * Light.Diffuse.rgb + Light.Ambient.rgb;
    outDiffuse.a *= In.Diffuse;
    
    // Create view vector
    float4 eyev = normalize(In.WorldPosition - CameraPosition);

    // Dot product of view vector and normal
    float d = dot(eyev, normal);

    // Determine if this is an edge
    if (d > -0.25)
    {
        outDiffuse.rgb *= 0.3f;
        //outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f); // Draw black outline
    }
}