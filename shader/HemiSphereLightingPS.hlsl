#include "Common.hlsl"

// PixelShader
float4 main(in PS_IN In) : SV_Target
{
    float3 N = normalize(In.Normal);

    // 1. ビューワー方向（カメラ位置 - ワールド座標）
    float3 V = normalize(CameraPosition.xyz - In.WorldPosition.xyz);

    // 2. ライトベクトル（サーフェス→ライト）
    float3 L = normalize(-Light.Direction.xyz);

    // 3. 半球ライティング（アンビエント代わり）
    float up = dot(N, Light.GroundNormal.xyz); // -1 〜 +1
    float hemi = up * 0.5f + 0.5f; // 0 〜 1
    float3 hemiColor = lerp(Light.GroundColor.xyz, Light.SkyColor.xyz, hemi);

    // 4. ディフューズ
    float NdotL = saturate(dot(N, L));
    float3 diffuse = Light.Diffuse.xyz * NdotL;

    // 5. スペキュラー（黄色いハイライト） - Blinn-Phong
    float3 H = normalize(L + V);
    float NdotH = saturate(dot(N, H));
    float spec = pow(NdotH, 64.0f);
    float3 specColor = float3(1.0f, 0.85f, 0.1f) * spec * 3.0f;

    // 6. フレネル（Schlick 近似）
    float NdotV = saturate(dot(N, V));
    float F0 = 0.04f;
    float fresnel = F0 + (1.0f - F0) * pow(1.0f - NdotV, 5.0f);

    // 7. 半透明っぽい色合い（中心は黄緑、縁は深緑）
    float3 transmitColor = float3(0.55f, 0.95f, 0.05f);
    float3 edgeColor = float3(0.0f, 0.80f, 0.05f);
    float3 baseColor = lerp(transmitColor, edgeColor, fresnel);

    // 8. 合成
    float3 ambient = Light.Ambient.xyz;
    float3 rgb =
        baseColor * (ambient + hemiColor * 0.4f)
      + baseColor * diffuse
      + specColor * (1.0f - baseColor.g * 0.5f)
      + fresnel * float3(0.6f, 0.9f, 0.2f) * 0.3f;

    // 9. アルファ（フレネルで縁を不透明に）
    float alpha = lerp(0.45f, 0.95f, fresnel);

    return float4(rgb, alpha);
}