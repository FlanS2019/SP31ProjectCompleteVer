/*==============================================================================
   [SpotLight.h]
==============================================================================*/
#pragma once
#include "main.h"
#include "renderer.h"
#include "model.h"

//*****************************************************************************
// スポットライト用構造体
//*****************************************************************************
struct SPOT_LIGHT
{
    XMFLOAT4 Diffuse;       // 光の色       (PSスロットb4)
    XMFLOAT4 Ambient;       // 環境光の色
    XMFLOAT4 Position;      // ライトのワールド座標
    XMFLOAT4 Direction;     // 照射方向（正規化済み）
    float    ConeAngle;     // スポットの半頂角（度数）
    float    Attenuation;   // 距離減衰係数
    float    Pow;           // ソフトエッジ用べき乗
    float    _pad;          // 16バイトアライメント用パディング
};

//*****************************************************************************
// SpotLight クラス
//*****************************************************************************
class SpotLight
{
protected:
    // トランスフォーム
    XMFLOAT3    Position;
    XMFLOAT3    Scale;
    XMFLOAT3    Rotate;

    // テクスチャ
    int         TexID;

    // シェーダー関連
    ID3D11VertexShader* VertexShader;
    ID3D11PixelShader*  PixelShader;
    ID3D11InputLayout*  VertexLayout;

    // モデル
    MODEL*      Model;

    // ライト
    SPOT_LIGHT  SpotLightParam;

    // 定数バッファ
    ID3D11Buffer* SpotLightCBuffer;  // PSスロット b4
    ID3D11Buffer* CameraCBuffer;     // PSスロット b5

public:
    HRESULT Init(void);
    void    Finalize(void);
    void    Update(void);
    void    Draw(void);

    // ライトパラメータのセッター（外部から変更したい場合に使う）
    void SetLightPosition(XMFLOAT3 pos) { SpotLightParam.Position = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f); }
    void SetLightDiffuse(XMFLOAT4 col)  { SpotLightParam.Diffuse  = col; }
    void SetConeAngle(float angle)      { SpotLightParam.ConeAngle = angle; }
    void SetAttenuation(float atten)    { SpotLightParam.Attenuation = atten; }
    void SetPow(float pow)              { SpotLightParam.Pow = pow; }
};
