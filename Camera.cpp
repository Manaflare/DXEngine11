#include "PreCompiled.h"
#include "Camera.h"
#include "Input.h"
#include "Engine.h"
const float RADIAN = 0.0174532925f;
const XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

Camera::Camera()
{
	m_Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
	m_Target = XMFLOAT3(0.f, 0.f, 0.f);
	m_Up = XMFLOAT3(0.f, 1.f, 0.f);
	m_moveLeftRight = m_moveBackForward = m_camPitch = m_camYaw = 0.f;
	m_mouseLastStateX = m_mouseLastStateY = -1.f;
}


Camera::~Camera()
{
}

void Camera::Initialize(int nScreenWidth, int nScreenHeight)
{
	SetPosition(0.0f, 3.0f, -8.0f);

	XMFLOAT3 eye(m_Position.x, m_Position.y, m_Position.z), lookAt(0.f, 1.f, 0.f), up(0.f, 1.f, 0.f);


	// Init default View Matrix;
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up)));

	// Create the projection matrix for 3D rendering.
	XMStoreFloat4x4(&m_projMatrix, XMMatrixPerspectiveFovLH(XM_PIDIV4, nScreenWidth / nScreenHeight, 1.0f, 1000.0f));

	// Create an orthographic projection matrix for 2D rendering.
	XMStoreFloat4x4(&m_orthoMatrix, XMMatrixOrthographicLH(nScreenWidth, nScreenHeight, 0.1f, 1000.f));
}

void Camera::Update(double time)
{
	Input* input = Engine::GetEngine()->GetInput();
	if (input == nullptr)
		return;

	float speed = 10.f * time;
	
	if (input->IsKeyDown(DIK_A))
	{
		m_moveLeftRight -= speed;
	}
	if (input->IsKeyDown(DIK_D))
	{
		m_moveLeftRight += speed;
	}
	if (input->IsKeyDown(DIK_W))
	{
		m_moveBackForward += speed;
	}

	if (input->IsKeyDown(DIK_S))
	{
		m_moveBackForward -= speed;
	}
	
	int mouseCurrStateX;
	int mouseCurrStateY;

	input->GetMouseState(mouseCurrStateX, mouseCurrStateY);

	// Set the yaw(Y axis), pitch(X axis), and roll(Z axis) rotations in radians.
	if ((mouseCurrStateX != m_mouseLastStateX) || (mouseCurrStateY != m_mouseLastStateY))
	{
		m_camYaw += mouseCurrStateX * 0.001f;
		
		m_camPitch += mouseCurrStateY * 0.001f;

		m_mouseLastStateX = mouseCurrStateX;
		m_mouseLastStateY = mouseCurrStateY;
	}

	UpdateCamera();
}
#include <iostream>
void Camera::UpdateCamera()
{
	static int test = 1;
	XMVECTOR position = XMLoadFloat3(&m_Position);
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(m_camPitch, m_camYaw, 0);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	XMVECTOR vTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	vTarget = XMVector3Normalize(vTarget);

	XMVECTOR camUp = XMLoadFloat3(&m_Up);
	if (test == 1)
	{
		// First-Person Camera
		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(m_camYaw);
		
		camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
		camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
		camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
	}
	else
	{
		// Free Camera
		camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
		camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
		camUp = XMVector3Cross(camForward, camRight);
	}


	position += m_moveLeftRight*camRight;
	position += m_moveBackForward*camForward;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;

	// Translate the rotated camera position to the location of the viewer.
	vTarget = position + vTarget;

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, vTarget, camUp);

	//store
	XMStoreFloat3(&m_Position, position);
	XMStoreFloat3(&m_Target, vTarget);
	XMStoreFloat3(&m_Up, camUp);
	XMStoreFloat4x4(&m_viewMatrix, viewMatrix);

	//std::cout << "camera X : " << m_Position.x << " camera Y : " << m_Position.y << " camera Z : " << m_Position.z << std::endl;
}
