
//===== �C���N���[�h�� =====
#include <Camera/CameraMgr.h>
#include <Camera/Camera_Test.h>
#include <Camera/Camera_FP.h>

namespace dx = DirectX;

//===== �N���X���� =====
CAMERA_MGR::CAMERA_MGR(APP_64& App) noexcept : m_App(App), m_mtxView(), m_mtxProj(), m_aCamera(static_cast<int>(CAMERA_ID::MAX_CAMERA)), m_CurrentCamera(CAMERA_ID::TEST)
{
	//�s�񏉊���
	dx::XMMATRIX mtx = dx::XMMatrixIdentity();
	dx::XMStoreFloat4x4(&m_mtxView, mtx);
	dx::XMStoreFloat4x4(&m_mtxProj, mtx);

	//�J����������
	m_aCamera[static_cast<int>(CAMERA_ID::TEST)] = std::make_unique<CAMERA_TEST>(App);
	m_aCamera[static_cast<int>(CAMERA_ID::FP)] = std::make_unique<CAMERA_FP>(App);
}

CAMERA_MGR::~CAMERA_MGR() noexcept
{
}

//�X�V����
void CAMERA_MGR::Update() noexcept
{
	m_aCamera[static_cast<int>(m_CurrentCamera)]->Update();
	m_mtxView = m_aCamera[static_cast<int>(m_CurrentCamera)]->GetViewMtx();
	m_mtxProj = m_aCamera[static_cast<int>(m_CurrentCamera)]->GetProjMtx();
}

//�`�揈��
void CAMERA_MGR::Draw() const noexcept
{
	//�s������o�C���h
	m_App.GetGfxPack().m_DX.SetViewMtx(m_mtxView);
	m_App.GetGfxPack().m_DX.SetProjectionMtx(m_mtxProj);
	m_App.GetGfxPack().m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::CB_VS_MTX_VP);
}

//�J�����Z�b�g
void CAMERA_MGR::SetCamera(CAMERA_ID id) noexcept
{
	m_CurrentCamera = id;
}

//���[���h�s��擾
DirectX::XMFLOAT4X4 CAMERA_MGR::GetWorldMtx() const noexcept
{
	return m_aCamera[static_cast<int>(m_CurrentCamera)]->GetWorldMtx();
}

//��]���擾
DirectX::XMFLOAT3 CAMERA_MGR::GetRotation() const noexcept
{
	return m_aCamera[static_cast<int>(m_CurrentCamera)]->GetRotation();
}
