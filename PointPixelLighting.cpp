/*==============================================================================

[VertexDirectionalLighting.cpp]
                                                         Author :
                                                         Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "PointPixelLighting.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"
#include "keyboard.h"
#include <windows.h> // OutputDebugString 用

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
HRESULT PointPixelLighting::Init(void)
{
	//シェーダー読み込み
	CreateVertexShader(&VertexShader, &VertexLayout, "PointPixelLightingVS.cso");
	CreatePixelShader(&PixelShader, "PointPixelLightingPS.cso");

	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	Light.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	Light.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	Light.PointLightParam = XMFLOAT4(3.0f, 0.0f, 0.0f, 0.0f);

	//3Dオブジェクト管理構造体の初期化
	Position = XMFLOAT3(0.4f, 0.2f, 0.0f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

	//モデル読み込み
	Model = ModelLoad("asset\\model\\model.fbx");
	//Model = ModelLoad("asset\\model\\cube.fbx");

	//ライト構造体の初期化
	XMVECTOR	dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
	dir = XMVector3Normalize(dir);
	XMStoreFloat4(&Light.Direction, dir);//光のベクトル

	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);//光の位置
	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);//光の色
	Light.Ambient = XMFLOAT4(0.5f, 0.3f, 0.3f, 1.0f);//環境光の色
	Light.PointLightParam = XMFLOAT4(3.0f, 0.0f, 0.0f, 0.0f);//点光源のパラメータ（xに減衰係数）

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void PointPixelLighting::Finalize(void)
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
void PointPixelLighting::Update(void)
{
	// デバッグ出力: Update が呼ばれているか確認する
	OutputDebugStringA("PointPixelLighting::Update() called\n");
	{
		char buf[128];
		snprintf(buf, sizeof(buf), "Light.Position = %.3f, %.3f, %.3f\n", Light.Position.x, Light.Position.y, Light.Position.z);
		OutputDebugStringA(buf);
	}

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

	//// Begin の戻り値をチェックする形式に変更（描画可否の確認）
	//if (ImGui::Begin("PointPixelLighting"))
	//{
	//	ImGui::Text("Debug ImGui active"); // シンプル表示でウィンドウの有無を確かめる
	//	ImGui::SliderFloat("PointLightParam.x",
	//		&Light.PointLightParam.x, 0.5f, 5.0f, "%.2f");

	//	ImGui::SliderFloat("Position.x",
	//		&Light.Position.x, -2.0f, 2.0f, "%.2f");
	//	ImGui::SliderFloat("Position.y",
	//		&Light.Position.y, -2.0f, 2.0f, "%.2f");
	//	ImGui::SliderFloat("Position.z",
	//		&Light.Position.z, -2.0f, 2.0f, "%.2f");
	//	// デバッグ表示: スライダーで変化しているかを可視化
	//	ImGui::Separator();
	//	ImGui::Text("Light.PointLightParam.x = %.3f", Light.PointLightParam.x);
	//	ImGui::Text("Light.Position = %.3f, %.3f, %.3f", Light.Position.x, Light.Position.y, Light.Position.z);
	//}
	//ImGui::End();

}

//=============================================================================
// 描画処理
//=============================================================================
void PointPixelLighting::Draw(void)
{
	// 頂点レイアウト設定
	GetDeviceContext()->IASetInputLayout(VertexLayout);
	//頂点シェーダーをセット
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	//ピクセルシェーダーをセット
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	SetLight(Light);

	//SPOTLIGHT sl;
	//ZeroMemory(&sl, sizeof(sl));
	//sl.Diffuse = XMFLOAT4(1.0f, 0.9f, 0.7f, 1.0f);
	//sl.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//sl.Position = XMFLOAT4(0.0f, 1.5f, 0.0f, 1.0f); // ライト位置
	//sl.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f); // 下向き
	//sl.ConeAngle = 25.0f;
	//sl.Attenuation = 0.3f;
	//sl.Pow = 9.5f;
	//SetSpotLight(sl);

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
