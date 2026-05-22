#include "Common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 normal = normalize(In.Normal);   
    
    float light =
    -dot(normal.xyz, Light.Direction.xyz);

    outDiffuse = 1;
    outDiffuse.rgb = light;
    outDiffuse.a = In.Diffuse.a;
    
    float norm = dot(normal, Light.GroundNormal);
    norm = (norm + 1) / 2;
    
    float3 hemiColor =
    lerp(Light.GroundColor, Light.SkyColor, norm);
    
    outDiffuse.rgb += hemiColor;
    
    return;
    light = saturate(light);
    
    
    
}






