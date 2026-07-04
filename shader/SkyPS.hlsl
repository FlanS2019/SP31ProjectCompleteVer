Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

struct PS_IN
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
    float3 Normal : NORMAL;
};

float4 main(PS_IN In) : SV_Target
{
    // ライティング計算なし、テクスチャそのまま出力
    return g_Texture.Sample(g_SamplerState, In.TexCoord);
}