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
    TexIDSky = TextureLoad(L"asset\\texture\\sky.png");

    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_NONE; // 内側から見るのでカリング無効
    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    GetDevice()->CreateRasterizerState(&rd, &RasterizerState);

    D3D11_SAMPLER_DESC sd = {};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sd.MinLOD = 0;
    sd.MaxLOD = D3D11_FLOAT32_MAX;
    GetDevice()->CreateSamplerState(&sd, &SamplerState);

    return S_OK;
}

void SkyBall::Finalize(void)
{
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();
    RasterizerState->Release();
    ModelRelease(Model);
    SamplerState->Release();
}

void SkyBall::Update(void)
{
    // カメラに常に追従させる（スカイボールが置いてきぼりにならないように）
    XMFLOAT3 camPos = GetCamera()->Position;
    Position = camPos;
}

void SkyBall::Draw(void)
{
    GetDeviceContext()->RSSetState(RasterizerState);
    GetDeviceContext()->IASetInputLayout(VertexLayout);
    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    ID3D11ShaderResourceView* tex = GetTexture(TexIDSky);
    GetDeviceContext()->PSSetShaderResources(0, 1, &tex);
    GetDeviceContext()->PSSetSamplers(0, 1, &SamplerState);

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

    // 他のオブジェクトに影響しないよう元の状態に戻す
    ID3D11SamplerState* defaultSampler = GetDefaultSamplerState();
    GetDeviceContext()->PSSetSamplers(0, 1, &defaultSampler);
    GetDeviceContext()->RSSetState(GetDefaultRasterizerState());

    ModelDraw(Model);
}