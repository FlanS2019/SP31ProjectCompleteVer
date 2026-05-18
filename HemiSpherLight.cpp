#include "HemiSpherLight.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "keyboard.h"

//Initialize

HRESULT HemiSpherLight::Init(void)
{
	//shaderの読み込み

	CreateVertexShader(&VertexShader, &VertexLayout, "VertexDirectionalLightingVS.cso");
	CreatePixelShader(&PixelShader, "VertexDirectionalLightingPS.cso");

	//3DObjectの読み込み
	Position = XMFLOAT3(1.5f, 0.2f, 0.0f);
	Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);
	//モデルの読み込み
	Model = ModelLoad("asset\\model\\cube.fbx");

	XMVECTOR dir = XMVector4Normalize(XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f));
	dir = XMVector3Normalize(dir);
	XMStoreFloat4(&Light.Direction, dir);
	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	dir = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat4(&Light.GroundNormal, dir);

	Light.SkyColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	Light.GroundColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);



	return S_OK;
}

//Finalize

void HemiSpherLight::Finalize(void)
{
	if (VertexShader) {
		VertexShader->Release();
		VertexShader = nullptr;
	}
	if (PixelShader) {
		PixelShader->Release();
		PixelShader = nullptr;
	}
	if (VertexLayout) {
		VertexLayout->Release();
		VertexLayout = nullptr;
	}
	ModelRelease(Model);
}

//Update

void HemiSpherLight::Update(void)
{
	if(Keyboard_IsKeyDown(KK_UP))
	{
		Position.z += 0.3f * (1.0f / 60.0f);
	}
	else if(Keyboard_IsKeyDown(KK_DOWN))
	{
		Position.z -= 0.3f * (1.0f / 60.0f);
	}
	if(Keyboard_IsKeyDown(KK_LEFT))
	{
		Position.x -= 0.3f * (1.0f / 60.0f);
	}
	else if(Keyboard_IsKeyDown(KK_RIGHT))
	{
		Position.x += 0.3f * (1.0f / 60.0f);
	}
	// 修正: Z / X は位置ではなく回転を変更する
	if(Keyboard_IsKeyDown(KK_Z))
	{
		Rotate.x += 60.0f * (1.0f / 60.0f); // 1度/frame 増加
	}
	else if(Keyboard_IsKeyDown(KK_X))
	{
		Rotate.x -= 60.0f * (1.0f / 60.0f); // 1度/frame 減少
	}
}

//draw

void HemiSpherLight::Draw(void)
{
	GetDeviceContext()->IASetInputLayout(VertexLayout);

	GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);

	GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

	SetLight(Light);

	{//3dpolygon
		ID3D11ShaderResourceView* tex = GetTexture(TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//平行移動
		XMMATRIX TranslationMatrix = XMMatrixTranslation
		(Position.x, Position.y, Position.z);

		//回転
		XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw
		(XMConvertToRadians(Rotate.x),
			XMConvertToRadians(Rotate.y), 
			XMConvertToRadians(Rotate.z));

		//拡大縮小
		XMMATRIX ScalingMatrix = XMMatrixScaling
		(Scale.x, Scale.y, Scale.z);

		//ワールド行列の合成
		XMMATRIX WorldMatrix = 
			ScalingMatrix * RotationMatrix * TranslationMatrix;

		//DirextXに行列を転送
		SetWorldMatrix(WorldMatrix);

		//プリミティブトポロジーの設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Materialの描画
		MATERIAL material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		ModelDraw(Model);
	}
}

