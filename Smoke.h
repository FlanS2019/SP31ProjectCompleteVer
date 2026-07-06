#pragma once
#include "Header.h"
class Smoke : public IGameObject3D
{
public:
	HRESULT Init(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	XMFLOAT3 Position; // 発生源の位置
private:
	struct PARTICLE
	{
		bool     Enable;
		int      Life;      // 現在の残り寿命
		int      LifeMax;    // 誕生時の寿命（フェード計算に使う）
		XMFLOAT3 Position;
		XMFLOAT3 Velocity;
	};
	static const int MAX_PARTICLES = 100;
	PARTICLE m_Particle[MAX_PARTICLES];
	ID3D11Buffer* VertexBuffer;
	ID3D11InputLayout* VertexLayout;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11BlendState* BlendState;   // 半透明合成用
	ID3D11RasterizerState* RasterizerState; // ★追加：前段のカリング設定を引き継がないようにする
	int TexID;
};