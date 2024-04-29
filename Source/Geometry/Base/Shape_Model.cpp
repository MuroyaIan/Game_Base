
//===== インクルード部 =====
#include <Geometry/Base/Shape_Model.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
SHAPE_MODEL::SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_Material()
{
	//頂点情報作成
	VS_DATA<VERTEX_M> Model = VSD_MAKER::MakeData_Model(m_Type);
	for (auto& v : Model.m_Vertices) {	//座標系変換（右手系へ）
		v.m_Pos.x *= -1.0f;
		v.m_Normal.x *= -1.0f;
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS定数バッファ作成（カメラ）
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS定数バッファ作成（マテリアル）
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, &cbData, m_Material));

	//VS・PS定数バッファ作成（ライト）
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_Light, &cbData);

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//テクスチャバッファ作成
	SRV_PTR SrvData;
	for (int i = 0, Cnt = static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType); i < Cnt; i++)
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));	//空画像
	AddBind(std::make_unique<SRV_MGR>(SrvData));

	//マテリアル情報初期化
	m_Material.ms_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.ms_Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.ms_Shininess = 30.0f;
}

SHAPE_MODEL::~SHAPE_MODEL() noexcept
{
	//テクスチャバッファ解放
	for (int i = 0, Cnt = static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType); i < Cnt; i++)
		m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
}

//更新処理
void SHAPE_MODEL::Update()
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<VSD_INSTANCE> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aInstData, VERTEX_BUFFER::VB_TYPE::Instance);
}

//描画処理
void SHAPE_MODEL::Draw(int InstanceNum) noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス描画
	m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int SHAPE_MODEL::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	m_aInstanceData.push_back(VSD_INSTANCE{});

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
