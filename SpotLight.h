#pragma once

#include "main.h"
#include "renderer.h"
#include "model.h"

class SpotLight
{
protected:
    XMFLOAT3 Position;
    XMFLOAT3 Scale;
    XMFLOAT3 Rotate;
    int      TexID;

    ID3D11VertexShader* VertexShader;
    ID3D11PixelShader* PixelShader;
    ID3D11InputLayout* VertexLayout;

    MODEL* Model;
    
public:
    HRESULT Init(void);
    void Finalize(void);
    void Update(void);
    void Draw(void);
};