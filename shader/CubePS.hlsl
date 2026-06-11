/*==============================================================================
   [SpotLightPS.hlsl]
==============================================================================*/

// -----------------------------------------------------------------------
// 定数バッファ
// -----------------------------------------------------------------------

// マテリアル（スロット3）
// ※ renderer.h の MATERIAL 構造体の順番に合わせる
//    MATERIAL { Ambient, Diffuse, Specular, Emission, Shininess }
cbuffer MaterialBuffer : register(b3)
{
    float4 MaterialAmbient;    // ← Ambientが先頭（renderer.hと一致）
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmissive;
    float  MaterialShininess;
    float3 _pad0;
}

// スポットライト（スロット4）
cbuffer SpotLightBuffer : register(b4)
{
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightPosition;
    float4 LightDirection;
    float  LightConeAngle;
    float  LightAttenuation;
    float  LightPow;
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
// 入力
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
    float3 N       = normalize(input.Normal.xyz);
    float3 toLight = LightPosition.xyz - input.WorldPos.xyz;
    float  dist    = length(toLight);
    float3 L       = normalize(toLight);
    float3 spotDir = normalize(LightDirection.xyz);

    // コーン判定
    float cosHalfAngle = cos(radians(LightConeAngle));
    float cosTheta     = dot(-L, spotDir);

    float4 texColor = DiffuseTexture.Sample(SamplerState0, input.TexCoord);
    float4 ambient  = MaterialAmbient * LightAmbient * texColor;

    // コーン外なら環境光のみ
    if (cosTheta < cosHalfAngle)
    {
        return saturate(ambient);
    }

    // スポット・距離減衰
    float spotFactor  = pow(cosTheta, LightPow);
    float attenuation = 1.0f / (1.0f + LightAttenuation * dist * dist);

    // 拡散反射
    float NdotL    = max(dot(N, L), 0.0f);
    float4 diffuse = MaterialDiffuse * LightDiffuse * texColor * NdotL;

    // 鏡面反射（Blinn-Phong）
    float3 V        = normalize(CameraPosition.xyz - input.WorldPos.xyz);
    float3 H        = normalize(L + V);
    float NdotH     = max(dot(N, H), 0.0f);
    float4 specular = MaterialSpecular * pow(NdotH, max(MaterialShininess, 1.0f));

    // 合成
    float4 color = ambient + (diffuse + specular) * spotFactor * attenuation;
    color.a = texColor.a * MaterialDiffuse.a;

    return saturate(color);
}
