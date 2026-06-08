/*==============================================================================
   [SpotLightVS.hlsl]
==============================================================================*/

// 定数バッファ（スロット0: ワールド/ビュー/プロジェクション行列）
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// 頂点シェーダー入力
struct VS_INPUT
{
    float4 Position : POSITION0;
    float4 Normal   : NORMAL0;
    float4 Diffuse  : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

// 頂点シェーダー出力（＝ピクセルシェーダー入力）
struct VS_OUTPUT
{
    float4 Position     : SV_POSITION;
    float4 Normal       : NORMAL0;       // ワールド空間の法線
    float4 Diffuse      : COLOR0;
    float2 TexCoord     : TEXCOORD0;
    float4 WorldPos     : TEXCOORD1;     // ワールド空間の頂点座標（ライト計算用）
};

//=============================================================================
// 頂点シェーダーメイン
//=============================================================================
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // ワールド変換
    output.WorldPos = mul(input.Position, World);

    // ビュー・プロジェクション変換
    float4 worldPos = mul(input.Position, World);
    float4 viewPos  = mul(worldPos, View);
    output.Position = mul(viewPos, Projection);

    // 法線をワールド空間へ変換（平行移動成分を除くためw=0）
    output.Normal = normalize(mul(float4(input.Normal.xyz, 0.0f), World));

    output.Diffuse  = input.Diffuse;
    output.TexCoord = input.TexCoord;

    return output;
}
