#include "Header.h"


HRESULT HemiSphereLighting::Init(void)
{
    CreateVertexShader(&VertexShader, &VertexLayout, "HemiSphereLightingVS.cso");
    CreatePixelShader(&PixelShader, "HemiSphereLightingPS.cso");

    Position = XMFLOAT3(0.0f + (0.5f * 4.0f), 0.2f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
    Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

    Model = ModelLoad("asset\\model\\model.fbx");


    XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
    dir = XMVector3Normalize(dir);
    XMStoreFloat4(&Light.Direction, dir);

    dir = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    XMStoreFloat4(&Light.GroundNormal, dir);

    Light.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    Light.Ambient = XMFLOAT4(0.5f, 0.3f, 0.3f, 1.0f);

    Light.SkyColor = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f);
    Light.GroundColor = XMFLOAT4(0.0f, 0.6f, 0.0f, 1.0f);

    return S_OK;
}

void HemiSphereLighting::Finalize(void)
{
    //��������̂����
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();

    ModelRelease(Model);
}

void HemiSphereLighting::Update(void)
{
    //�K���ɉ�]
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

void HemiSphereLighting::Draw(void)
{
    GetDeviceContext()->IASetInputLayout(VertexLayout);

    GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);

    GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

    SetLight(Light);

    // 3D�|���S��1���̏���
    // �e�N�X�`�����Z�b�g
    ID3D11ShaderResourceView* tex = GetTexture(TexID);
    GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

    // ���s�ړ��s��쐬
    XMMATRIX TranslationMatrix =
        XMMatrixTranslation(
            Position.x,
            Position.y,
            Position.z
        );

    // ��]�s��쐬
    XMMATRIX RotationMatrix =
        XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(Rotate.x),
            XMConvertToRadians(Rotate.y),
            XMConvertToRadians(Rotate.z)
        );

    // �X�P�[�����O�s��쐬
    XMMATRIX ScalingMatrix =
        XMMatrixScaling(
            Scale.x,
            Scale.y,
            Scale.z
        );

    // ���[���h�s��쐬 ����Z�̏��Ԃɒ���
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