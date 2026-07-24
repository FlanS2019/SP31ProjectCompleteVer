#pragma once

#include "main.h"
#include "renderer.h"

////////////////////
// 構造体
////////////////////

class BumpField
{
protected:
    XMFLOAT3 Position;
    XMFLOAT3 Scale;
    XMFLOAT3 Rotate;
    int      TexID;
    int      MapID;
    int      MapIDBlob;

    ID3D11Buffer* VertexBuffer;   // 頂点バッファ
    ID3D11Buffer* IndexBuffer;    // インデックスバッファ
    ID3D11VertexShader* VertexShader;   // 頂点シェーダーオブジェクト
    ID3D11PixelShader* PixelShader;    // ピクセルシェーダーオブジェクト
    ID3D11InputLayout* VertexLayout;   // 頂点レイアウトオブジェクト

    LIGHT Light;

public:
    HRESULT Init(void);
    void Finalize(void);
    void Update(void);
    void Draw(void);
};

