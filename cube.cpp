/*==============================================================================
   [Cube.cpp]
==============================================================================*/
#include "Cube.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "keyboard.h"

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Cube::Init(void)
{
	// シェーダーの読み込み（コメントアウトを解除）
	CreateVertexShader(&VertexShader, &VertexLayout, "CubeVS.cso");
	CreatePixelShader(&PixelShader, "CubePS.cso");

	// 3Dオブジェクトのトランスフォーム初期化
	Position = XMFLOAT3(0.2f, 0.0f, 0.7f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f); // ← (0,0,0)だと消えるので修正

	// モデルの読み込み
	Model = ModelLoad("asset\\model\\cube.fbx");

	// ライト構造体の初期化
	XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
	dir = XMVector3Normalize(dir);
	XMStoreFloat4(&Light.Direction, dir);
	Light.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	Light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	dir = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat4(&Light.GroundNormal, dir);
	Light.SkyColor = XMFLOAT4(0.4f, 0.6f, 0.2f, 1.0f);
	Light.GroundColor = XMFLOAT4(0.1f, 0.3f, 0.1f, 1.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Cube::Finalize(void)
{
	if (VertexShader) { VertexShader->Release(); VertexShader = nullptr; }
	if (PixelShader) { PixelShader->Release();  PixelShader = nullptr; }
	if (VertexLayout) { VertexLayout->Release();  VertexLayout = nullptr; }
	ModelRelease(Model);
}

//=============================================================================
// 更新処理
//=============================================================================
void Cube::Update(void)
{
	if (Keyboard_IsKeyDown(KK_UP))
		Position.z += 0.3f * (1.0f / 60.0f);
	else if (Keyboard_IsKeyDown(KK_DOWN))
		Position.z -= 0.3f * (1.0f / 60.0f);

	if (Keyboard_IsKeyDown(KK_LEFT))
		Position.x -= 0.3f * (1.0f / 60.0f);
	else if (Keyboard_IsKeyDown(KK_RIGHT))
		Position.x += 0.3f * (1.0f / 60.0f);

	if (Keyboard_IsKeyDown(KK_Z))
		Rotate.x += 60.0f * (1.0f / 60.0f);
	else if (Keyboard_IsKeyDown(KK_X))
		Rotate.x -= 60.0f * (1.0f / 60.0f);
}

//=============================================================================
// 描画処理
//=============================================================================
void Cube::Draw(void)
{
	GetDeviceContext()->IASetInputLayout(VertexLayout);
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	SetLight(Light);

	{
		ID3D11ShaderResourceView* tex = GetTexture(TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
		XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(Rotate.x),
			XMConvertToRadians(Rotate.y),
			XMConvertToRadians(Rotate.z));
		XMMATRIX ScalingMatrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
		XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

		SetWorldMatrix(WorldMatrix);

		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		MATERIAL material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		ModelDraw(Model);
	}
}