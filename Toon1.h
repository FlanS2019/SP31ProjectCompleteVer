/*==============================================================================

   [PolygonModel.h]
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


class Toon1
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

	XMFLOAT4 Parameter; // パラメータ用の構造体

public:

	HRESULT Init(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);
};