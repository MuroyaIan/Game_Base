
//===== インクルード部 =====
#include <Geometry/Debug/Mesh_Light.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <GraphicApp/Drawer/vsdPlane.h>
#include <Camera/CameraMgr.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== クラス実装 =====
MESH_LIGHT::MESH_LIGHT(GFX_PACK& Gfx, CAMERA_MGR& CameraMgr) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_CameraMgr(CameraMgr), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum), m_aMtxData(m_InstanceNum)
{
	//頂点情報作成
	VS_DATA<VERTEX> Model = std::move(VSD_PLANE::MakeData<VERTEX>());
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS定数バッファ作成（カメラ）
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MTX_VP, &cbData);

	//PS定数バッファ作成（ポリゴン色）
	const dx::XMFLOAT4 cbColor(1.0f, 1.0f, 0.0f, 0.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(m_Gfx.m_DX, cbColor, &cbData, false, true));

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

MESH_LIGHT::~MESH_LIGHT() noexcept
{
}

//更新処理
void MESH_LIGHT::Update() noexcept
{
	//ビルボード変換
	dx::XMFLOAT4X4 mtxCam = m_CameraMgr.GetWorldMtx();
	dx::XMFLOAT4X4 mtxR{
		mtxCam._11, mtxCam._12, mtxCam._13, 0.0f,
		mtxCam._21, mtxCam._22, mtxCam._23, 0.0f,
		mtxCam._31, mtxCam._32, mtxCam._33, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	dx::XMMATRIX mtxRot = dx::XMLoadFloat4x4(&mtxR);	//板ポリの向きはZ(-1.0f)の為、回転行列の逆転はなし

	//ワールド行列更新
	for (int i = 0; i < m_InstanceNum; i++){

		//ワールド行列を更新
		dx::XMMATRIX mtx = dx::XMMatrixScaling(m_aMtxData[i].m_Scale.x, m_aMtxData[i].m_Scale.y, m_aMtxData[i].m_Scale.z)
			* mtxRot
			* dx::XMMatrixTranslation(m_aMtxData[i].m_Pos.x, m_aMtxData[i].m_Pos.y, m_aMtxData[i].m_Pos.z);
		dx::XMStoreFloat4x4(&m_aMtxWorld[i], mtx);
	}
}

//書込み処理
void MESH_LIGHT::Draw(int InstanceNum) const noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<dx::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxWorld, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス描画
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_INSTANCE);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_INSTANCE);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_TRI);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int MESH_LIGHT::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(std::move(mtx));
	m_aMtxData.push_back(MTX_DATA());

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxWorld, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
