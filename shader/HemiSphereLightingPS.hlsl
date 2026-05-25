#include "Common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float4 main(in PS_IN In) : SV_Target
{
    float4 outDiffuse;

    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;

    float3 normal = normalize(In.Normal);
    float light = saturate(-dot(normal, Light.Direction.xyz));

    float norm = (dot(normal, Light.GroundNormal.xyz) + 1.0f) / 2.0f;
    float3 hemiColor = lerp(Light.GroundColor.rgb, Light.SkyColor.rgb, norm);

    outDiffuse.rgb *= light;
    outDiffuse.rgb += hemiColor;
    outDiffuse.a = In.Diffuse.a;

    return outDiffuse;
}