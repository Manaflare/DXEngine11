#pragma once

#include <DirectXMath.h>

using namespace DirectX;
class Camera
{
public:
	Camera();
	~Camera();


	void Initialize(int nScreenWidth, int nScreenHeight);
	void SetPosition(float x, float y, float z) { m_Position.x = x, m_Position.y = y; m_Position.z = z; }
	void SetPosition(XMFLOAT3 vPos)				{ m_Position = vPos; }
	void SetRotation(float x, float y, float z) { m_Rotation.x = x, m_Rotation.y = y; m_Rotation.z = z; }
	void SetRotation(XMFLOAT3 vRot)				{ m_Rotation = vRot; }

	XMFLOAT3	GetPosition() const				{ return m_Position; }
	XMFLOAT3	GetRotation() const				{ return m_Rotation; }
	void GetViewMatrix(XMFLOAT4X4& pOut)		{ pOut = m_viewMatrix; }
	void Update();
	void UpdateCamera();
	

private:

	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT4X4 m_viewMatrix;
	XMVECTOR m_Target;
	XMVECTOR m_Up;

	float m_moveLeftRight;
	float m_moveBackForward;
	float m_camYaw;
	float m_camPitch;

	float m_mouseLastStateX;
	float m_mouseLastStateY;

};

