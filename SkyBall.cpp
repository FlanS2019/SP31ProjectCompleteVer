#include "SkyBall.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"

HRESULT SkyBall::Init(void)
{
    CreateVertexShader(&VertexShader, &VertexLayout, "SkyVS.cso");
    CreatePixelShader(&PixelShader, "SkyPS.cso");

    Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Scale = XMFLOAT3(500.0f, 500.0f, 500.0f);

    // sky.objをそのままmodelフォルダに入れた想定
    Model = ModelLoad("asset\\model\\sky.obj");
    TexIDSky = TextureLoad(L"asset\\texture\\sky.png"); // 拡張子は実際のファイルに合わせて変更

    return S_OK;
}

void SkyBall::Finalize(void)
{
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();
    ModelRelease(Model);
}

void SkyBall::Update(void)
{
    // カメラに常に追従させる（スカイボールが置いてきぼりにならないように）
    XMFLOAT3 camPos = GetCameraPosition();
    Position = camPos;
}

void SkyBall::Draw(void)
{
    GetDeviceContext()->IASetInputLayout(VertexLayout);
    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    ID3D11ShaderResourceView* tex = GetTexture(TexIDSky);
    GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

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

    GetDeviceContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    MATERIAL material;
    ZeroMemory(&material, sizeof(MATERIAL));
    material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    ModelDraw(Model);
}