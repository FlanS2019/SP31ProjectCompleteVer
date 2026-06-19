#include "DesneyPBR.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"
#include "keyboard.h"

HRESULT DesneyPBR::Init(void)
{
    CreateVertexShader(&VertexShader, &VertexLayout, "CookTorranceVS.cso");
    CreatePixelShader(&PixelShader, "CookTorrancePS.cso");

    Position = XMFLOAT3(0.0f, 0.5f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

    // ライト構造体の初期化
    XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
    dir = XMVector3Normalize(dir);
    XMStoreFloat4(&Light.Direction, dir); // 光のベクトル
    Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f); // 光の位置
    Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); // 光の色
    Light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f); // 環境光
    Light.PointLightParam = XMFLOAT4(10.0f, 1.0f, 0.0f, 0.0f); // 距離減衰のパラメータ
    Light.Angle.x = XMConvertToRadians(20.0f); // コーンの角度

	Parameter = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	Parameter.x = 0.5f; // Roughness
	Parameter.y = 0.8f; // Metallic

    Model = ModelLoad("asset\\model\\model.fbx");

    return S_OK;
}

void DesneyPBR::Finalize(void)
{
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();

    ModelRelease(Model);
}

void DesneyPBR::Update(void)
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
	ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("TEST");
    {
		ImGui::SliderFloat("Roughness", &Parameter.x, 0.0f, 1.0f, "%.2f");

		ImGui::SliderFloat("Metallic", &Parameter.y, 0.0f, 1.0f, "%.2f");
    }
    ImGui::End();

}

void DesneyPBR::Draw(void)
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
	SetParameter(Parameter);
    SetLight(Light);
    ModelDraw(Model);
}