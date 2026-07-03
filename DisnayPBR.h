#pragma once

#include "main.h"
#include "renderer.h"
#include "model.h"
#include "IGameObject3D.h"

class DisnayPBR : public IGameObject3D
{
protected:
    XMFLOAT3 Position;
    XMFLOAT3 Scale;
    XMFLOAT3 Rotate;
    int      TexIDRoughness;
    int      TexIDMetalness;

    ID3D11VertexShader* VertexShader;
    ID3D11PixelShader* PixelShader;
    ID3D11InputLayout* VertexLayout;
    // グローバルライト構造体の追加
    LIGHT Light;

    MODEL* Model;
	XMFLOAT4 Parameter;
    
public:
    HRESULT Init(void);
    void Finalize(void);
    void Update(void);
    void Draw(void);


};