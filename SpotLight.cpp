/*==============================================================================
   [SpotLightBuffer.cpp]
   SpotLightPS.hlsl の cbuffer(b4) へパラメータを渡すヘルパー実装例
   ※ renderer.cpp / renderer.h に統合するか、SpotLight::Draw() 内で使う
==============================================================================*/

// -----------------------------------------------------------------------
// 定数バッファに対応する CPU 側の構造体
// （SpotLight.h か renderer.h に追加してください）
// -----------------------------------------------------------------------

struct SPOT_LIGHT
{
    XMFLOAT4 Diffuse;       // 光の色
    XMFLOAT4 Ambient;       // 環境光
    XMFLOAT4 Position;      // ライト位置
    XMFLOAT4 Direction;     // 照射方向（正規化済み）
    float    ConeAngle;     // 半頂角（度数）
    float    Attenuation;   // 距離減衰係数
    float    Pow;           // ソフトエッジ用べき乗
    float    _pad;
};

// -----------------------------------------------------------------------
// SpotLight クラスのメンバー変数に追加するもの（SpotLight.h に追記）
// -----------------------------------------------------------------------
//   SPOT_LIGHT       SpotLightParam;
//   ID3D11Buffer*    SpotLightBuffer;
//   ID3D11Buffer*    CameraBuffer;

// -----------------------------------------------------------------------
// Init() 内に追加するバッファ生成コード
// -----------------------------------------------------------------------
/*
    // SpotLight 定数バッファ作成
    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth           = sizeof(SPOT_LIGHT);
    cbd.Usage               = D3D11_USAGE_DEFAULT;
    cbd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    GetDevice()->CreateBuffer(&cbd, nullptr, &SpotLightBuffer);

    // Camera 定数バッファ作成
    cbd.ByteWidth = sizeof(XMFLOAT4);
    GetDevice()->CreateBuffer(&cbd, nullptr, &CameraBuffer);

    // スポットライトパラメータ初期化
    SpotLightParam.Diffuse     = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SpotLightParam.Ambient     = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    SpotLightParam.Position    = XMFLOAT4(0.3f, 0.7f, -1.0f, 1.0f); // ライト位置（調整可）
    XMVECTOR dir = XMVector3Normalize(XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f));
    XMStoreFloat4(&SpotLightParam.Direction, dir);
    SpotLightParam.ConeAngle   = 23.0f;   // UIのCone Angle
    SpotLightParam.Attenuation = 10.0f;   // UIのAttenuation
    SpotLightParam.Pow         = 9.49f;   // UIのPow
*/

// -----------------------------------------------------------------------
// Draw() 内に追加する定数バッファ送信コード
// -----------------------------------------------------------------------
/*
    // SpotLight パラメータを更新してセット（PSスロット4）
    GetDeviceContext()->UpdateSubresource(SpotLightBuffer, 0, nullptr, &SpotLightParam, 0, 0);
    GetDeviceContext()->PSSetConstantBuffers(4, 1, &SpotLightBuffer);

    // カメラ位置を取得してセット（PSスロット5）
    XMFLOAT3 camPos = GetCamera()->GetPosition(); // ※Camera クラスに合わせて変更
    XMFLOAT4 camPos4 = XMFLOAT4(camPos.x, camPos.y, camPos.z, 1.0f);
    GetDeviceContext()->UpdateSubresource(CameraBuffer, 0, nullptr, &camPos4, 0, 0);
    GetDeviceContext()->PSSetConstantBuffers(5, 1, &CameraBuffer);
*/

// -----------------------------------------------------------------------
// Finalize() 内に追加する解放コード
// -----------------------------------------------------------------------
/*
    if (SpotLightBuffer) { SpotLightBuffer->Release(); SpotLightBuffer = nullptr; }
    if (CameraBuffer)    { CameraBuffer->Release();    CameraBuffer = nullptr;    }
*/
