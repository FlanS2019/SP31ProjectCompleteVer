#include "Header.h"
#define FIELD_SIZE (5)

//グローバル変数
std::vector<IGameObject3D*> g_Models3D;
 
Camera		CameraObject;
Sprite2D	test2D;

// グローバルライト構造体の追加
LIGHT Light;

//ポーズフラグ
static	bool	pause = false;

//ポーズフラグセット
void	SetPause(bool flg)
{
	pause = flg;
}
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

//ゲームシーン初期化
void InitGame()
{
	TextureInitialize(GetDevice());
	InitCamera();
	test2D.Init();

	AddModel3D<BumpField>();
	AddModel3D<SkyBall>();


	Wall3D* wall1 = AddModel3D<Wall3D>();
	wall1->Position = XMFLOAT3(0.0f, 0.0f, FIELD_SIZE / 2.5f);
	wall1->Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Wall3D* wall2 = AddModel3D<Wall3D>();
	wall2->Position = XMFLOAT3(-FIELD_SIZE / 2.5f, 0.0f, 0.0f);
	wall2->Rotate = XMFLOAT3(0.0f, -90.0f, 0.0f);

	// Cube + モデル のペアを2列（4個×2列）に並べる
		// SpotLightは除外
	const float CUBE_TOP_Y = 0.5f;   // ★モデルが浮く/めり込む場合はここを調整
	const float SPACING_X = 0.6f;   // 横方向の間隔
	const float SPACING_Z = 1.0f;   // 奥行き方向の間隔（列と列の間）
	const float MODEL_Z_OFFSET = 0.15f;

	// 1列目（Z = SPACING_Z）※旧2列目の内容
	float posX = -SPACING_X * 1.0f; // 元々2列目に使っていた中央寄せ値
	float rowZ = SPACING_Z;

	TexCube* cube5 = AddModel3D<TexCube>();
	cube5->Position = XMFLOAT3(posX, 0.0f, rowZ);
	PixelLightingBlinPhong* blin = AddModel3D<PixelLightingBlinPhong>();
	blin->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ);
	posX += SPACING_X;

	TexCube* cube6 = AddModel3D<TexCube>();
	cube6->Position = XMFLOAT3(posX, 0.0f, rowZ);
	HemiSphereLighting* hemi = AddModel3D<HemiSphereLighting>();
	hemi->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ);
	posX += SPACING_X;

	TexCube* cube7 = AddModel3D<TexCube>();
	cube7->Position = XMFLOAT3(posX, 0.0f, rowZ);
	DisnayPBR* pbr = AddModel3D<DisnayPBR>();
	pbr->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ + MODEL_Z_OFFSET);
	posX += SPACING_X;

	TexCube* cube8 = AddModel3D<TexCube>();
	cube8->Position = XMFLOAT3(posX, 0.0f, rowZ);
	CookTorrance* ct = AddModel3D<CookTorrance>();
	ct->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ + MODEL_Z_OFFSET);

	// 2列目（Z = 0.0）※旧1列目の内容
	posX = -SPACING_X * 1.5f; // 元々1列目に使っていた中央寄せ値
	rowZ = 0.0f;

	TexCube* cube1 = AddModel3D<TexCube>();
	cube1->Position = XMFLOAT3(posX, 0.0f, rowZ);
	LimLighting* lim = AddModel3D<LimLighting>();
	lim->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ);
	posX += SPACING_X;

	TexCube* cube2 = AddModel3D<TexCube>();
	cube2->Position = XMFLOAT3(posX, 0.0f, rowZ);
	PointPixelLighting* ppl = AddModel3D<PointPixelLighting>();
	ppl->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ);
	posX += SPACING_X;

	TexCube* cube3 = AddModel3D<TexCube>();
	cube3->Position = XMFLOAT3(posX, 0.0f, rowZ);
	VertexDirectionalLighting* vdl = AddModel3D<VertexDirectionalLighting>();
	vdl->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ);
	posX += SPACING_X;

	TexCube* cube4 = AddModel3D<TexCube>();
	cube4->Position = XMFLOAT3(posX, 0.0f, rowZ);
	PixelDirectionalLighting* pdl = AddModel3D<PixelDirectionalLighting>();
	pdl->Position = XMFLOAT3(posX, CUBE_TOP_Y, rowZ);

	//Smoke* smoke = AddModel3D<Smoke>();
	//smoke->Position = XMFLOAT3(0.0f, 0.5f, 0.0f);

	XMVECTOR dir = XMVector4Normalize(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
	XMStoreFloat4(&Light.Direction, dir);
	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Light.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	Light.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	Light.PointLightParam = XMFLOAT4(2000.0f, 1.5f, 0.0f, 0.0f);
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