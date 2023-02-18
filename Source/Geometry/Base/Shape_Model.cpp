
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
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));
	TEX_LOADER::TEX_DATA& NullImage = m_Gfx.m_TextureMgr.GetTexPack(TEXTURE_MGR::TEX_ID::TEX_Null).TexData;		//空画像
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Displacement)] = NullImage;
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	for (auto& d : aData)
		d.pImageData = nullptr;

	//マテリアル情報初期化
	m_Material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.Shininess = 30.0f;
}

SHAPE_MODEL::~SHAPE_MODEL() noexcept
{
}

//更新処理
void SHAPE_MODEL::Update() noexcept
{
}

//書込み処理
void SHAPE_MODEL::Draw(int InstanceNum) const noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<VSD_INSTANCE> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aInstData, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス描画
	m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int SHAPE_MODEL::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	VSD_INSTANCE InstData{};
	//InstData.MaterialColor = {
	//	RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
	//	RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
	//	RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
	//	1.0f
	//};
	m_aInstanceData.push_back(std::move(InstData));

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
