/*==============================================================================
[Smoke.cpp] 煙パーティクル
==============================================================================*/
#include "Header.h"
#include "Smoke.h"
#include "Camera.h"
#include "texture.h"

inline float saturate(float x)
{
	if (x < 0.0f) return 0.0f;
	if (x > 1.0f) return 1.0f;
	return x;
}

HRESULT Smoke::Init(void)
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// 頂点データ（ビルボード板ポリ、原点中心）
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertex;
	GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);

	// シェーダー（ライティング無しの単純テクスチャ描画。CubeVS/CubePSと同じ構成でOK）
	CreateVertexShader(&VertexShader, &VertexLayout, "SmokeVS.cso");
	CreatePixelShader(&PixelShader, "SmokePS.cso");

	if (VertexShader == nullptr) {
		OutputDebugStringA("SmokeVS.cso 読み込み失敗！\n");
	}
	if (PixelShader == nullptr) {
		OutputDebugStringA("SmokePS.cso 読み込み失敗！\n");
	}

	// テクスチャ
	TexID = TextureLoad(L"asset\\texture\\muramatu.png");

	// 半透明合成用のブレンドステート
	D3D11_BLEND_DESC bsd = {};
	bsd.RenderTarget[0].BlendEnable = TRUE;
	bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	GetDevice()->CreateBlendState(&bsd, &BlendState);

	// ★追加：前段（SkyBallなど）のカリング設定を引き継がないよう専用のラスタライザステートを用意
	//         ビルボードは巻き順の向きが怪しくても両面表示されるようCULL_NONEにしておく
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE;
	GetDevice()->CreateRasterizerState(&rd, &RasterizerState);

	ZeroMemory(m_Particle, sizeof(m_Particle));

	return S_OK;
}

void Smoke::Finalize(void)
{
	VertexBuffer->Release();
	VertexLayout->Release();
	VertexShader->Release();
	PixelShader->Release();
	BlendState->Release();
	RasterizerState->Release(); // ★追加
}

void Smoke::Update(void)
{
	const float dt = 1.0f / 60.0f;

	// ---- 新規発生（少しずつ、煙のようにポツポツ出す） ----
	int spawnCount = 2; // 1フレームに出す数。多いともくもく、少ないとまばら
	for (int i = 0; i < MAX_PARTICLES && spawnCount > 0; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].LifeMax = 90; // 約1.5秒（60fps換算）
			m_Particle[i].Life = m_Particle[i].LifeMax;
			m_Particle[i].Position = Position;

			// 煙らしく：横のブレは小さめ、上にゆっくり漂う
			m_Particle[i].Velocity.x = ((float)rand() / RAND_MAX - 0.5f) * 0.3f;
			m_Particle[i].Velocity.y = 0.5f + ((float)rand() / RAND_MAX) * 0.3f;
			m_Particle[i].Velocity.z = ((float)rand() / RAND_MAX - 0.5f) * 0.3f;

			spawnCount--;
		}
	}

	// ---- 既存パーティクルの更新 ----
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (m_Particle[i].Enable == false) continue;

		// 煙は重力で落ちず、逆にゆっくり浮かび上向きの力を持つ
		// 少しずつ横に揺らぐ（簡易乱流）
		m_Particle[i].Velocity.x += ((float)rand() / RAND_MAX - 0.5f) * 0.05f;
		m_Particle[i].Velocity.z += ((float)rand() / RAND_MAX - 0.5f) * 0.05f;

		m_Particle[i].Position.x += m_Particle[i].Velocity.x * dt;
		m_Particle[i].Position.y += m_Particle[i].Velocity.y * dt;
		m_Particle[i].Position.z += m_Particle[i].Velocity.z * dt;

		m_Particle[i].Life--;
		if (m_Particle[i].Life <= 0)
		{
			m_Particle[i].Enable = false;
		}
	}
}

void Smoke::Draw(void)
{
	// ★追加：直前のオブジェクト（SkyBallなど）のラスタライザステートを引き継がないよう明示的に設定
	GetDeviceContext()->RSSetState(RasterizerState);

	GetDeviceContext()->IASetInputLayout(VertexLayout);
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	ID3D11ShaderResourceView* tex = GetTexture(TexID);
	GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

	ID3D11SamplerState* defaultSampler = GetDefaultSamplerState();
	GetDeviceContext()->PSSetSamplers(0, 1, &defaultSampler);

	float blendFactor[4] = { 0,0,0,0 };
	GetDeviceContext()->OMSetBlendState(BlendState, blendFactor, 0xffffffff);

	// ---- カメラ情報からビュー行列を自作 ----
	Camera* camera = GetCamera();

	XMVECTOR eye = XMLoadFloat3(&camera->Position);
	XMVECTOR at = XMLoadFloat3(&camera->AtPosition);
	XMVECTOR up = XMLoadFloat3(&camera->UpVector);

	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
	XMMATRIX invView = XMMatrixInverse(NULL, view);

	// Yビルボード（上下は固定、Y軸回転のみ反映）
	invView.r[0].m128_f32[1] = 0.0f;
	invView.r[1].m128_f32[0] = 0.0f;
	invView.r[1].m128_f32[1] = 1.0f;
	invView.r[1].m128_f32[2] = 0.0f;
	invView.r[2].m128_f32[1] = 0.0f;

	XMVECTOR right = XMVector3Normalize(invView.r[0]);
	XMVECTOR forward = XMVector3Normalize(invView.r[2]);
	invView.r[0] = right;
	invView.r[2] = forward;
	invView.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	MATERIAL material;
	ZeroMemory(&material, sizeof(MATERIAL));

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (m_Particle[i].Enable == false) continue;

		// ↓ デバッグ用に計算をすっ飛ばして固定値にする
		material.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // 赤・完全不透明を強制
		SetMaterial(material);

		float scale = 1.0f; // ← ここも固定（前回のscale計算が悪さしてる可能性を排除）

		XMMATRIX scaleMat = XMMatrixScaling(scale, scale, scale);
		XMMATRIX transMat = XMMatrixTranslation(
			m_Particle[i].Position.x,
			m_Particle[i].Position.y,
			m_Particle[i].Position.z);

		XMMATRIX world = scaleMat * invView * transMat;
		SetWorldMatrix(world);

		GetDeviceContext()->Draw(4, 0);
	}

	GetDeviceContext()->OMSetBlendState(NULL, blendFactor, 0xffffffff);
	GetDeviceContext()->RSSetState(NULL); // ★追加：他のオブジェクトに影響を残さないよう元に戻す
}