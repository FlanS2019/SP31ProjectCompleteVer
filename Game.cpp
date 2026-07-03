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

#include "SpotLight.h"

#include "BumpField.h"
#include "CookTorrance.h"
#include "DisnayPBR.h"

#include "IGameObject3D.h"
//===============================================
//グローバル変数

std::vector<IGameObject3D*> g_Models3D;
 
Camera		CameraObject;
Sprite2D	test2D;

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

template <class T>
T* AddModel3D()
{
	T* obj = new T();
	g_Models3D.push_back(obj);
	obj->Init();
	return obj;
}

//===============================================
//ゲームシーン初期化
void InitGame()
{
	TextureInitialize(GetDevice());
	InitCamera();

	test2D.Init();

	
	AddModel3D<BumpField>();
	AddModel3D<CookTorrance>();
	AddModel3D<DisnayPBR>();	
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
}

void FinalizeGame()
{
	FinalizeCamera();
	test2D.Finalize();

	for (auto* model : g_Models3D)
	{
		model->Finalize();
		delete model;
	}
	g_Models3D.clear();

	TextureFinalize();
}

void UpdateGame()
{
	if (GetPause() == false)
	{
		UpdateCamera();
		test2D.Update();

		for (auto* model : g_Models3D)
		{
			model->Update();
		}
	}
}

void DrawGame()
{
	SetDepthEnable(true);
	DrawCamera();

	SetLight(Light);

	for (auto* model : g_Models3D)
	{
		model->Draw();
	}
}