#include "Common.hlsl"

// x : ポスタライズ段階数 (Levels)
// y : ノイズ強度 (Noise)
// z : リムライトの強さ (Rim Power)
// w : 経過時間 (Time)

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

// 疑似乱数(ハッシュ関数)
float Hash(float2 p)
{
    float h = dot(p, float2(127.1f, 311.7f));
    return frac(sin(h) * 43758.5453123f);
}

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    float4 lv = normalize(Light.Position - In.WorldPosition); //光源へのベクトル
    float4 normal = normalize(In.Normal);

    float diff = saturate(dot(normal.xyz, lv.xyz));
    float light = diff * 0.5f + 0.5f;

    float4 texColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
    float3 baseColor = texColor.rgb * In.Diffuse.rgb * (Light.Diffuse.rgb * light + Light.Ambient.rgb);

    //---------------------------------------------
    // ポスタライズ(色を段階的に区切って"映え"る絵にする)
    //---------------------------------------------
    float levels = max(2.0f, Parameter.x);
    baseColor = floor(baseColor * levels + 0.5f) / levels;

    //---------------------------------------------
    // ネオン風リムライト(縁を光らせる。時間で色が脈動する)
    //---------------------------------------------
    float4 eyev = normalize(In.WorldPosition - CameraPosition);
    float rim = 1.0f - saturate(dot(-eyev.xyz, normal.xyz));
    rim = pow(rim, max(0.1f, Parameter.z));

    float pulse = 0.5f + 0.5f * sin(Parameter.w * 3.0f);
    float3 rimColorA = float3(0.1f, 0.9f, 1.0f); // シアン
    float3 rimColorB = float3(1.0f, 0.1f, 0.8f); // マゼンタ
    float3 rimColor = lerp(rimColorA, rimColorB, pulse);

    baseColor += rim * rimColor * (0.6f + 0.4f * pulse);

    //---------------------------------------------
    // アニメーションノイズ(時間で動く砂嵐/グリッチ)
    //---------------------------------------------
    float2 seed = In.Position.xy * 0.75f + Parameter.w * 60.0f;
    float n = Hash(seed);
    baseColor += (n - 0.5f) * Parameter.y;

    // ときどき走るスキャンライン
    float scan = frac(In.Position.y * 0.05f - Parameter.w * 0.6f);
    if (scan < 0.03f)
    {
        baseColor *= 1.5f;
    }

    outDiffuse.rgb = saturate(baseColor);
    outDiffuse.a = texColor.a * In.Diffuse.a;
}
