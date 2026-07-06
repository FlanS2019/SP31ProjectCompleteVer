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
#include "IGameObject3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


////////////////
//構造体
////////////////


class VertexDirectionalLighting : public IGameObject3D
{
	protected:
		int			TexID;

		ID3D11VertexShader* VertexShader; //頂点シェーダーオブジェクト
		ID3D11PixelShader* PixelShader; //ピクセルシェーダーオブジェクト
		ID3D11InputLayout* VertexLayout; //頂点レイアウトオブジェクト

		LIGHT	Light;

		MODEL* Model;

	public:

		HRESULT Init(void);
		void Finalize(void);
		void Update(void);
		void Draw(void);
		XMFLOAT3 Position;
		XMFLOAT3 Scale;
		XMFLOAT3 Rotate;
};







//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

