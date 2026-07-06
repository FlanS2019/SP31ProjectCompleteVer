#include "common.hlsl"

struct SKY_PS_IN
{
    float4 Position : SV_POSITION;
    float3 LocalDir : TEXCOORD0; // 球の中心からの方向（ローカル座標そのまま）
};

SKY_PS_IN main(VS_IN In)
{
    SKY_PS_IN Out;
    float4 worldPos = mul(In.Position, World);
    Out.Position = mul(worldPos, mul(View, Projection));
    Out.LocalDir = In.Position.xyz; // 球の中心(0,0,0)からの方向 = ローカル座標そのもの
    return Out;
}