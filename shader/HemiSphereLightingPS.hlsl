#include "Common.hlsl"


// ─── PixelShader ───────────────────────────────────────────────────────────────
float4 main(in PS_IN In) : SV_Target
{
    float3 N = normalize(In.Normal);

    // ────────────────────────────────────────────────────────
    // 1. ビューワー方向（カメラ位置 - ワールド座標）
    //    CameraPosition は Common.hlsl にある想定。無ければ
    //    float3 viewDir = normalize(CameraPos.xyz - In.WorldPosition.xyz);
    // ────────────────────────────────────────────────────────
    float3 V = normalize(Camera.Position.xyz - In.WorldPosition.xyz);

    // 2. ライトベクトル（サーフェス→ライト）
    float3 L = normalize(-Light.Direction.xyz);

    // ────────────────────────────────────────────────────────
    // 3. 半球ライティング（アンビエント代わり）
    //    NdotUp を [-1,1]→[0,1] にリマップするのが正しい半球補間
    // ────────────────────────────────────────────────────────
    float up = dot(N, Light.GroundNormal.xyz); // -1 〜 +1
    float hemi = up * 0.5f + 0.5f; // 0 〜 1
    float3 hemiColor = lerp(Light.GroundColor.xyz, Light.SkyColor.xyz, hemi);

    // ────────────────────────────────────────────────────────
    // 4. ディフューズ
    // ────────────────────────────────────────────────────────
    float NdotL = saturate(dot(N, L));
    float3 diffuse = Light.Diffuse.xyz * NdotL;

    // ────────────────────────────────────────────────────────
    // 5. スペキュラー（黄色いハイライト）
    //    Blinn-Phong: ハーフベクトル使用 → より物理的に自然
    // ────────────────────────────────────────────────────────
    float3 H = normalize(L + V);
    float NdotH = saturate(dot(N, H));
    float spec = pow(NdotH, 64.0f); // 指数を上げるほど小さく鋭い
    float3 specColor = float3(1.0f, 0.85f, 0.1f) * spec * 3.0f;

    // ────────────────────────────────────────────────────────
    // 6. フレネル反射（縁に向かって緑が薄くなり透過感が出る）
    //    Schlick近似: F = F0 + (1-F0)*(1-NdotV)^5
    //    ガラス系: F0 ≈ 0.04
    // ────────────────────────────────────────────────────────
    float NdotV = saturate(dot(N, V));
    float F0 = 0.04f;
    float fresnel = F0 + (1.0f - F0) * pow(1.0f - NdotV, 5.0f);

    // ────────────────────────────────────────────────────────
    // 7. 半透明ガラスの色 = 透過色 + フレネルハイライト
    //    透過時は黄緑っぽく、縁はより鮮やかな緑
    // ────────────────────────────────────────────────────────
    float3 transmitColor = float3(0.55f, 0.95f, 0.05f); // 黄緑（中心部）
    float3 edgeColor = float3(0.0f, 0.80f, 0.05f); // 深緑（縁）
    float3 baseColor = lerp(transmitColor, edgeColor, fresnel);

    // ────────────────────────────────────────────────────────
    // 8. 合成
    // ────────────────────────────────────────────────────────
    float3 ambient = Light.Ambient.xyz;

    float3 rgb =
        baseColor * (ambient + hemiColor * 0.4f) // アンビエント + 半球
      + baseColor * diffuse // ディフューズ
      + specColor * (1.0f - baseColor.g * 0.5f) // スペキュラー（緑が濃い所は抑制）
      + fresnel * float3(0.6f, 0.9f, 0.2f) * 0.3f; // フレネルエッジグロー

    // ────────────────────────────────────────────────────────
    // 9. 半透明アルファ（フレネルで縁を不透明に）
    // ────────────────────────────────────────────────────────
    float alpha = lerp(0.45f, 0.95f, fresnel);

    return float4(rgb, alpha);
}