/*==============================================================================

[VertexDirectionalLighting.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "LimLighting.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"
#include "keyboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
//カメラは複数あるとダメなので外す

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT LimLighting::Init(void)
{
	//シェーダー読み込み
	CreateVertexShader(&VertexShader, &VertexLayout, "PointPixelLightingVS.cso");//描画がなんかおかしかったような気がするから一時的にこれ使う
	CreatePixelShader(&PixelShader, "PointPixelLightingPS.cso");//これも一時的にこれ使うｗ

	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	Light.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	Light.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	Light.PointLightParam = XMFLOAT4(3.0f, 0.0f, 0.0f, 0.0f);

	//3Dオブジェクト管理構造体の初期化
	Position = XMFLOAT3(-0.1f, 0.2f, 0.0f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

	//モデル読み込み
	Model = ModelLoad("asset\\model\\model.fbx");
	//Model = ModelLoad("asset\\model\\cube.fbx");

	//ライト構造体の初期化
	XMVECTOR	dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
	dir = XMVector3Normalize(dir);
	XMStoreFloat4(&Light.Direction, dir);//光のベクトル
	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);//光の色
	Light.Ambient = XMFLOAT4(0.5f, 0.3f, 0.3f, 1.0f);//環境光の色

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void LimLighting::Finalize(void)
{
	//作ったものを解放

	VertexLayout->Release();
	VertexShader->Release();
	PixelShader->Release();


	ModelRelease(Model);

}

//=============================================================================
// 更新処理
//=============================================================================
void LimLighting::Update(void)
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

	ImGui::Begin("LimLighting");
	{
		ImGui::SliderFloat("PointLightParam.x",
			&Light.PointLightParam.x, 0.5f, 5.0f, "%.2f");

		ImGui::SliderFloat("Position.x",
			&Light.Position.x, -2.0f, 2.0f, "%.2f");
		ImGui::SliderFloat("Position.y",
			&Light.Position.y, -2.0f, 2.0f, "%.2f");
		ImGui::SliderFloat("Position.z",
			&Light.Position.z, -2.0f, 2.0f, "%.2f");

		// デバッグ表示: スライダーで変化しているかを可視化
		ImGui::Separator();
		ImGui::Text("Light.PointLightParam.x = %.3f", Light.PointLightParam.x);
		ImGui::Text("Light.Position = %.3f, %.3f, %.3f", Light.Position.x, Light.Position.y, Light.Position.z);
	}
	ImGui::End();

}

//=============================================================================
// 描画処理
//=============================================================================
void LimLighting::Draw(void)
{
	// 頂点レイアウト設定
	GetDeviceContext()->IASetInputLayout(VertexLayout);
	//頂点シェーダーをセット
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	//ピクセルシェーダーをセット
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	SetLight(Light);


	{//3Dポリゴン１つずつの処理
		//テクスチャをセット
		ID3D11ShaderResourceView* tex = GetTexture(TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//平行移動行列作成
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(
				Position.x,
				Position.y,
				Position.z
			);
		//回転行列作成
		XMMATRIX	RotationMatrix =
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(Rotate.x),
				XMConvertToRadians(Rotate.y),
				XMConvertToRadians(Rotate.z)
			);
		//スケーリング行列作成
		XMMATRIX	ScalingMatrix =
			XMMatrixScaling(
				Scale.x,
				Scale.y,
				Scale.z
			);
		//ワールド行列作成 ※乗算の順番に注意
		XMMATRIX	WorldMatrix =
			ScalingMatrix *
			RotationMatrix *
			TranslationMatrix;
		//DirectXへセット
		SetWorldMatrix(WorldMatrix);

		//プリミティブトポロジーの設定
		GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);
		//マテリアル設定
		MATERIAL	material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		//描画
		ModelDraw(Model);
	}


}
