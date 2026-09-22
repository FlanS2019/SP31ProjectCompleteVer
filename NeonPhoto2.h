/*==============================================================================

   ネオン写真カード2枚目(くるくる回転) [NeonPhoto2.h]
                                                         Author :
                                                         Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"
#include "renderer.h"

////////////////
//クラス
////////////////

class NeonPhoto2
{
protected:
	XMFLOAT3	Position;		//表示座標(画面右上のカード位置)
	XMFLOAT2	Size;			//サイズ(写真の縦横比を保つ)
	float		Rotate;			//回転角度(常時くるくる回す)
	int			TexID;			//2枚目の写真テクスチャID

	ID3D11VertexShader* VertexShader; //頂点シェーダーオブジェクト
	ID3D11PixelShader* PixelShader; //ピクセルシェーダーオブジェクト
	ID3D11InputLayout* VertexLayout; //頂点レイアウトオブジェクト

	XMFLOAT4	Parameter;		// w:経過時間(縁取りアニメーション用)
	float		Time;

public:
	HRESULT Init(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);
};
