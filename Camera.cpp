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

	//create projection matrix
	XMMatrixPerspectiveFovLH(XM_PIDIV4, nScreenWidth / nScreenHeight, 1.0f, 1000.0f);
}

void Camera::Update()
{
	Input* input = Engine::GetEngine()->GetInput();
	if (input == nullptr)
		return;

	//need a time
	float time = 0.0005f;
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
void Camera::UpdateCamera()
{

	XMVECTOR position = XMLoadFloat3(&m_Position);
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(m_camPitch, m_camYaw, 0);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	m_Target = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	m_Target = XMVector3Normalize(m_Target);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(m_camYaw);

	// Walk
	//camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	//camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	//camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	// Free Cam
	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	XMVECTOR camUp = XMVector3Cross(camForward, camRight);

	position += m_moveLeftRight*camRight;
	position += m_moveBackForward*camForward;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;

	// Translate the rotated camera position to the location of the viewer.
	m_Target = position + m_Target;

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, m_Target, camUp);

	//store
	XMStoreFloat3(&m_Position, position);
	XMStoreFloat4x4(&m_viewMatrix, viewMatrix);
}
