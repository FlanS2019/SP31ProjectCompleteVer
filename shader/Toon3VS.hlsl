
#include "Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    float4 pos = In.Position;
    float4 norm = normalize(In.Normal) * Parameter.z;
    norm.w = 0.0f;
    pos += norm;
    
    Out.Position = mul(pos, wvp);
    
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0f);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    Out.Normal = worldNormal;
    
    Out.Diffuse.rgb = In.Diffuse.rgb * 0.2f;
    Out.Diffuse.a = 1.0f;
    Out.TexCoord = In.TexCoord;
    
    Out.WorldPosition = mul(In.Position, World);
}