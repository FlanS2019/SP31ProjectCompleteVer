#include "common.hlsl"
#include "CookTorranceSub.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureRoughness : register(t1);
Texture2D g_TextureMetalness : register(t2);

SamplerState g_SamplerState : register(s0);

float CalculateDiffuseFromFresnel(float3 N, float3 L, float3 V);
float CalculateCookTorranceSpecular(float3 L, float3 V,
                                    float3 N, float smooth,
                                    float metallic);

static const float PI = 3.1415926f;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 法線を計算
    float4 normal = normalize(In.Normal);

    // 各種マップをサンプリングする
    // アルベドカラー
    float4 albedoColor = g_Texture.Sample(
        g_SamplerState,
        In.TexCoord);

    // スペキュラカラーはアルベドカラーと同じ
    float3 specColor = albedoColor.rgb;

    // 滑らかさを取得
    float smooth =
        g_TextureRoughness.Sample(
        g_SamplerState,
        In.TexCoord).r * 2.0f - 1.0f;

    smooth = Parameter.x; //これは無くてもよい

    smooth = saturate(smooth);

    // 金属度を取得
    float metallic =
        g_TextureMetalness.Sample(
        g_SamplerState,
        In.TexCoord).r * 2.0f - 1.0f;

    metallic = Parameter.y; //これは無くてもよい

    metallic = saturate(metallic);

    // カメラへのベクトル
    float3 eyev =
        CameraPosition.xyz -
        In.WorldPosition.xyz;

    eyev = normalize(eyev);

    // ライトマシマシ
    float3 lit = 0;

    for (int ligNo = 0; ligNo < 1; ligNo++)
    {
        // 光源へのベクトル
        float4 lv =
            Light.Position -
            In.WorldPosition;

        // フレネル反射を考慮した拡散反射
        float diffuseFromFresnel =
            CalculateDiffuseFromFresnel(
                normal.xyz,
                lv.xyz,
                eyev);

        // Lambert
        float nl =
            saturate(dot(
                normal.xyz,
                lv.xyz));

        float3 light =
            nl + Light.Diffuse.rgb / PI;

        // 拡散反射
        float3 diffuse =
        albedoColor.rgb *
        diffuseFromFresnel *
        nl *
        Light.Diffuse.rgb / PI;
        
        // 鏡面反射
        float3 spec =
            CalculateCookTorranceSpecular(
                lv.xyz,
                eyev,
                normal.xyz,
                smooth,
                metallic)
            * Light.Diffuse.rgb;

        // 金属度で補間
        spec *= lerp(
            float3(1.0f, 1.0f, 1.0f),
            specColor,
            metallic);

        // 滑らかさで拡散反射を調整
        lit += diffuse *
               (1.0f - metallic)
               + spec;
    }

    // 環境光
    lit += Light.Ambient.rgb *
           albedoColor.rgb;

    // 出力
    outDiffuse.rgb = lit;
    outDiffuse.a =
        albedoColor.a *
        In.Diffuse.a;
}

float CalculateCookTorranceSpecular(
    float3 L,
    float3 V,
    float3 N,
    float smooth,
    float metallic)
{
    // ライト方向と視線方向のハーフベクトル
    float3 H = normalize(L + V);

    // 各ベクトルがどれくらい似ているかを内積で求める
    float nh = saturate(dot(N, H));
    float vh = saturate(dot(V, H));
    float nl = saturate(dot(N, L));
    float nv = saturate(dot(N, V));

    // D項をベックマン分布を用いて計算
    float D = CalculateBeckmann(smooth, nh);

    // F項をSchlick近似を用いて計算
    float F = CalculateFresnel(metallic, vh);

    // G項を求める
    float G = CalculateGeometricDamping(nh, nv, nl, vh);

    // m項を求める
    float m = 4.0f * nl * nv;
    
    
    // Cook-Torranceモデルの鏡面反射
    return max(F * D * G / m, 0.0f);
}

float CalculateDiffuseFromFresnel(
    float3 N,
    float3 L,
    float3 V)
{
    // 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める
    float nl = saturate(dot(N, L));

    // 法線と視線に向かうベクトルがどれだけ似ているかを内積で求める
    float nv = saturate(dot(N, V));

    // 法線と光源への方向に依存する拡散反射率
    return (nl * nv);
}