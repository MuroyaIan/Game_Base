
//===== インクルード部 =====
#include <Camera/Camera.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== グローバル定数宣言 =====
constexpr dx::XMFLOAT3 START_POS = { 0.0f, 0.0f, -10.0f };	//カメラ位置
constexpr dx::XMFLOAT3 START_LOOK = { 0.0f, 0.0f, 0.0f };	//注視点位置
constexpr dx::XMFLOAT3 START_UP = { 0.0f, 1.0f, 0.0f };		//アップベクトル

constexpr short START_FOV = 60;			//視野角
constexpr float START_NEAR_Z = 0.5f;
constexpr float START_FAR_Z = 40.0f;	//クリップ距離

//===== クラス実装 =====
CAMERA::CAMERA(APP& App) noexcept : m_App(App),
	m_Pos(START_POS), m_LookAt(START_LOOK), m_vUp(START_UP),
	m_FOV(START_FOV), g_AspectRatio(), m_NearZ(START_NEAR_Z), m_FarZ(START_FAR_Z),
	m_mtxWorld(), m_mtxView(), m_mtxProj(), m_Rot(0.0f, 0.0f, 0.0f), m_Sensitivity(1.0f)
{
	//縦横比初期化
	auto [x, y] = m_App.GetWindowProc().GetSize();
	g_AspectRatio = static_cast<float>(x) / static_cast<float>(y);

	//行列初期化
	InitMatrix();
}

CAMERA::~CAMERA() noexcept
{
}

//行列初期化
void CAMERA::InitMatrix() noexcept
{
	//ワールド行列
	dx::XMStoreFloat4x4(&m_mtxWorld, dx::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z));

	//ビュー行列
	dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixLookAtLH(
		dx::XMVectorSet(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f),
		dx::XMVectorSet(m_LookAt.x, m_LookAt.y, m_LookAt.z, 0.0f),
		dx::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f))
	);

	//投影行列
	dx::XMStoreFloat4x4(&m_mtxProj, dx::XMMatrixPerspectiveFovLH(gMath::GetRad(m_FOV), g_AspectRatio, m_NearZ, m_FarZ));
}

//投影行列更新
void CAMERA::UpdateProjMatrix() noexcept
{
	dx::XMStoreFloat4x4(&m_mtxProj, dx::XMMatrixPerspectiveFovLH(gMath::GetRad(m_FOV), g_AspectRatio, m_NearZ, m_FarZ));
}
