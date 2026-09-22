/*==============================================================================

   ネオン背景(サイバーパンク演出) [NeonBackground.h]
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

class NeonBackground
{
protected:
	XMFLOAT3	Position;		//表示座標(画面中心)
	XMFLOAT2	Size;			//サイズ(画面サイズ)
	int			TexID;			//写真テクスチャID

	ID3D11VertexShader* VertexShader; //頂点シェーダーオブジェクト
	ID3D11PixelShader* PixelShader; //ピクセルシェーダーオブジェクト
	ID3D11InputLayout* VertexLayout; //頂点レイアウトオブジェクト

	XMFLOAT4	Parameter;		// x:写真の縦横比(幅/高さ)  w:経過時間(アニメーション用)
	float		Time;

public:
	HRESULT Init(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);
};
