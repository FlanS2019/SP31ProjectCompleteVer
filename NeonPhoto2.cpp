/*==============================================================================

   ネオン写真カード2枚目(くるくる回転) [NeonPhoto2.cpp]
                                                         Author :
                                                         Date   :
--------------------------------------------------------------------------------

   2枚目のサイバーパンク加工済み写真(photo2_cyberpunk.png)を、画面右上に
   常時くるくる回転するネオンフレーム付きカードとして表示する。
   3D描画のあと・depth無効で最後に描くので、他のモデルに隠れず必ず見える。

==============================================================================*/
#include "NeonPhoto2.h"
#include "sprite.h"
#include "texture.h"

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT NeonPhoto2::Init(void)
{
	//もう1枚のサイバーパンク加工済み写真を読み込む
	TexID = TextureLoad(L"asset\\texture\\photo2_cyberpunk.png");

	//シェーダー読み込み(頂点は既存のUnlitColorVS、ピクセルは専用のネオンカードシェーダー)
	CreateVertexShader(&VertexShader, &VertexLayout, "UnlitColorVS.cso");
	CreatePixelShader(&PixelShader, "NeonPhotoCardPS.cso");

	//写真の縦横比を保ったサイズを計算
	float w = (float)TextureGetWidth(TexID);
	float h = (float)TextureGetHeight(TexID);
	float aspect = (h > 0.0f) ? (w / h) : 1.0f;

	float targetH = 300.0f;
	Size = XMFLOAT2(targetH * aspect, targetH);

	//画面右上にカードとして配置(左側のImGuiパネルと重ならない位置)
	Position = XMFLOAT3(1120.0f, 210.0f, 0.0f);
	Rotate = 0.0f;

	Time = 0.0f;
	Parameter = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void NeonPhoto2::Finalize(void)
{
	VertexLayout->Release();
	VertexShader->Release();
	PixelShader->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void NeonPhoto2::Update(void)
{
	Time += (1.0f / 60.0f);
	Parameter.w = Time;

	//やかましく常時くるくる回転させる
	Rotate += 50.0f * (1.0f / 60.0f);
}

//=============================================================================
// 描画処理
//=============================================================================
void NeonPhoto2::Draw(void)
{
	//奥行き処理をOFF(常に手前に見せる)
	SetDepthEnable(false);

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

	//平行移動 + Z回転行列作成
	XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
	XMMATRIX RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(Rotate));
	XMMATRIX WorldMatrix = RotationMatrix * TranslationMatrix;
	SetWorldMatrix(WorldMatrix);

	//アニメーションパラメータ(経過時間)を送る
	SetParameter(Parameter);

	//カードを描画
	DrawSprite(Size, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
