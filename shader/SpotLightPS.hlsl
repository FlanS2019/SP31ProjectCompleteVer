/*==============================================================================
   [SpotLightPS.hlsl]
==============================================================================*/

// -----------------------------------------------------------------------
// 定数バッファ
// -----------------------------------------------------------------------

// マテリアル（スロット3）
cbuffer MaterialBuffer : register(b3)
{
    float4 MaterialDiffuse;
    float4 MaterialAmbient;
    float4 MaterialSpecular;
    float4 MaterialEmissive;
    float  MaterialShininess;
    float3 _pad0;
}

// スポットライト（スロット4）
cbuffer SpotLightBuffer : register(b4)
{
    float4 LightDiffuse;        // 光の色 (Diffuse-R, G, B, A)
    float4 LightAmbient;        // 環境光の色
    float4 LightPosition;       // ライトのワールド座標 (Position.x, y, z)
    float4 LightDirection;      // ライトの照射方向（正規化済み）
    float  LightConeAngle;      // スポットライトの半頂角（度数法）
    float  LightAttenuation;    // 距離減衰係数
    float  LightPow;            // スポットライトの輝度べき乗（ソフトエッジ）
    float  _pad1;
}

// カメラ座標（スペキュラ計算用, スロット5）
cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
}

// -----------------------------------------------------------------------
// テクスチャ・サンプラー
// -----------------------------------------------------------------------
Texture2D    DiffuseTexture : register(t0);
SamplerState SamplerState0  : register(s0);

// -----------------------------------------------------------------------
// 入力（VSからの出力と一致させる）
// -----------------------------------------------------------------------
struct PS_INPUT
{
    float4 Position  : SV_POSITION;
    float4 Normal    : NORMAL0;
    float4 Diffuse   : COLOR0;
    float2 TexCoord  : TEXCOORD0;
    float4 WorldPos  : TEXCOORD1;
};

//=============================================================================
// ピクセルシェーダーメイン
//=============================================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // -----------------------------------------------------------------------
    // 各ベクトルを正規化
    // -----------------------------------------------------------------------
    float3 N = normalize(input.Normal.xyz);             // サーフェス法線
    float3 toLight = LightPosition.xyz - input.WorldPos.xyz; // フラグメント→ライト
    float  dist    = length(toLight);
    float3 L = normalize(toLight);                       // ライト方向（正規化）
    float3 spotDir = normalize(LightDirection.xyz);      // スポットの照射方向

    // -----------------------------------------------------------------------
    // スポットライトのコーン判定
    //   dot(L_inv, spotDir) = cos(フラグメントとコーン軸のなす角)
    //   ※ L はフラグメント→ライト方向なので、
    //   　 ライト→フラグメント方向は -L
    // -----------------------------------------------------------------------
    float cosHalfAngle = cos(radians(LightConeAngle));
    float cosTheta     = dot(-L, spotDir);               // ライト→フラグメントとコーン軸のcos

    // コーン外なら環境光のみ
    if (cosTheta < cosHalfAngle)
    {
        // 環境光
        float4 texColor = DiffuseTexture.Sample(SamplerState0, input.TexCoord);
        float4 ambient  = MaterialAmbient * LightAmbient * texColor;
        return saturate(ambient);
    }

    // -----------------------------------------------------------------------
    // スポットライト減衰計算
    //   spotFactor : コーン内での角度による減衰（べき乗でソフトエッジ）
    //   attenuation: 距離による減衰
    // -----------------------------------------------------------------------
    float spotFactor   = pow(cosTheta, LightPow);
    float attenuation  = 1.0f / (1.0f + LightAttenuation * dist * dist);

    // -----------------------------------------------------------------------
    // 拡散反射（Diffuse）
    // -----------------------------------------------------------------------
    float NdotL    = max(dot(N, L), 0.0f);
    float4 texColor = DiffuseTexture.Sample(SamplerState0, input.TexCoord);
    float4 diffuse  = MaterialDiffuse * LightDiffuse * texColor * NdotL;

    // -----------------------------------------------------------------------
    // 鏡面反射（Specular / Blinn-Phong）
    // -----------------------------------------------------------------------
    float3 V        = normalize(CameraPosition.xyz - input.WorldPos.xyz);
    float3 H        = normalize(L + V);
    float NdotH     = max(dot(N, H), 0.0f);
    float4 specular = MaterialSpecular * pow(NdotH, max(MaterialShininess, 1.0f));

    // -----------------------------------------------------------------------
    // 環境光
    // -----------------------------------------------------------------------
    float4 ambient  = MaterialAmbient * LightAmbient * texColor;

    // -----------------------------------------------------------------------
    // 合成
    // -----------------------------------------------------------------------
    float4 color = ambient + (diffuse + specular) * spotFactor * attenuation;
    color.a = texColor.a * MaterialDiffuse.a;

    return saturate(color);
}
