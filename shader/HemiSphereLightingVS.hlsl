#include "Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    Out = (PS_IN) 0; // ‘Sƒƒ“ƒo[‰Šú‰»

    float4 pos = float4(In.Position.xyz, 1.0f);
    pos = mul(pos, World);
    Out.WorldPosition = pos;
    pos = mul(pos, View);
    pos = mul(pos, Projection);

    Out.Position = pos;
    Out.Normal = In.Normal;
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse;
}