//SpotLightPS.hlsl
#include "Common.hlsl"
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float spot = 0.0f;
    float light = 0.0f;
    float specular = 0.0f;

    // ライトからピクセルへのベクトル
    float3 Vp = normalize(In.WorldPosition.xyz - Light.Position.xyz);

    float angle = acos(dot(Light.Direction.xyz, Vp));

    if (angle > Light.Angle.x)
    {
        light = 0.0f;
        spot = 0.0f;
        specular = 0.0f;
    }
    else
    {
        spot = saturate(1.0f - pow(abs(angle) / Light.Angle.x, Light.PointLightparam.y));

        float4 normal = normalize(In.Normal);
        light = saturate(-dot(normal.xyz, Vp));

        float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
        float3 refv = normalize(reflect(Vp, normal.xyz));

        specular = saturate(-dot(eyev, refv));
        specular = pow(specular, 30.0f);

        // 距離減衰
        float dist = length(In.WorldPosition.xyz - Light.Position.xyz);
        float ofs = saturate(1.0f - dist / Light.PointLightparam.x);
        spot *= ofs;
    }

    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= Light.Diffuse.rgb * In.Diffuse.rgb * light * spot
                      + Light.Ambient.rgb;
    outDiffuse.a *= In.Diffuse.a;
    outDiffuse.rgb += specular * spot;
}