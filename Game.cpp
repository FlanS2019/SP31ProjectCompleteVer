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
//===============================================
//グローバル変数
 
Camera		CameraObject;
Sprite2D	test2D;

Field3D			Field;
PolygonModel	Model;
VertexDirectionalLighting	VDLModel;
PixelDirectionalLighting	PDLModel;
PixelLightingBlinPhong		PLBPModel;
HemiSpherLight				HSLModel;
PointPixelLighting			PPLModel;
LimLighting					LLModel;

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

	Field.Init();
	Model.Init();
	VDLModel.Init();
	PDLModel.Init();
	PLBPModel.Init();
	HSLModel.Init();
	//PPLModel.Init();
	//LLModel.Init();

	// ライト構造体の初期化
	XMVECTOR dir = XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f);
	dir = XMVector3Normalize(dir);
	XMStoreFloat4(&Light.Direction, dir); // 光のベクトル
	Light.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f); // 光の色
	Light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f); // 環境光

	dir = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat4(&Light.GroundNormal, dir);

	Light.SkyColor = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f); // 赤っぽい
	Light.GroundColor = XMFLOAT4(0.0f, 0.6f, 0.0f, 1.0f); // 緑っぽい

}

//===============================================
//ゲームシーン終了
void FinalizeGame()
{
	FinalizeCamera();
	test2D.Finalize();

	Field.Finalize();
	Model.Finalize();
	VDLModel.Finalize();
	PDLModel.Finalize();
	PLBPModel.Finalize();
	HSLModel.Finalize();

	//PPLModel.Finalize();
	//LLModel.Finalize();
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

		Field.Update();
		Model.Update();
		VDLModel.Update();
		PDLModel.Update();
		PLBPModel.Update();
		HSLModel.Update();
		//PPLModel.Update();
		//LLModel.Update();
	}

}

//===============================================
//ゲームシーン描画
void DrawGame()
{

	// 2D用マトリクス設定
	{
		SetWorldViewProjection2D();
		test2D.Draw();
	}
	//3D用マトリクス設定
	{
		SetDepthEnable(true);		//奥行き処理有効
		DrawCamera();

		Field.Draw();
		Model.Draw();
		VDLModel.Draw();
		PDLModel.Draw();
		PLBPModel.Draw();
		HSLModel.Draw();
		//PPLModel.Draw();
		//LLModel.Draw();
	}
}
