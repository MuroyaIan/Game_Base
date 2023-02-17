
//===== インクルード部 =====
#include <Geometry/Shape_Tex.h>
#include <GraphicApp/Binder/BinderRef.h>

#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
SHAPE_TEX::SHAPE_TEX(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type, TEXTURE_MGR::TEX_ID Tex) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum), m_Tex(Tex)
{
	//頂点情報作成
	VS_DATA<VERTEX_T> Model = VSD_MAKER::MakeData_Tex(m_Type);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS定数バッファ作成（カメラ）
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//テクスチャ初期化
	m_Gfx.m_TextureMgr.SetTextureOn(m_Tex);
}

SHAPE_TEX::~SHAPE_TEX() noexcept
{
	//テクスチャ終了
	m_Gfx.m_TextureMgr.SetTextureOff(m_Tex);
}

//更新処理
void SHAPE_TEX::Update() noexcept
{
}

//書込み処理
void SHAPE_TEX::Draw(int InstanceNum) const noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<DirectX::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxWorld, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス描画
	m_Gfx.m_TextureMgr.Bind(m_Tex);
	m_Gfx.m_ShaderMgr.Bind_Instance_Texture();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int SHAPE_TEX::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(mtx);

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
