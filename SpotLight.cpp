#include "SpotLight.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"
#include "keyboard.h"

HRESULT SpotLight::Init(void)
{
    CreateVertexShader(&VertexShader, &VertexLayout, "SpotLightVS.cso");
    CreatePixelShader(&PixelShader, "SpotLightPS.cso");

    Position = XMFLOAT3(0.0f + (0.5f * 0.0f), 0.2f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);

    Model = ModelLoad("asset\\model\\model.fbx");

    return S_OK;
}

void SpotLight::Finalize(void)
{
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();

    ModelRelease(Model);
}

void SpotLight::Update(void)
{
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
}

void SpotLight::Draw(void)
{
    GetDeviceContext()->IASetInputLayout(VertexLayout);

    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);

    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    ID3D11ShaderResourceView* tex = GetTexture(TexID);
    GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

    XMMATRIX TranslationMatrix =
        XMMatrixTranslation(
            Position.x,
            Position.y,
            Position.z
        );

    XMMATRIX RotationMatrix =
        XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(Rotate.x),
            XMConvertToRadians(Rotate.y),
            XMConvertToRadians(Rotate.z)
        );

    XMMATRIX ScalingMatrix =
        XMMatrixScaling(
            Scale.x,
            Scale.y,
            Scale.z
        );

    XMMATRIX WorldMatrix =
        ScalingMatrix *
        RotationMatrix *
        TranslationMatrix;

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