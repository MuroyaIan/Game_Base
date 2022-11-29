
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Viewer_Camera.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== �O���[�o���萔�錾 =====
constexpr dx::XMFLOAT3 START_POS = { 0.0f, 0.0f, -20.0f };	//�J�����ʒu
constexpr dx::XMFLOAT3 START_LOOK = { 0.0f, 0.0f, 0.0f };	//�����_�ʒu
constexpr dx::XMFLOAT3 START_UP = { 0.0f, 1.0f, 0.0f };		//�A�b�v�x�N�g��

//===== �N���X���� =====
VIEWER_CAM::VIEWER_CAM(APP& App) noexcept :
	m_App(App), m_LookAt(START_LOOK), m_vUp(START_UP),
	m_FOV(60), g_AspectRatio(), m_NearZ(0.5f), m_FarZ(10000.0f),
	m_mtxWorld(), m_mtxView(), m_mtxProj(),
	m_Mode(MODE::NONE), m_MousePos_Old(dx::XMFLOAT2(0.0f, 0.0f)),
	m_Rot(dx::XMFLOAT3(gMath::GetRad(30), gMath::GetRad(-30), 0.0f)), m_Level_RotSPD(5), m_Pos(START_POS),
	m_Level_Zoom(0), m_Backup_Wheel(0), m_ZoomSPD(1), m_CtrDist(START_POS.z), m_Offset(dx::XMFLOAT3(0.0f, 0.0f, 0.0f))
{
	//�c���䏉����
	auto [x, y] = m_App.GetWindowProc()->GetSize();
	g_AspectRatio = static_cast<float>(x) / static_cast<float>(y);

	//�s�񏉊���
	UpdateMatrix();

	//�}�E�X�z�C�[���l������
	m_Backup_Wheel = m_App.GetInputMgr().m_Mouse.GetWheelVal();
}

VIEWER_CAM::~VIEWER_CAM() noexcept
{
}

//�X�V����
void VIEWER_CAM::Update() noexcept
{
	//�J�������[�h�ؑ�
	if (m_App.GetWindowProc()->m_Mouse.LeftIsPressed())
		m_Mode = MODE::ORBIT;
	else if (m_App.GetWindowProc()->m_Mouse.RightIsPressed())
		m_Mode = MODE::TRACK;
	else
		m_Mode = MODE::NONE;

	//��O����
	if (m_Mode == MODE::NONE) {

		//�Y�[������
		int Val = m_App.GetInputMgr().m_Mouse.GetWheelVal();
		if (m_Backup_Wheel > Val) {
			m_Level_Zoom--;							//�Y�[�������X�V
			m_Backup_Wheel = Val;					//�}�E�X�z�C�[���l�ۑ�
			UpdateMatrix();							//�s��X�V
		}
		else if (m_Backup_Wheel < Val) {
			if (m_CtrDist < -1.0f)					//���ʂȃY�[���C����h��
				m_Level_Zoom++;
			m_Backup_Wheel = Val;
			UpdateMatrix();
		}

		m_MousePos_Old = m_App.GetInputMgr().m_Mouse.GetPos();	//���W�ۑ�
		return;
	}

	//�}�E�X�ړ��ʎZ�o
	dx::XMFLOAT2 MousePos = m_App.GetInputMgr().m_Mouse.GetPos();
	dx::XMFLOAT2 MouseMove = { MousePos.x - m_MousePos_Old.x, MousePos.y - m_MousePos_Old.y };

	//�I�[�r�b�g����
	if (m_Mode == MODE::ORBIT) {

		//��]�ʌv�Z
		float AngleX = 360.0f * MouseMove.x / 1600.0f;
		float AngleY = 180.0f * MouseMove.y / 900.0f;
		float Delta = 0.04f * m_Level_RotSPD * 0.2f;
		m_Rot.x += AngleY * Delta;
		m_Rot.y += AngleX * Delta;
	}

	//***** �g���b�N���� *****
	if (m_Mode == MODE::TRACK) {

		//�I�t�Z�b�g�X�V
		float dVal = 0.04f * m_CtrDist / START_POS.z;
		m_Offset.x += (MouseMove.x * m_mtxWorld._11 - MouseMove.y * m_mtxWorld._21) * dVal;
		m_Offset.y += (MouseMove.x * m_mtxWorld._12 - MouseMove.y * m_mtxWorld._22) * dVal;
		m_Offset.z += (MouseMove.x * m_mtxWorld._13 - MouseMove.y * m_mtxWorld._23) * dVal;
	}

	//�s��X�V
	UpdateMatrix();

	//�}�E�X���W�ۑ�
	m_MousePos_Old = m_App.GetInputMgr().m_Mouse.GetPos();
}

//�s��X�V
void VIEWER_CAM::UpdateMatrix() noexcept
{
	//���[���h�s��
	m_CtrDist = START_POS.z + m_Level_Zoom * m_ZoomSPD;
	if (m_CtrDist > -1.0f)
		m_CtrDist = -1.0f;
	dx::XMMATRIX mtx = dx::XMMatrixTranslation(START_POS.x, START_POS.y, m_CtrDist)
		* dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z)
		* dx::XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);
	dx::XMStoreFloat4x4(&m_mtxWorld, mtx);

	//�r���[�s��
	m_Pos = { m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43 };
	m_vUp = { m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23 };
	mtx = dx::XMMatrixLookAtLH(
		dx::XMVectorSet(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f),
		dx::XMVectorSet(m_LookAt.x + m_Offset.x, m_LookAt.y + m_Offset.y, m_LookAt.z + m_Offset.z, 0.0f),
		dx::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f)
	);
	dx::XMStoreFloat4x4(&m_mtxView, mtx);

	//���e�s��
	mtx = dx::XMMatrixPerspectiveFovLH(gMath::GetRad(m_FOV), g_AspectRatio, m_NearZ, m_FarZ);
	dx::XMStoreFloat4x4(&m_mtxProj, mtx);

	//�s������o�C���h
	m_App.GetGfxPack().m_DX.SetViewMtx(m_mtxView);
	m_App.GetGfxPack().m_DX.SetProjectionMtx(m_mtxProj);
}

//�J�������Z�b�g
void VIEWER_CAM::ResetCamera() noexcept
{
	//�f�[�^������
	m_Rot = { gMath::GetRad(30), gMath::GetRad(-30), 0.0f };
	m_Level_Zoom = 0;
	m_ZoomSPD = 1;
	m_Offset = { 0.0f, 0.0f, 0.0f };

	//�s��X�V
	UpdateMatrix();
}
