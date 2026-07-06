#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Header.h"

class Wall3D : public IGameObject3D
{
public:
    HRESULT Init(void) override;
    void Finalize(void) override;
    void Update(void) override;
    void Draw(void) override;

    XMFLOAT3 Position;
    XMFLOAT3 Rotate;
    XMFLOAT3 Scale;

private: 
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* VertexLayout = nullptr;
    ID3D11Buffer* VertexBuffer = nullptr;
    int TexID = -1;
};