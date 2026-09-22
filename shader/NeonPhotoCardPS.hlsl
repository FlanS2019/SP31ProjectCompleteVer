#include "Common.hlsl"

Texture2D g_Texture : register(t0);	//写真テクスチャ
SamplerState g_SamplerState : register(s0);

//----------------------------------------------------
// シアン→マゼンタ→パープルを循環するネオンカラー
//----------------------------------------------------
float3 NeonPalette2(float t)
{
    float3 c1 = float3(0.0, 0.95, 1.0);    //シアン
    float3 c2 = float3(1.0, 0.05, 0.85);   //マゼンタ
    float3 c3 = float3(0.55, 0.05, 1.0);   //パープル

    float phase = frac(t);
    if (phase < 0.5)
        return lerp(c1, c2, phase * 2.0);
    else
        return lerp(c2, c3, (phase - 0.5) * 2.0);
}

void main(in PS_IN In, out float4 outColor : SV_TARGET)
{
    float2 uv = In.TexCoord;
    float4 tex = g_Texture.Sample(g_SamplerState, uv);
    float  time = Parameter.w;

    //カード端までの距離(0=端、0.5=中心)
    float2 d = abs(uv - 0.5);
    float edgeDist = 0.5 - max(d.x, d.y);

    float borderCore = exp(-abs(edgeDist) / 0.012);          //芯の明るい線
    float borderGlow = exp(-abs(edgeDist) / 0.05) * 0.6;     //外ににじむグロー

    float chase = frac((uv.x + uv.y) * 2.0 - time * 1.2);
    float3 borderColor = NeonPalette2(chase);

    float3 color = tex.rgb * float3(1.05, 1.0, 1.12);
    color += borderColor * (borderCore * 1.5 + borderGlow);

    //ふわっと明滅する発光パルス
    float pulse = 0.85 + 0.15 * sin(time * 2.5);
    color *= pulse;

    outColor = float4(saturate(color), 1.0);
}
