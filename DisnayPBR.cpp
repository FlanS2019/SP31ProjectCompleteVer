#include "DisnayPBR.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"
#include "keyboard.h"

HRESULT DisnayPBR::Init(void)
{
	//FILE* fp = fopen("asset\\model\\Lowpoly_tree_sample.fbx", "r");

    Position = XMFLOAT3(0.0f, 0.5f, 0.0f);
    Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

    CreatePixelShader(
        &PixelShader,
        "DesneyPBRPS.cso");

    Position = XMFLOAT3(
        0.0f,
        0.5f,
        0.0f);

    Rotate = XMFLOAT3(
        0.0f,
        0.0f,
        0.0f);

    Scale = XMFLOAT3(
        0.2f,
        0.2f,
        0.2f);

    //-------------------
    // Light初期化
    //-------------------

    XMVECTOR dir =
        XMVectorSet(
            0.0f,
            -1.0f,
            0.0f,
            0.0f);

    dir = XMVector3Normalize(dir);

	Parameter = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	Parameter.x = 0.5f; // Roughness
	Parameter.y = 0.8f; // Metallic
	Parameter.z = 3.0f; // AO

    Model = ModelLoad("asset\\model\\model.fbx");

    TexIDRoughness = TextureLoad(L"asset\\texture\\Roughness.png");
    TexIDMetalness = TextureLoad(L"asset\\texture\\Metalness.png");


    Light.PointLightParam =
        XMFLOAT4(
            2000.0f,
            1.0f,
            0.0f,
            0.0f);

    Light.Angle.x =
        XMConvertToRadians(
            20.0f);

    //-------------------
    // PBRパラメータ
    //-------------------

    Parameter =
        XMFLOAT4(
            0.5f, // Roughness
            0.8f, // Metallic
            3.0f, // LightPower
            0.0f);

    //-------------------
    // モデル
    //-------------------

    Model =
        ModelLoad(
            "asset\\model\\Lowpoly_tree_sample.fbx");

    //if (fp == NULL)
    //{
    //    MessageBoxA(
    //        NULL,
    //        "FBXファイル見つからない",
    //        "Error",
    //        MB_OK);
    //}
    //else
    //{
    //    fclose(fp);

    //    MessageBoxA(
    //        NULL,
    //        "FBX発見",
    //        "OK",
    //        MB_OK);
    //}

    //{
    //    MessageBox(
    //        NULL,
    //        "モデル読み込み失敗",
    //        "Error",
    //        MB_OK);
    //}

    //-------------------
    // テクスチャ
    //-------------------

    //TexIDRoughness =
    //    TextureLoad(
    //        L"asset\\texture\\Roughness.png");

    //TexIDMetalness =
    //    TextureLoad(
    //        L"asset\\texture\\Metalness.png");

    return S_OK;
}

void DisnayPBR::Finalize(void)
{
    VertexLayout->Release();
    VertexShader->Release();
    PixelShader->Release();

    ModelRelease(Model);
}

void DisnayPBR::Update(void)
{
    //-------------------
    // 移動
    //-------------------

    if (Keyboard_IsKeyDown(KK_UP))
        Position.z += 0.3f / 60.0f;

    if (Keyboard_IsKeyDown(KK_DOWN))
        Position.z -= 0.3f / 60.0f;

    if (Keyboard_IsKeyDown(KK_RIGHT))
        Position.x += 0.3f / 60.0f;

    if (Keyboard_IsKeyDown(KK_LEFT))
        Position.x -= 0.3f / 60.0f;

    if (Keyboard_IsKeyDown(KK_Z))
    {
        Rotate.x += 60.0f * (1.0f / 60.0f);
    }
    else if (Keyboard_IsKeyDown(KK_X))
    {
        Rotate.x -= 60.0f * (1.0f / 60.0f);
    }
	ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Desney PBR");
    {
		ImGui::SliderFloat("Light", &Parameter.z, 1.0f, 15.0f, "%.0f");
    }
	ImGui::End();
}

void DisnayPBR::Draw(void)
{
    GetDeviceContext()->IASetInputLayout(VertexLayout);

    GetDeviceContext()->VSSetShader(
        VertexShader,
        NULL,
        0);

    GetDeviceContext()->PSSetShader(
        PixelShader,
        NULL,
        0);

    //-------------------
    // PBRテクスチャ設定
    //-------------------

    //ID3D11ShaderResourceView* tex;

    //tex = GetTexture(TexIDRoughness);

    //GetDeviceContext()->PSSetShaderResources(
    //    1,
    //    1,
    //    &tex);

    //tex = GetTexture(TexIDMetalness);

    //GetDeviceContext()->PSSetShaderResources(
    //    2,
    //    1,
    //    &tex);

    //-------------------
    // 超重要追加
    //-------------------

    GetDeviceContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    XMMATRIX world =
        XMMatrixScaling(
            0.02f,      // ←ここ変更
            0.02f,
            0.02f)

        *

        XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(-90.0f), // ←追加
            0,
            0)

        *

        XMMatrixTranslation(
            0.0f,
            0.0f,
            3.0f);

    SetWorldMatrix(world);

    MATERIAL material = {};
    material.Diffuse =
        XMFLOAT4(1, 1, 1, 1);

    SetMaterial(material);

    SetParameter(Parameter);
    SetLight(Light);

    ModelDraw(Model);
}