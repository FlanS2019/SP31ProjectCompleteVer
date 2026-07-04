#include "common.hlsl"

PS_IN main(VS_IN In)
{
    PS_IN Out;

    // 位置変換（スカイボールなので視差を消すためカメラ位置基準）
    float4 worldPos = mul(float4(In.Position, 1.0f), WorldMatrix);
    Out.Position = mul(worldPos, mul(ViewMatrix, ProjectionMatrix));

    Out.TexCoord = In.TexCoord;
    Out.WorldPosition = worldPos;
    Out.Normal = mul(In.Normal, (float3x3) WorldMatrix);

    return Out;
}