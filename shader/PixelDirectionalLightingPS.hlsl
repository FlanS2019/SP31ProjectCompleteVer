#include "Common.hlsl"
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float3 normal = normalize(In.Normal); // Å© float3 Ç…ïœçX

    float light = -dot(normal, Light.Direction.xyz);
    light = saturate(light);

    outDiffuse.rgba = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a = In.Diffuse.a;

    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);

    float3 refv = reflect(Light.Direction.xyz, normal); // .xyz ïsóvÇ…
    refv = normalize(refv);

    float specular = -dot(eyev, refv);
    specular = saturate(specular);
    specular = pow(specular, 20);
    outDiffuse.rgb += specular;
}