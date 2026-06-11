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
	CreateVertexShader(&VertexShader, &VertexLayout, "LimLightingVS.cso");//描画がなんかおかしかったような気がするから一時的にこれ使う
	CreatePixelShader(&PixelShader, "LimLightingPS.cso");//これも一時的にこれ使うｗ
	//CreateVertexShader(&VertexShader, &VertexLayout, "PointPixelLightingVS.cso");//描画がなんかおかしかったような気がするから一時的にこれ使う
	//CreatePixelShader(&PixelShader, "PointPixelLightingPS.cso");//これも一時的にこれ使うｗ
	
	//3Dオブジェクト管理構造体の初期化
	Position = XMFLOAT3(-0.1f, 0.2f, 0.0f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	Light.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	Light.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	Light.PointLightParam = XMFLOAT4(3.0f, 6.0f, 0.0f, 0.0f);


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

	// //ImGui でパラメータを調整できるようにする（デバッグ用）
	// //必要ならファイル先頭に #include "imgui.h" を追加してください
	//ImGui::Begin("LimLighting");
	//{
	//	// PointLightParam.x: 範囲（視認用）、PointLightParam.y: リムの鋭さ（exponent）
	//	ImGui::SliderFloat("PointLightParam.x (range)", &Light.PointLightParam.x, 0.5f, 10.0f, "%.2f");
	//	ImGui::SliderFloat("Rim Exponent (PointLightParam.y)", &Light.PointLightParam.y, 0.1f, 20.0f, "%.2f");

	//	// ライト位置を操作（float4 の先頭アドレスを渡す）
	//	ImGui::SliderFloat3("Light.Position", (float*)&Light.Position, -5.0f, 5.0f);

	//	// デバッグ表示
	//	ImGui::Separator();
	//	ImGui::Text("Light.PointLightParam = %.3f, %.3f", Light.PointLightParam.x, Light.PointLightParam.y);
	//	ImGui::Text("Light.Position = %.3f, %.3f, %.3f", Light.Position.x, Light.Position.y, Light.Position.z);
	//}
	//ImGui::End();

	// 安全策：指数は 0 にならないようにクランプ
	if (Light.PointLightParam.y < 0.0001f) Light.PointLightParam.y = 0.0001f;
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
	SetLight(Light);

	//SPOTLIGHT sl;
	//ZeroMemory(&sl, sizeof(sl));
	//sl.Diffuse = XMFLOAT4(1.0f, 0.9f, 0.7f, 1.0f);
	//sl.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//sl.Position = XMFLOAT4(0.0f, 1.5f, 0.0f, 1.0f); // ライト位置
	//sl.Direction = XMFLOAT4(0.0f, -3.0f, 0.0f, 0.0f); // 下向き
	//sl.ConeAngle = 35.0f;
	//sl.Attenuation = 0.1f;
	//sl.Pow = 5.0f;
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

