
//===== インクルード部 =====
#include <Geometry/Shape_Default.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== クラス実装 =====
SHAPE_DEFAULT::SHAPE_DEFAULT(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum)
{
	//頂点情報作成
	VS_DATA<VERTEX> Model = VSD_MAKER::MakeData_Default(m_Type);
	//dx::XMFLOAT4X4 mtx{};
	//dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	//Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS定数バッファ作成（カメラ）
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS定数バッファ作成（ポリゴン色）
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_PS_Default, &cbData);

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

SHAPE_DEFAULT::~SHAPE_DEFAULT() noexcept
{
}

//更新処理
void SHAPE_DEFAULT::Update() noexcept
{
}

//書込み処理
void SHAPE_DEFAULT::Draw(int InstanceNum) const noexcept
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
	m_Gfx.m_ShaderMgr.Bind_Instance();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int SHAPE_DEFAULT::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(std::move(mtx));

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
