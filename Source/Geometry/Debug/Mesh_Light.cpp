
//===== インクルード部 =====
#include <Geometry/Debug/Mesh_Light.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <GraphicApp/Drawer/vsdSphere.h>

namespace dx = DirectX;

//===== クラス実装 =====
MESH_LIGHT::MESH_LIGHT(GFX_PACK& Gfx) :
	DRAWER_EX(), m_Gfx(Gfx), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum), m_aMtxData(m_InstanceNum)
{
	//頂点情報作成
	VS_DATA<VERTEX> Model = std::move(VSD_SPHERE::MakeData<VERTEX>());
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//静的初期化
	if (!StaticIsInit()) {

		//インデックス情報作成
		AddStaticBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

		//定数バッファ作成（ポリゴン色）
		const dx::XMFLOAT4 cbColor(1.0f, 1.0f, 0.0f, 0.0f);
		AddStaticBind(std::make_unique<PS_CBUFFER<dx::XMFLOAT4>>(m_Gfx.m_DX, cbColor));
	}
}

MESH_LIGHT::~MESH_LIGHT() noexcept
{
}

//更新処理
void MESH_LIGHT::Update() noexcept
{
	//ワールド行列更新
	for (int i = 0; i < m_InstanceNum; i++){
		dx::XMMATRIX mtx = dx::XMMatrixScaling(m_aMtxData[i].m_Scale.x, m_aMtxData[i].m_Scale.y, m_aMtxData[i].m_Scale.z)
			* dx::XMMatrixTranslation(m_aMtxData[i].m_Pos.x, m_aMtxData[i].m_Pos.y, m_aMtxData[i].m_Pos.z);
		dx::XMStoreFloat4x4(&m_aMtxWorld[i], mtx);
	}
}

//書込み処理
void MESH_LIGHT::Draw(GRAPHIC& Gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<DirectX::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(Gfx, aMtxWorld, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//インスタンス描画
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_INSTANCE);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_INSTANCE);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_TRI);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	(void)bDrawInstance;
	DRAWER::Draw(Gfx, true);
}

//インスタンス追加
int MESH_LIGHT::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(mtx);
	m_aMtxData.push_back(MTX_DATA());

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxWorld, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
