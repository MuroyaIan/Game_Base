
//===== �C���N���[�h�� =====
#include <Camera/Camera_Test.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== �O���[�o���萔�錾 =====
constexpr dx::XMFLOAT3 START_POS = { 0.0f, 0.0f, -20.0f };	//�J�����ʒu
constexpr dx::XMFLOAT3 START_LOOK = { 0.0f, 0.0f, 0.0f };	//�����_�ʒu
constexpr dx::XMFLOAT3 START_UP = { 0.0f, 1.0f, 0.0f };		//�A�b�v�x�N�g��

constexpr short START_FOV = 60;			//����p
constexpr float START_NEAR_Z = 0.5f;
constexpr float START_FAR_Z = 40.0f;	//�N���b�v����

//===== �N���X���� =====
CAMERA_TEST::CAMERA_TEST(APP& App) noexcept : CAMERA(App), m_Input(m_App.GetInputMgr())
{
	//�e�����o������
	m_Pos = START_POS;
	m_LookAt = START_LOOK;
	m_vUp = START_UP;
	m_FOV = START_FOV;
	m_NearZ = START_NEAR_Z;
	m_FarZ = START_FAR_Z;

	//�s�񏉊���
	CAMERA::InitMatrix();
}

CAMERA_TEST::~CAMERA_TEST() noexcept
{
}

//�X�V����
void CAMERA_TEST::Update() noexcept
{
	//��]����
	if (m_Input.m_KB.GetPress(VK_W))
		m_Rot.x += gMath::GetRad(1);
	else if (m_Input.m_KB.GetPress(VK_S))
		m_Rot.x -= gMath::GetRad(1);
	if (m_Input.m_KB.GetPress(VK_A))
		m_Rot.y += gMath::GetRad(1);
	else if (m_Input.m_KB.GetPress(VK_D))
		m_Rot.y -= gMath::GetRad(1);
	if (m_Input.m_KB.GetTrigger(VK_R))
		ResetCamera();

	//��]�p�x����
	if (m_Rot.x > fPI)
		m_Rot.x -= f2PI;
	else if (m_Rot.x < -fPI)
		m_Rot.x += f2PI;
	if (m_Rot.y > fPI)
		m_Rot.y -= f2PI;
	else if (m_Rot.y < -fPI)
		m_Rot.y += f2PI;

	//�s��X�V
	UpdateMatrix();
}

//�J�������Z�b�g
void CAMERA_TEST::ResetCamera() noexcept
{
	//�f�[�^������
	m_Rot.x = 0.0f;
	m_Rot.y = 0.0f;
}

//�s��X�V
void CAMERA_TEST::UpdateMatrix() noexcept
{
	//���[���h�s��
	dx::XMStoreFloat4x4(&m_mtxWorld,
		dx::XMMatrixTranslation(START_POS.x, START_POS.y, START_POS.z)
		* dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z)
	);

	//�r���[�s��
	m_Pos = { m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43 };
	m_vUp = { m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23 };
	dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixLookAtLH(
		dx::XMVectorSet(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f),
		dx::XMVectorSet(m_LookAt.x, m_LookAt.y, m_LookAt.z, 0.0f),
		dx::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f))
	);
}
