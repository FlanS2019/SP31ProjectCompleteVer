#include "common.hlsl"
#include "CookTorranceSub.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

float CalculateDiffuseFromFresnel(
    float3 N,
    float3 L,
    float3 V);

float CalculateCookTorranceSpecular(
    float3 L,
    float3 V,
    float3 N,
    float smooth,
    float metallic);

static const float PI = 3.1415926f;

void main(
    in PS_IN In,
    out float4 outDiffuse : SV_Target)
{
    //-----------------------
    // 法線
    //-----------------------

    float3 normal =
        normalize(In.Normal.xyz);

    //-----------------------
    // モデルの色
    //-----------------------

    float4 albedoColor =
        g_Texture.Sample(
            g_SamplerState,
            In.TexCoord);

    //-----------------------
    // ImGui値
    //-----------------------

    float smooth =
        saturate(Parameter.x);

    float metallic =
        saturate(Parameter.y);

    //-----------------------
    // カメラ方向
    //-----------------------

    float3 eyev =
        normalize(
            CameraPosition.xyz -
            In.WorldPosition.xyz);

    float3 lit = 0;

    //-----------------------
    // ライト方向
    //-----------------------

    float3 lv =
        normalize(
            Light.Position.xyz -
            In.WorldPosition.xyz);

    //-----------------------
    // Lambert
    //-----------------------

    float nl =
        saturate(
            dot(
                normal,
                lv));

    //-----------------------
    // Fresnel拡散
    //-----------------------

    float diffuseFromFresnel =
        CalculateDiffuseFromFresnel(
            normal,
            lv,
            eyev);

    //-----------------------
    // 拡散反射
    //-----------------------

    float3 diffuse =
        albedoColor.rgb *
        diffuseFromFresnel *
        Light.Diffuse.rgb *
        nl;

    //-----------------------
    // 鏡面反射
    //-----------------------

    float3 spec =
        CalculateCookTorranceSpecular(
            lv,
            eyev,
            normal,
            smooth,
            metallic);

    spec *= Light.Diffuse.rgb;

    //-----------------------
    // Metallic補間
    //-----------------------

    spec *= lerp(
        float3(
            1.0f,
            1.0f,
            1.0f),

        albedoColor.rgb,

        metallic);

    //-----------------------
    // 合成
    //-----------------------

    lit =
        diffuse *
        (1.0f - smooth)

        +

        spec;

    //-----------------------
    // 環境光
    //-----------------------

    lit +=
        Light.Ambient.rgb *
        albedoColor.rgb;

    //-----------------------
    // 出力
    //-----------------------

    outDiffuse =
        float4(
            lit,
            albedoColor.a);
}


float CalculateCookTorranceSpecular(
    float3 L,
    float3 V,
    float3 N,
    float smooth,
    float metallic)
{
    float3 H =
        normalize(L + V);

    float nh =
        saturate(dot(N, H));

    float vh =
        saturate(dot(V, H));

    float nl =
        saturate(dot(N, L));

    float nv =
        saturate(dot(N, V));

    float D =
        CalculateBeckmann(
            smooth,
            nh);

    float F =
        CalculateFresnel(
            metallic,
            vh);

    float G =
        CalculateGeometricDamping(
            nh,
            nv,
            nl,
            vh);

    float m =
        PI *
        nv *
        nh;

    return max(
        F * D * G / m,
        0.0f);
}

float CalculateDiffuseFromFresnel(
    float3 N,
    float3 L,
    float3 V)
{
    float nl =
        saturate(dot(N, L));

    float nv =
        saturate(dot(N, V));

    return nl * nv;
}