#include "Common.hlsl"

Texture2D g_Texture : register(t0);	//写真テクスチャ
SamplerState g_SamplerState : register(s0);

//----------------------------------------------------
// 疑似乱数
//----------------------------------------------------
float Hash(float2 p)
{
    p = frac(p * float2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return frac(p.x * p.y);
}

//----------------------------------------------------
// シアン→マゼンタ→パープルを循環するネオンカラー
//----------------------------------------------------
float3 NeonPalette(float t)
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
    float2 uv = In.TexCoord;                       //0-1 画面全体のUV
    float2 pixelPos = uv * float2(1280.0, 720.0);  //ピクセル座標に変換
    float  time = Parameter.w;

    //================================================
    // 1. 背景 : 斜めに流れるオーロラ風グラデーション
    //================================================
    float diag = (uv.x + uv.y) * 0.5;
    float wave1 = sin(diag * 8.0 - time * 0.6) * 0.5 + 0.5;
    float wave2 = sin(diag * 14.0 + time * 1.1 + 2.0) * 0.5 + 0.5;

    float3 bg = lerp(float3(0.02, 0.02, 0.10), float3(0.12, 0.02, 0.22), wave1);
    bg = lerp(bg, float3(0.0, 0.10, 0.22), wave2 * 0.6);
    bg += NeonPalette(diag * 0.6 + time * 0.05) * 0.10 * wave1;

    //================================================
    // 2. 斜めにスイープするネオンサーチライト×2
    //================================================
    float beamA = frac((uv.x - uv.y) * 1.5 + time * 0.15);
    float beamAmountA = pow(saturate(1.0 - abs(beamA - 0.5) * 2.0), 24.0);
    bg += float3(0.2, 0.9, 1.0) * beamAmountA * 0.9;

    float beamB = frac((uv.x + uv.y) * 1.3 - time * 0.11 + 0.5);
    float beamAmountB = pow(saturate(1.0 - abs(beamB - 0.5) * 2.0), 24.0);
    bg += float3(1.0, 0.15, 0.85) * beamAmountB * 0.9;

    //================================================
    // 3. 中心から広がるパルスリング
    //================================================
    float2 center = float2(0.5, 0.5);
    float distC = length((uv - center) * float2(1280.0 / 720.0, 1.0));
    float ring = frac(distC * 3.0 - time * 0.7);
    float ringGlow = pow(saturate(1.0 - abs(ring - 0.5) * 2.0), 8.0) * saturate(1.0 - distC * 0.9);
    bg += float3(0.6, 0.9, 1.0) * ringGlow * 0.35;

    //================================================
    // 4. きらめくスパークル(星屑)
    //================================================
    float2 sparkleCell = floor(pixelPos / 6.0);
    float sparkleRnd = Hash(sparkleCell);
    float twinkle = sin(time * 3.0 + sparkleRnd * 62.0) * 0.5 + 0.5;
    float sparkle = step(0.985, sparkleRnd) * pow(twinkle, 3.0);
    bg += float3(1.0, 1.0, 1.0) * sparkle * 1.5;

    //================================================
    // 5. 写真ボックスの配置計算
    //    手前の3Dモデルに隠れにくいよう、横長のバナー状に大きく・やや上寄りに配置する
    //================================================
    float photoAspect = max(Parameter.x, 0.01);
    float boxW = 940.0;
    float boxH = 460.0;
    float2 boxCenter = float2(640.0, 260.0);
    float2 rectMin = float2(boxCenter.x - boxW * 0.5, boxCenter.y - boxH * 0.5);
    float2 rectMax = float2(boxCenter.x + boxW * 0.5, boxCenter.y + boxH * 0.5);

    float2 localUV = (pixelPos - rectMin) / (rectMax - rectMin);
    bool insideBox = (localUV.x >= 0.0 && localUV.x <= 1.0 && localUV.y >= 0.0 && localUV.y <= 1.0);

    //================================================
    // 6. ネオンの縁取り(フレームグロー、光が流れる)
    //================================================
    float2 outsideD = max(rectMin - pixelPos, pixelPos - rectMax);
    outsideD = max(outsideD, 0.0);
    float distOutside = length(outsideD);

    float distInsideX = min(pixelPos.x - rectMin.x, rectMax.x - pixelPos.x);
    float distInsideY = min(pixelPos.y - rectMin.y, rectMax.y - pixelPos.y);
    float distInside = min(distInsideX, distInsideY);

    float signedDist = insideBox ? -distInside : distOutside;

    float borderCore = exp(-abs(signedDist) / 3.0);           //芯の明るい線
    float borderGlow = exp(-abs(signedDist) / 26.0) * 0.55;   //外ににじむグロー

    float chase = frac((pixelPos.x + pixelPos.y) * 0.01 - time * 0.8);
    float3 borderColor = NeonPalette(chase);

    float3 frameColor = borderColor * (borderCore * 1.4 + borderGlow);

    //================================================
    // 7. 合成(写真 or 背景 + ネオン縁取り)
    //================================================
    float3 finalColor = bg;

    if (insideBox)
    {
        //ボックスの縦横比に合わせて写真をカバー・クロップ(縦長写真を横長バナーいっぱいに敷き詰める)
        float boxAspect = boxW / boxH;
        float2 uvScale = (boxAspect > photoAspect)
            ? float2(1.0, photoAspect / boxAspect)
            : float2(boxAspect / photoAspect, 1.0);
        float2 uvOffset = (1.0 - uvScale) * 0.5;
        float2 sampleUV = localUV * uvScale + uvOffset;

        float4 photoColor = g_Texture.Sample(g_SamplerState, sampleUV);
        //写真をほんのりネオンで発光させる
        float3 photoGraded = photoColor.rgb * float3(1.05, 1.0, 1.12) + borderColor * borderGlow * 0.15;
        finalColor = photoGraded;
    }

    finalColor += frameColor;

    //================================================
    // 8. スキャンライン & ビネット & 微細ノイズ
    //================================================
    float scan = sin(pixelPos.y * 3.14159) * 0.5 + 0.5;
    finalColor *= lerp(1.0, 0.90, scan * 0.15);

    float vig = distance(uv, float2(0.5, 0.5));
    finalColor *= saturate(1.35 - vig * 1.1);

    float grain = (Hash(pixelPos + time * 60.0) - 0.5) * 0.03;
    finalColor += grain;

    outColor = float4(saturate(finalColor), 1.0);
}
