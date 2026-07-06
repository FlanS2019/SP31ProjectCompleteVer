Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

struct SKY_PS_IN
{
    float4 Position : SV_POSITION;
    float3 LocalDir : TEXCOORD0;
};

float4 main(SKY_PS_IN In) : SV_Target
{
    float3 dir = normalize(In.LocalDir);
    float u = 0.5f + atan2(dir.z, dir.x) / (2.0f * 3.14159265f);
    float v = 0.5f - asin(dir.y) / 3.14159265f;
    return g_Texture.Sample(g_SamplerState, float2(u, v));
}