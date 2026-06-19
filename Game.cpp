#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Camera.h"
#include "texture.h"

#include	"Sprite2D.h"

#include	"Field3D.h"
#include	"PolygonModel.h"
#include	"VertexDirectionalLighting.h"
#include	"PixelDirectionalLighting.h"

#include "PixelLightingBlinPhong.h"
#include "HemiSpherLight.h"

#include	"PointPixelLighting.h"
#include	"LimLighting.h"

#include "Cube.h"
#include "SpotLight.h"

#include "BumpField.h"
#include "CookTorrance.h"
#include "DesneyPBR.h"
//===============================================
//グローバル変数
 
Camera		CameraObject;
Sprite2D	test2D;

Field3D	Field;
PolygonModel	Model;
HemiSpherLight	HSLModel;
PointPixelLighting	PPLModel;
LimLighting	LLModel;
BumpField	BumpField;
SpotLight	SLModel;
CookTorrance	CTModel;
DesneyPBR	DPModel;

//Cube cube;
//VertexDirectionalLighting	VDLModel;
//PixelDirectionalLighting	PDLModel;
//PixelLightingBlinPhong		PLBPModel;


// グローバルライト構造体の追加
LIGHT Light;

//ポーズフラグ
static	bool	pause = false;

//===============================================
//ポーズフラグセット
void	SetPause(bool flg)
{
	pause = flg;
}
//===============================================
//ポーズフラグ取得
bool	GetPause()
{
	return pause;
}

//===============================================
//ゲームシーン初期化
void InitGame()
{
	TextureInitialize(GetDevice());
	InitCamera();

	test2D.Init();

	
	BumpField.Init();
	CTModel.Init();
	DPModel.Init();
	//Field.Init();
	//Model.Init();
	//LLModel.Init();
	//PPLModel.Init();
	//SLModel.Init();
	//VDLModel.Init();
	//PDLModel.Init();
	//PLBPModel.Init();
	//cube.Init();
	//HSLModel.Init();
	
	//// ライト構造体の初期化
	//XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	//dir = XMVector3Normalize(dir);
	//XMStoreFloat4(&Light.Direction, dir); // 光のベクトル
	//Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f); // 光の位置
	//Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); // 光の色
	//Light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f); // 環境光
	//Light.PointLightParam = XMFLOAT4(10.0f, 1.0f, 0.0f, 0.0f); // 距離減衰のパラメータ
	//Light.Angle.x = XMConvertToRadians(30.0f); // コーンの角度

	//dir = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	//XMStoreFloat4(&Light.GroundNormal, dir);

	//Light.SkyColor = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f); // 赤っぽい
	//Light.GroundColor = XMFLOAT4(0.0f, 0.6f, 0.0f, 1.0f); // 緑っぽい

}

//===============================================
//ゲームシーン終了
void FinalizeGame()
{
	FinalizeCamera();
	test2D.Finalize();
	BumpField.Finalize();
	CTModel.Finalize();
	DPModel.Finalize();
	//Field.Finalize();
	////Model.Finalize();
	//LLModel.Finalize();

	//PPLModel.Finalize();
	//SLModel.Finalize();
	//VDLModel.Finalize();
	//PDLModel.Finalize();
	//PLBPModel.Finalize();
	//cube.Finalize();
	//HSLModel.Finalize();
	TextureFinalize();
}

//===============================================
//ゲームシーン更新
void UpdateGame()
{

	if (GetPause() == false)//ポーズ中でなければ更新実行
	{
		UpdateCamera();
		test2D.Update();
		BumpField.Update();
		CTModel.Update();
		DPModel.Update();
		//Field.Update();
		//Model.Update();
		////LLModel.Update();
		//PPLModel.Update();
		//SLModel.Update();
		//VDLModel.Update();
		//PDLModel.Update();
		//PLBPModel.Update();
		//cube.Update();
		//HSLModel.Update();
	}

	//ImGui::Begin("SPOT LIGHT");
	//{
	//	ImGui::SliderFloat("Diffuse-R", &Light.Diffuse.x, 0.0f, 1.0f, "%.1f");
	//	ImGui::SliderFloat("Diffuse-G", &Light.Diffuse.y, 0.0f, 1.0f, "%.1f");
	//	ImGui::SliderFloat("Diffuse-B", &Light.Diffuse.z, 0.0f, 1.0f, "%.1f");

	//	float angle = XMConvertToDegrees(Light.Angle.x); // 度に変換
	//	ImGui::SliderFloat("Cone Angle", &angle, 5.0f, 45.0f, "%.1f");
	//	Light.Angle.x = XMConvertToRadians(angle); // ラジアンに変換

	//	ImGui::SliderFloat("Attenuation", &Light.PointLightParam.x, 0.0f, 10.0f, "%.1f"); // 距離
	//	ImGui::SliderFloat("Pow", &Light.PointLightParam.y, 1.0f, 50.0f, "%0.2f"); // 調整用

	//	ImGui::SliderFloat("Position.x",
	//		&Light.Position.x, -2.0f, 2.0f, "%.1f");

	//	ImGui::SliderFloat("Position.y",
	//		&Light.Position.y, -2.0f, 2.0f, "%.1f");

	//	ImGui::SliderFloat("Position.z",
	//		&Light.Position.z, -2.0f, 2.0f, "%.1f");
	//}
	//ImGui::End();
}

//===============================================
//ゲームシーン描画
void DrawGame()
{

	
	SetDepthEnable(true);		//奥行き処理有効
	DrawCamera();

	{//全体ライト表示
		SetLight(Light);
		//Field.Draw();
		//SLModel.Draw();
	}

	//3D用マトリクス設定//個別ライト表示
	{
		BumpField.Draw();
		CTModel.Draw();
		DPModel.Draw();
		//LLModel.Draw();
		////Model.Draw();
		//PPLModel.Draw();
		//VDLModel.Draw();
		//PDLModel.Draw();
		//PLBPModel.Draw();
		//cube.Draw();
		//HSLModel.Draw();
	}
	//// 2D用マトリクス設定
	//{
	//	SetWorldViewProjection2D();
	//	test2D.Draw();
	//}
}