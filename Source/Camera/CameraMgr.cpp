
//===== インクルード部 =====
#include <Camera/CameraMgr.h>
#include <Camera/Camera_Test.h>
#include <Camera/Camera_FP.h>

namespace dx = DirectX;

//===== クラス実装 =====
CAMERA_MGR::CAMERA_MGR(APP& App) noexcept : m_App(App), m_aCamera(static_cast<int>(CAMERA_ID::MAX_CAMERA)), m_CurrentCamera(CAMERA_ID::Test)
{
	//カメラ初期化
	m_aCamera[static_cast<int>(CAMERA_ID::Test)] = std::make_unique<CAMERA_TEST>(App);
	m_aCamera[static_cast<int>(CAMERA_ID::FirstPerson)] = std::make_unique<CAMERA_FP>(App);
}

CAMERA_MGR::~CAMERA_MGR() noexcept
{
}

//更新処理
void CAMERA_MGR::Update() const noexcept
{
	int CameraID = static_cast<int>(m_CurrentCamera);
	m_aCamera[CameraID]->Update();
	m_App.GetGfxPack().m_DX.SetViewMtx(m_aCamera[CameraID]->GetViewMtx());
	m_App.GetGfxPack().m_DX.SetProjectionMtx(m_aCamera[CameraID]->GetProjMtx());

	//行列情報をバインド(GPU側を更新)
	m_App.GetGfxPack().m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::CB_VS_MTX_VP);
}

//カメラセット
void CAMERA_MGR::SetCamera(CAMERA_ID id) noexcept
{
	m_CurrentCamera = id;
}

//ワールド行列取得
dx::XMFLOAT4X4 CAMERA_MGR::GetWorldMtx(CAMERA_ID id) const noexcept
{
	return m_aCamera[static_cast<int>(id)]->GetWorldMtx();
}

//回転情報取得
dx::XMFLOAT3 CAMERA_MGR::GetRotation(CAMERA_ID id) const noexcept
{
	return m_aCamera[static_cast<int>(id)]->GetRotation();
}
