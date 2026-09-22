/*==============================================================================

   映えエフェクト(ポスタライズ+ノイズ) [PosterizeNoise.h]
													 Author :
													 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


////////////////
//構造体
////////////////


class PosterizeNoise
{
protected:
	XMFLOAT3	Position;
	XMFLOAT3	Scale;
	XMFLOAT3	Rotate;
	int			TexID;

	ID3D11VertexShader* VertexShader; //頂点シェーダーオブジェクト
	ID3D11PixelShader* PixelShader; //ピクセルシェーダーオブジェクト
	ID3D11InputLayout* VertexLayout; //頂点レイアウトオブジェクト

	LIGHT	Light;

	MODEL* Model;

	XMFLOAT4 Parameter; // x:ポスタライズ段階 y:ノイズ強度 z:リムパワー w:経過時間
	float	 Time;

public:

	HRESULT Init(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);
};
