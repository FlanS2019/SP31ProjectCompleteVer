//CookTorranceSub.hlsl
//マイクラファセット分布関数

float CalculateBeckmann(float smooth, float nh)
{
    if (nh <= 0.0f)
        return 0.0f;
    if (smooth <= 0.0f)
        return 0.0f;

    float smo2 = smooth * smooth;
    float nh2 = nh * nh;
    float nh4 = nh2 * nh2;

    float ex = -(1.0f - nh2) / (smo2 * nh2);
    
    float PI = 3.14159265f;
    float bun = PI * smo2 * nh4;

    return (1.0f / bun) * exp(ex); // ← これを追加
}

float CalculateFresnel(float matal, float hv)
{
    matal = lerp(0.04f, 1.0f, matal);
    return matal + (1.0f - matal) * pow(1.0f - hv, 5.0f);
}

float CalculateGeometricDamping(float nh, float nv, float vh, float nl)
{
    float G;
    vh = max(vh, 0.00001f);
    G = min(1.0f, min(2.0f * nh * vh / nv, 2.0f * nh * vh / nl));
    
    return G;
}