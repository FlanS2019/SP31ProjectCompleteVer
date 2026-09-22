#include "Toon1.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "keyboard.h"


//************************************************************
// 初期化処理
//************************************************************
HRESULT Toon1::Init(void)
{
    Parameter = XMFLOAT4(0, 0, 0, 0);
    Parameter.x = 0.4f;
    Parameter.y = 0.7f;
    Parameter.z = -0.30f;

    //シェーダー読み込み
    CreateVertexShader(&VertexShader, &VertexLayout, "Toon1VS.cso");
    CreatePixelShader(&PixelShader, "Toon1PS.cso");

    //3Dオブジェクト管理構造体の初期化
    Position = XMFLOAT3(0.0f + (0.5f * 0.0f), 0.2f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

    //モデル読み込み
    Model = ModelLoad("asset\\model\\model.fbx");
    //Model = ModelLoad("asset\\model\\cube.fbx");


    XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
    dir = XMVector3Normalize(dir);
    XMStoreFloat4(&Light.Direction, dir);

    Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    Light.Diffuse = XMFLOAT4(0.75f, 0.95f, 1.0f, 1.0f);   //ネオンシアン寄りのキーライト
    Light.Ambient = XMFLOAT4(0.35f, 0.08f, 0.45f, 1.0f);  //マゼンタ~パープルのアンビエント
    Light.PointLightParam = XMFLOAT4(3.0f, 0.0f, 0.0f, 1.0f);
    Light.Angle.x = XMConvertToRadians(20.0f);


    return S_OK;
}

//************************************************************
// 終了処理
//************************************************************
void Toon1::Finalize(void)
{
    //作ったものを解放
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();

    ModelRelease(Model);
}

//************************************************************
// 更新処理
//************************************************************
void Toon1::Update(void)
{
    //やかましく常時回転させる(リアルタイム感を強調)
    Rotate.y += 40.0f * (1.0f / 60.0f);

    //適当に回転
    if (Keyboard_IsKeyDown(KK_UP))
    {
        Position.z += 0.3f * (1.0f / 60.0f);
    }
    else if (Keyboard_IsKeyDown(KK_DOWN))
    {
        Position.z -= 0.3f * (1.0f / 60.0f);
    }
    if (Keyboard_IsKeyDown(KK_RIGHT))
    {
        Position.x += 0.3f * (1.0f / 60.0f);
    }
    else if (Keyboard_IsKeyDown(KK_LEFT))
    {
        Position.x -= 0.3f * (1.0f / 60.0f);
    }
    if (Keyboard_IsKeyDown(KK_Z))
    {
        Rotate.x += 60.0f * (1.0f / 60.0f);
    }
    else if (Keyboard_IsKeyDown(KK_X))
    {
        Rotate.x -= 60.0f * (1.0f / 60.0f);
    }

    ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Toon1");
    {
        ImGui::SliderFloat("Level_1", &Parameter.x, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Level_2", &Parameter.y, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Edge", &Parameter.z, 0.0f, -0.6f, "%.2f");
    }
    ImGui::End();
}

//************************************************************
// 描画処理
//************************************************************
void Toon1::Draw(void)
{
    // 頂点レイアウト設定
    GetDeviceContext()->IASetInputLayout(VertexLayout);

    // 頂点シェーダーをセット
    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);

    // ピクセルシェーダーをセット
    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    SetLight(Light);
    SetParameter(Parameter);

    // 3Dポリゴン1つずつの処理
    // テクスチャをセット
    ID3D11ShaderResourceView* tex = GetTexture(TexID);
    GetDeviceContext()->PSSetShaderResources(0, 1, &tex);


    // 平行移動行列作成
    XMMATRIX TranslationMatrix =
        XMMatrixTranslation(
            Position.x,
            Position.y,
            Position.z
        );

    // 回転行列作成
    XMMATRIX RotationMatrix =
        XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(Rotate.x),
            XMConvertToRadians(Rotate.y),
            XMConvertToRadians(Rotate.z)
        );

    // スケーリング行列作成
    XMMATRIX ScalingMatrix =
        XMMatrixScaling(
            Scale.x,
            Scale.y,
            Scale.z
        );

    // ワールド行列作成 ※乗算の順番に注意
    XMMATRIX WorldMatrix =
        ScalingMatrix *
        RotationMatrix *
        TranslationMatrix;

    // DirectXへセット
    SetWorldMatrix(WorldMatrix);

    // プリミティブトポロジーの設定
    GetDeviceContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    // マテリアル設定
    MATERIAL material;
    ZeroMemory(&material, sizeof(MATERIAL));
    material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    // 描画
    ModelDraw(Model);
}