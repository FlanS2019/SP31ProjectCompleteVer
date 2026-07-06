#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "model.h"
#include "IGameObject3D.h"
using namespace DirectX;

class SkyBall : public IGameObject3D
{
public:
    HRESULT Init(void);
    void Finalize(void);
    void Update(void);
    void Draw(void);

private:
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* VertexLayout = nullptr;

    ID3D11RasterizerState* RasterizerState = nullptr;

    ID3D11SamplerState* SamplerState = nullptr;

    MODEL* Model = nullptr; // ModelLoadÇÃñﬂÇËíl
    int TexIDSky = -1; // TextureLoadÇÃñﬂÇËíl

    XMFLOAT3 Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMFLOAT3 Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMFLOAT3 Scale = XMFLOAT3(500.0f, 500.0f, 500.0f); // è\ï™ëÂÇ´Ç≠
};