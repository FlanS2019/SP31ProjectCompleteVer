#pragma once

#include "Header.h"
#include "model.h"

class CookTorrance : public IGameObject3D
{
protected:
    XMFLOAT3 Position;
    XMFLOAT3 Scale;
    XMFLOAT3 Rotate;
    int      TexID;

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