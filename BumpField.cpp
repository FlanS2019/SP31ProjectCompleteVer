#include "BumpField.h"
#include "sprite.h"
#include "texture.h"

//************************************************************
// マクロ定義
//************************************************************
#define NUM_VERTEX (4)
#define SIZE (5)
#define TSIZE (5)


//************************************************************
// グローバル変数
//************************************************************

//頂点データ
/*
    V0-----------V1
    |             |
    |             |
    V2-----------V3
*/

static VERTEX_3D    Box[NUM_VERTEX] =
{
    { // 頂点V0 LEFT-TOP
        XMFLOAT3(-SIZE / 2, 0.0f, SIZE / 2),    // 頂点座標
        XMFLOAT3(0.0f, 1.0f, 0.0f),            // 法線
        XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),      // カラー
        XMFLOAT2(0.0f * TSIZE, 0.0f * TSIZE)     // テクスチャ座標
    },
    { // 頂点V1 RIGHT-TOP
        XMFLOAT3(SIZE / 2, 0.0f, SIZE / 2),     // 頂点座標
        XMFLOAT3(0.0f, 1.0f, 0.0f),            // 法線
        XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),      // カラー
        XMFLOAT2(1.0f * TSIZE, 0.0f * TSIZE)     // テクスチャ座標
    },
    { // 頂点V2 LEFT-BOTTOM
        XMFLOAT3(-SIZE / 2, 0.0f, -SIZE / 2),   // 頂点座標
        XMFLOAT3(0.0f, 1.0f, 0.0f),            // 法線
        XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),      // カラー
        XMFLOAT2(0.0f * TSIZE, 1.0f * TSIZE)     // テクスチャ座標
    },
    { // 頂点V3 RIGHT-BOTTOM
        XMFLOAT3(SIZE / 2, 0.0f, -SIZE / 2),    // 頂点座標
        XMFLOAT3(0.0f, 1.0f, 0.0f),            // 法線
        XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),      // カラー
        XMFLOAT2(1.0f * TSIZE, 1.0f * TSIZE)     // テクスチャ座標
    },
};

//************************************************************
// 初期化処理
//************************************************************
HRESULT BumpField::Init(void)
{
    //テクスチャ読み込み
    TexID = TextureLoad(L"asset\\texture\\sura.jpg");
    MapID = TextureLoad(L"asset\\texture\\normal\\Normal.png");
    MapIDBlob = TextureLoad(L"asset\\texture\\normal\\NormalBlob.png");

    //シェーダー読み込み
    CreateVertexShader(&VertexShader, &VertexLayout, "BumpVS.cso");
    CreatePixelShader(&PixelShader, "BumpPS.cso");

    //頂点バッファ作成
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GetDevice()->CreateBuffer(&bd, NULL, &VertexBuffer);

    //頂点バッファの書き込み
    D3D11_MAPPED_SUBRESOURCE msr;
    GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    //頂点データをコピー
    CopyMemory(&vertex[0], &Box[0], sizeof(VERTEX_3D) * NUM_VERTEX);

    //書き込み完了
    GetDeviceContext()->Unmap(VertexBuffer, 0);

    //3Dオブジェクト管理変数の初期化
    Position = XMFLOAT3(0.0f, -0.1f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

    XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
    dir = XMVector3Normalize(dir);
    XMStoreFloat4(&Light.Direction, dir);

    Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    Light.Diffuse = XMFLOAT4(0.75f, 0.95f, 1.0f, 1.0f);   //ネオンシアン寄りのキーライト
    Light.Ambient = XMFLOAT4(0.35f, 0.08f, 0.45f, 1.0f);  //マゼンタ~パープルのアンビエント
    Light.PointLightParam = XMFLOAT4(3000.0f, 0.0f, 0.0f, 1.0f);

    return S_OK;
}

//************************************************************
// 終了処理
//************************************************************
void BumpField::Finalize(void)
{
    //作ったもの解放
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();
    VertexBuffer->Release();
}

//************************************************************
// 更新処理
//************************************************************
void BumpField::Update(void)
{
    ImGui::Begin("Light");
    {
        ImGui::SliderFloat("PositionX", &Light.Position.x, -3.0f, 3.0f, "%.1f");
        ImGui::SliderFloat("PositionY", &Light.Position.y, -3.0f, 3.0f, "%.1f");
        ImGui::SliderFloat("PositionZ", &Light.Position.z, -3.0f, 3.0f, "%.1f");
    }
    ImGui::End();
}

//************************************************************
// 描画処理
//************************************************************
void BumpField::Draw(void)
{
    // 頂点レイアウト設定
    GetDeviceContext()->IASetInputLayout(VertexLayout);

    // シェーダー設定
    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    SetLight(Light);

    // テクスチャ設定
    ID3D11ShaderResourceView* tex = GetTexture(TexID);
    GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

    tex = GetTexture(MapID);
    GetDeviceContext()->PSSetShaderResources(1, 1, &tex);

    tex = GetTexture(MapIDBlob);
    GetDeviceContext()->PSSetShaderResources(2, 1, &tex);

    // 行列作成
    XMMATRIX TranslationMatrix =
        XMMatrixTranslation(Position.x, Position.y, Position.z);

    XMMATRIX RotationMatrix =
        XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(Rotate.x),
            XMConvertToRadians(Rotate.y),
            XMConvertToRadians(Rotate.z)
        );

    XMMATRIX ScalingMatrix =
        XMMatrixScaling(Scale.x, Scale.y, Scale.z);

    XMMATRIX WorldMatrix =
        ScalingMatrix * RotationMatrix * TranslationMatrix;

    SetWorldMatrix(WorldMatrix);

    // 頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(
        0,
        1,
        &VertexBuffer,
        &stride,
        &offset
    );

    // プリミティブトポロジー設定
    GetDeviceContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
    );

    // マテリアル設定
    MATERIAL material;
    ZeroMemory(&material, sizeof(MATERIAL));
    material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    // 描画
    GetDeviceContext()->Draw(NUM_VERTEX, 0);
}
