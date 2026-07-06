/*==============================================================================

[TexCube.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "Header.h"
#include "model.h"

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT TexCube::Init(void)
{
	//シェーダー読み込み(ライティングなし、単純テクスチャ描画用)
	CreateVertexShader(&VertexShader, &VertexLayout, "TexCubeVS.cso");
	CreatePixelShader(&PixelShader, "TexCubePS.cso");

	//3Dオブジェクト管理構造体の初期化
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

	//モデル読み込み
	Model = ModelLoad("asset\\model\\cube.fbx");

	//テクスチャ読み込み ← ここ、実際の関数名/引数に合わせて直してください
	TexID = TextureLoad(L"asset\\texture\\Roughness.png");

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void TexCube::Finalize(void)
{
	VertexLayout->Release();
	VertexShader->Release();
	PixelShader->Release();

	ModelRelease(Model);
}

//=============================================================================
// 更新処理
//=============================================================================
void TexCube::Update(void)
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

//=============================================================================
// 描画処理
//=============================================================================
void TexCube::Draw(void)
{
	GetDeviceContext()->IASetInputLayout(VertexLayout);
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	{
		//テクスチャをセット
		ID3D11ShaderResourceView* tex = GetTexture(TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//平行移動行列作成
		XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
		//回転行列作成
		XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(Rotate.x),
			XMConvertToRadians(Rotate.y),
			XMConvertToRadians(Rotate.z)
		);
		//スケーリング行列作成
		XMMATRIX ScalingMatrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
		//ワールド行列作成
		XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;
		SetWorldMatrix(WorldMatrix);

		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//マテリアル設定(ライティングなしなのでDiffuse=白でOK)
		MATERIAL material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		//描画
		ModelDraw(Model);
	}
}