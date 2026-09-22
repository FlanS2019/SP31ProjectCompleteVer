/*==============================================================================

   ネオン背景(サイバーパンク演出) [NeonBackground.cpp]
                                                         Author :
                                                         Date   :
--------------------------------------------------------------------------------

   画面全体を覆う2Dの全画面ポリゴンに、加工済みのネオン写真をレターボックス
   表示し、まわりを流れるオーロラ状のグラデーション・ビーム・パルスリング・
   きらめくスパークル・ネオンの縁取りで派手に演出する背景オブジェクト。

   ※ SetDepthEnable(false) で最初(一番奥)に描画すること。

==============================================================================*/
#include "NeonBackground.h"
#include "sprite.h"
#include "texture.h"

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT NeonBackground::Init(void)
{
	//前回作成したサイバーパンク加工済みの写真を背景として読み込む(背景は絶対コレを使う)
	TexID = TextureLoad(L"asset\\texture\\photo1_cyberpunk.png");

	//シェーダー読み込み(頂点は既存のUnlitColorVS、ピクセルは専用のネオン演出シェーダー)
	CreateVertexShader(&VertexShader, &VertexLayout, "UnlitColorVS.cso");
	CreatePixelShader(&PixelShader, "NeonBGPS.cso");

	//画面いっぱいに表示する2Dオブジェクトとして初期化
	Position = XMFLOAT3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f);
	Size = XMFLOAT2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	Time = 0.0f;

	float w = (float)TextureGetWidth(TexID);
	float h = (float)TextureGetHeight(TexID);
	Parameter = XMFLOAT4((h > 0.0f) ? (w / h) : 1.0f, 0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void NeonBackground::Finalize(void)
{
	VertexLayout->Release();
	VertexShader->Release();
	PixelShader->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void NeonBackground::Update(void)
{
	Time += (1.0f / 60.0f);
	Parameter.w = Time;
}

//=============================================================================
// 描画処理
//=============================================================================
void NeonBackground::Draw(void)
{
	//奥行き処理をOFF(一番奥に全画面表示するため)
	SetDepthEnable(false);

	//2D用の射影行列に切り替え
	SetWorldViewProjection2D();

	//頂点レイアウト設定
	GetDeviceContext()->IASetInputLayout(VertexLayout);
	//頂点シェーダーをセット
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	//ピクセルシェーダーをセット
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//写真テクスチャをセット
	ID3D11ShaderResourceView* tex = GetTexture(TexID);
	GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

	//平行移動行列作成(画面中央に配置)
	XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
	SetWorldMatrix(TranslationMatrix);

	//アニメーションパラメータ(縦横比・経過時間)を送る
	SetParameter(Parameter);

	//画面いっぱいのポリゴンを描画
	DrawSprite(Size, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
