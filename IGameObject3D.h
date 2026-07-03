#pragma once
// IGameObject3D.h
class IGameObject3D
{
public:
	virtual ~IGameObject3D() {}
	virtual HRESULT Init() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};