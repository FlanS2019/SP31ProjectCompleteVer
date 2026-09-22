
//Camera.cpp

#include	"Camera.h"
#include	"keyboard.h"

Camera		g_Camera;	//カメラ管理構造体
static float g_ManualHeightOffset = 0.0f;	//E/Qキーでの手動高さオフセット


void	InitCamera()
{
	//カメラの初期化
	g_Camera.Position = XMFLOAT3(0.0f, 1.5f, -2.0f);	//カメラ基本座標
	g_Camera.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);		//カメラの上方ベクトル
	g_Camera.AtPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);	//カメラの注視点座標
	g_Camera.Fov = 45.0f;								//画角
	g_Camera.Nearclip = 0.5f;							//近面クリップ
	g_Camera.Farclip = 1000.0f;							//遠面クリップ
	g_Camera.Rotation = 0.0f;
}

void	FinalizeCamera()
{

}

void	UpdateCamera()
{

	g_Camera.Rotation = 0;

	if (Keyboard_IsKeyDown(KK_A))
	{
		g_Camera.Rotation = 0.3f;
	}
	else if (Keyboard_IsKeyDown(KK_D))
	{
		g_Camera.Rotation = -0.3f;
	}

	float co = cosf(XMConvertToRadians(g_Camera.Rotation));
	float si = sinf(XMConvertToRadians(g_Camera.Rotation));
	float posx = (g_Camera.Position.x * co) - (g_Camera.Position.z * si);
	float posz = (g_Camera.Position.x * si) + (g_Camera.Position.z * co);

	g_Camera.Position.x = posx;
	g_Camera.Position.z = posz;

	//Eキーで上へ、Qキーで下へ、手動でもカメラの高さを操作できるようにする
	if (Keyboard_IsKeyDown(KK_E))
	{
		g_ManualHeightOffset += 1.2f * (1.0f / 60.0f);
	}
	else if (Keyboard_IsKeyDown(KK_Q))
	{
		g_ManualHeightOffset -= 1.2f * (1.0f / 60.0f);
	}
	if (g_ManualHeightOffset > 2.0f)  g_ManualHeightOffset = 2.0f;
	if (g_ManualHeightOffset < -1.3f) g_ManualHeightOffset = -1.3f;

	//カメラが上下にゆっくり首振りするように高さをアニメーションさせる(自動の首振り+手動オフセット)
	float t = GetTime();
	g_Camera.Position.y = 1.5f + sinf(t * 0.5f) * 1.0f + g_ManualHeightOffset;	// 自動0.5~2.5 + 手動-1.3~2.0

}

void	DrawCamera()//3D使用時
{
	//プロジェクション行列を作成
	XMMATRIX	ProjectionMatrix =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(g_Camera.Fov),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			g_Camera.Nearclip,
			g_Camera.Farclip
		);
	//プロジェクション行列をセット
	SetProjectionMatrix(ProjectionMatrix);

	//カメラ行列を作成
	XMVECTOR	eyev = XMLoadFloat3(&g_Camera.AtPosition);
	XMVECTOR	pos = XMLoadFloat3(&g_Camera.Position);
	XMVECTOR	up = XMLoadFloat3(&g_Camera.UpVector);
	XMMATRIX	ViewMatrix =
		XMMatrixLookAtLH(pos, eyev, up);

	//カメラ行列をセット
	SetViewMatrix(ViewMatrix);
	//カメラ座標セット
	SetCameraPosition(g_Camera.Position);
}
Camera* GetCamera()
{
	return &g_Camera;
}




