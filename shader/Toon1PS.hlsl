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
    
    //–¾‚é‚³’²®
    if(light > Parameter.y)
    {
        light = 1.0f;
    }
    else if(light > Parameter.x)
    {
        light = 0.7f;
    }
    else
    {
        light = (light - Parameter.x) / (Parameter.y - Parameter.x);
    }
    
    light *= ofs;
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    outDiffuse.rgb *= In.Diffuse.rgb * Light.Diffuse.rgb * light + Light.Ambient.rgb;
    outDiffuse.a *= In.Diffuse.a;
    
    //float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    //float3 halfv = normalize(eyev + lv.xyz);
 
    //float specular = saturate(-dot(normal.xyz, halfv));
    //specular = pow(specular, 30.0f);

    //// Add specular highlight
    //outDiffuse.rgb += (specular * ofs);
    
    //ŠÈˆÕƒGƒbƒWì¬
    float4 eyev = In.WorldPosition - CameraPosition;
    eyev = normalize(eyev);
    
    float d = dot(normal, eyev);
    if(d > Parameter.z)
    {
        outDiffuse.rgb *= 0.3f;
    }
}