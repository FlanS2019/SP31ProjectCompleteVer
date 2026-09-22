#include "PosterizeNoise.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "keyboard.h"


//************************************************************
// 初期化処理
//************************************************************
HRESULT PosterizeNoise::Init(void)
{
    Time = 0.0f;

    Parameter = XMFLOAT4(0, 0, 0, 0);
    Parameter.x = 6.0f;    // ポスタライズ段階数
    Parameter.y = 0.12f;   // ノイズ強度
    Parameter.z = 2.5f;    // リムライトの強さ
    Parameter.w = 0.0f;    // 経過時間

    TexID = TextureLoad(L"asset\\texture\\sura.jpg");

    //シェーダー読み込み
    CreateVertexShader(&VertexShader, &VertexLayout, "PosterizeNoiseVS.cso");
    CreatePixelShader(&PixelShader, "PosterizeNoisePS.cso");

    //3Dオブジェクト管理構造体の初期化
    Position = XMFLOAT3(1.5f, 0.2f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

    //モデル読み込み
    Model = ModelLoad("asset\\model\\model.fbx");

    XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
    dir = XMVector3Normalize(dir);
    XMStoreFloat4(&Light.Direction, dir);

    Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    Light.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
    Light.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    Light.PointLightParam = XMFLOAT4(3.0f, 0.0f, 0.0f, 1.0f);
    Light.Angle.x = XMConvertToRadians(20.0f);

    return S_OK;
}

//************************************************************
// 終了処理
//************************************************************
void PosterizeNoise::Finalize(void)
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
void PosterizeNoise::Update(void)
{
    //リアルタイムであることが分かるように常時回転させる
    Rotate.y += 45.0f * (1.0f / 60.0f);
    Time += (1.0f / 60.0f);
    Parameter.w = Time;

    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("PosterizeNoise (Kadai05)");
    {
        ImGui::SliderFloat("Levels", &Parameter.x, 2.0f, 12.0f, "%.0f");
        ImGui::SliderFloat("Noise", &Parameter.y, 0.0f, 0.5f, "%.2f");
        ImGui::SliderFloat("RimPower", &Parameter.z, 0.5f, 6.0f, "%.2f");
    }
    ImGui::End();
}

//************************************************************
// 描画処理
//************************************************************
void PosterizeNoise::Draw(void)
{
    // 頂点レイアウト設定
    GetDeviceContext()->IASetInputLayout(VertexLayout);

    // 頂点シェーダーをセット
    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);

    // ピクセルシェーダーをセット
    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    SetLight(Light);
    SetParameter(Parameter);

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
