#include "Header.h"

#define NUM_VERTEX (4)
#define WIDTH  (5)   // 壁の横幅
#define HEIGHT (3)   // 壁の高さ

/*
	  V0------------------V1
	  |                  |
	 |                  |
	|                  |
   V2-----------------V3
   壁はXY平面（法線はZ方向）に配置
*/
static VERTEX_3D Wall[NUM_VERTEX] =
{
	{//V0 LEFT-TOP
		XMFLOAT3(-WIDTH / 2, HEIGHT +1.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f), // 法線はZ-方向（正面を向く側に合わせて調整）
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
		XMFLOAT2(0.0f, 0.0f)
	},
	{//V1 RIGHT-TOP
		XMFLOAT3(WIDTH / 2, HEIGHT + 1.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f),
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
		XMFLOAT2(1.0f, 0.0f)
	},
	{//V2 LEFT-BOTTOM
		XMFLOAT3(-WIDTH / 2, 0.0f - 1.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f),
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
		XMFLOAT2(0.0f, 1.0f)
	},
	{//V3 RIGHT-BOTTOM
		XMFLOAT3(WIDTH / 2, 0.0f - 1.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f),
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
		XMFLOAT2(1.0f, 1.0f)
	},
};

HRESULT Wall3D::Init(void)
{
	TexID = TextureLoad(L"asset\\texture\\youkan2.png"); // 壁用テクスチャに変更

	CreateVertexShader(&VertexShader, &VertexLayout, "SpotLightVS.cso");
	CreatePixelShader(&PixelShader, "SpotLightPS.cso");

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &VertexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	CopyMemory(&vertex[0], &Wall[0], sizeof(VERTEX_3D) * NUM_VERTEX);
	GetDeviceContext()->Unmap(VertexBuffer, 0);

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

void Wall3D::Finalize(void)
{
	VertexLayout->Release();
	VertexShader->Release();
	PixelShader->Release();
	VertexBuffer->Release();
}

void Wall3D::Update(void)
{
}

void Wall3D::Draw(void)
{
	GetDeviceContext()->IASetInputLayout(VertexLayout);
	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	ID3D11ShaderResourceView* tex = GetTexture(TexID);
	GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

	XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(Rotate.x),
		XMConvertToRadians(Rotate.y),
		XMConvertToRadians(Rotate.z)
	);
	XMMATRIX ScalingMatrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;
	SetWorldMatrix(WorldMatrix);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	MATERIAL material;
	ZeroMemory(&material, sizeof(MATERIAL));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}