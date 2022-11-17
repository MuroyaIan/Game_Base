
//===== インクルード部 =====
#include <Camera/Camera_FP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== グローバル定数宣言 =====
constexpr dx::XMFLOAT3 START_POS = { 0.0f, 0.0f, -20.0f };	//カメラ位置
constexpr dx::XMFLOAT3 START_LOOK = { 0.0f, 0.0f, 0.0f };	//注視点位置
constexpr dx::XMFLOAT3 START_UP = { 0.0f, 1.0f, 0.0f };		//アップベクトル

constexpr short START_FOV = 60;							//視野角
constexpr float START_NEAR_Z = 0.5f;
constexpr float START_FAR_Z = 100.0f;					//クリップ距離

constexpr float AXIS_X_LIMIT = fPI * 0.5f - 0.001f;		//垂直回転角度制限
constexpr float CAM_SPEED = 0.25f;						//カメラ移動速度

//===== クラス実装 =====
CAMERA_FP::CAMERA_FP(APP_64& App) noexcept : CAMERA(App), m_Input(m_App.GetInputMgr())
{
	//親メンバ初期化
	m_Pos = START_POS;
	m_LookAt = START_LOOK;
	m_vUp = START_UP;
	m_FOV = START_FOV;
	m_NearZ = START_NEAR_Z;
	m_FarZ = START_FAR_Z;
	m_Sensitivity = 0.2f;

	//行列初期化
	CAMERA::InitMatrix();
}

CAMERA_FP::~CAMERA_FP() noexcept
{
}

//更新処理
void CAMERA_FP::Update() noexcept
{
	//回転制御
	DirectX::XMINT2 MouseMove = m_Input.m_Mouse.GetMoveVal();
	m_Rot.x += gMath::GetRad(MouseMove.y * m_Sensitivity);
	m_Rot.y += gMath::GetRad(MouseMove.x * m_Sensitivity);

	//回転角度制限
	if (m_Rot.x > AXIS_X_LIMIT)
		m_Rot.x = AXIS_X_LIMIT;
	else if (m_Rot.x < -AXIS_X_LIMIT)
		m_Rot.x = -AXIS_X_LIMIT;
	if (m_Rot.y > fPI)
		m_Rot.y -= f2PI;
	else if (m_Rot.y < -fPI)
		m_Rot.y += f2PI;

	//移動制御
	float CamSpeed = CAM_SPEED;
	if (m_Input.m_KB.GetPress(VK_SPACE))
		CamSpeed *= 2.0f;
	if (m_Input.m_KB.GetPress(VK_W)) {
		m_Pos.x += m_mtxWorld._31 * CamSpeed;
		m_Pos.y += m_mtxWorld._32 * CamSpeed;
		m_Pos.z += m_mtxWorld._33 * CamSpeed;
	}
	else if (m_Input.m_KB.GetPress(VK_S)) {
		m_Pos.x -= m_mtxWorld._31 * CamSpeed;
		m_Pos.y -= m_mtxWorld._32 * CamSpeed;
		m_Pos.z -= m_mtxWorld._33 * CamSpeed;
	}
	if (m_Input.m_KB.GetPress(VK_A)) {
		m_Pos.x -= m_mtxWorld._11 * CamSpeed;
		m_Pos.y -= m_mtxWorld._12 * CamSpeed;
		m_Pos.z -= m_mtxWorld._13 * CamSpeed;
	}
	else if (m_Input.m_KB.GetPress(VK_D)) {
		m_Pos.x += m_mtxWorld._11 * CamSpeed;
		m_Pos.y += m_mtxWorld._12 * CamSpeed;
		m_Pos.z += m_mtxWorld._13 * CamSpeed;
	}
	if (m_Input.m_KB.GetPress(VK_LSHIFT))
		m_Pos.y += CamSpeed;
	else if (m_Input.m_KB.GetPress(VK_LCONTROL))
		m_Pos.y -= CamSpeed;
	if (m_Input.m_KB.GetTrigger(VK_R))
		ResetCamera();

	//行列更新
	UpdateMatrix();
}

//カメラリセット
void CAMERA_FP::ResetCamera() noexcept
{
	//データ初期化
	m_Pos = START_POS;
	m_Rot.x = 0.0f;
	m_Rot.y = 0.0f;
}

//行列更新
void CAMERA_FP::UpdateMatrix() noexcept
{
	//ワールド行列
	dx::XMStoreFloat4x4(&m_mtxWorld,
		dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z)
		* dx::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z)
	);

	//ビュー行列
	m_LookAt = { m_mtxWorld._41 + m_mtxWorld._31, m_mtxWorld._42 + m_mtxWorld._32, m_mtxWorld._43 + m_mtxWorld._33 };
	m_vUp = { m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23 };
	dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixLookAtLH(
		dx::XMVectorSet(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f),
		dx::XMVectorSet(m_LookAt.x, m_LookAt.y, m_LookAt.z, 0.0f),
		dx::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f))
	);
}
