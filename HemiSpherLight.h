#pragma once

#include "Header.h"

class HemiSphereLighting : public IGameObject3D
{
protected:
    int      TexID;

    ID3D11VertexShader* VertexShader; 
    ID3D11PixelShader* PixelShader;
    ID3D11InputLayout* VertexLayout;  

    LIGHT Light;

    MODEL* Model;

public:
    HRESULT Init(void);
    void Finalize(void);
    void Update(void);
    void Draw(void);
    XMFLOAT3 Position;
    XMFLOAT3 Scale;
    XMFLOAT3 Rotate;

};
