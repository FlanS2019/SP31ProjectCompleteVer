#include "Header.h"

#define FIELD_SIZE (5)

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
	AddModel3D<SkyBall>();

	Wall3D* wall1 = AddModel3D<Wall3D>();
	wall1->Position = XMFLOAT3(0.0f, 0.0f, FIELD_SIZE / 2.5f);
	wall1->Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Wall3D* wall2 = AddModel3D<Wall3D>();
	wall2->Position = XMFLOAT3(-FIELD_SIZE / 2.5f, 0.0f, 0.0f); // 左側の壁
	wall2->Rotate = XMFLOAT3(0.0f, -90.0f, 0.0f); // 90度回転して横向きに

	AddModel3D<LimLighting>();
	AddModel3D<PointPixelLighting>();
	AddModel3D<SpotLight>();
	AddModel3D<VertexDirectionalLighting>();
	AddModel3D<PixelDirectionalLighting>();
	AddModel3D<PixelLightingBlinPhong>();
	AddModel3D<HemiSpherLight>();
	AddModel3D<TexCube>();

	XMVECTOR    dir = XMVector4Normalize(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
	XMStoreFloat4(&Light.Direction, dir);
	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Light.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	Light.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);//少し大きめに
	Light.PointLightParam = XMFLOAT4(2000.0f, 1.5f, 0.0f, 0.0f);//減衰率
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