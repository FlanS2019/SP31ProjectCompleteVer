#pragma once
#include "Header.h"

class TexCube :public IGameObject3D
{
public:
	HRESULT Init(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	XMFLOAT3	Position;
	XMFLOAT3	Rotate;
	XMFLOAT3	Scale;

private:
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* VertexLayout;

	MODEL* Model;
	int			TexID;
};