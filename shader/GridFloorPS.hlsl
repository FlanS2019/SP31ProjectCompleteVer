#include "Common.hlsl"

void main(in PS_IN In, out float4 outColor : SV_TARGET)
{
    float2 worldXZ = In.WorldPosition.xz;
    float  time = Parameter.w;

    //細かいグリッド線(スクロールアニメーション)
    float2 gridUV = worldXZ * 2.0 - float2(0.0, time * 0.6);
    float2 gridLine = abs(frac(gridUV) - 0.5);
    float lineWidth = 0.035;
    float gx = smoothstep(lineWidth, 0.0, gridLine.x);
    float gy = smoothstep(lineWidth, 0.0, gridLine.y);
    float grid = saturate(gx + gy);

    //太い区画線
    float2 majorUV = worldXZ * 0.4 - float2(0.0, time * 0.12);
    float2 majorLine = abs(frac(majorUV) - 0.5);
    float majorWidth = 0.02;
    float majorGrid = saturate(smoothstep(majorWidth, 0.0, majorLine.x) + smoothstep(majorWidth, 0.0, majorLine.y));

    //カメラからの距離で色をグラデーション(近い=マゼンタ、遠い=シアン)
    float dist = length(In.WorldPosition.xyz - CameraPosition.xyz);
    float t = saturate(dist / 14.0);
    float3 nearColor = float3(1.0, 0.1, 0.85);
    float3 farColor  = float3(0.05, 0.9, 1.0);
    float3 lineColor = lerp(nearColor, farColor, t);

    float3 baseFloor = float3(0.015, 0.01, 0.04);

    float3 color = baseFloor;
    color += lineColor * grid * 0.9;
    color += lineColor * majorGrid * 1.3;

    //中心のほのかな発光(スポットライト風)
    float centerGlow = saturate(1.0 - dist / 9.0);
    color += float3(0.3, 0.15, 0.5) * centerGlow * 0.15;

    //遠くは背景へフェードアウト
    float fade = saturate(1.0 - dist / 16.0);

    outColor = float4(color, fade);
}
