/*==============================================================================
   [CubePS.hlsl]
==============================================================================*/
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPos : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // 灰色で固定描画（テクスチャ・ライティング無視、形状確認用）
    return float4(0.5f, 0.5f, 0.5f, 1.0f);
}