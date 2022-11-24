
//===== インクルード部 =====
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
MESH::MESH(MODEL& ModelRef, int MeshIdx) :
	DRAWER_EX(), m_FileData(ModelRef.m_FileData), m_MeshIdx(MeshIdx),
	m_Gfx(ModelRef.m_Gfx), m_InstanceNum(ModelRef.m_InstanceNum), m_aInstanceData(ModelRef.m_aInstanceData), m_Material(),
	m_bStatic(ModelRef.m_bStatic), m_pLocalData(), m_AnimID(ModelRef.m_AnimID), m_AnimID_Backup(ModelRef.m_AnimID_Backup),
	m_AnimFrame(ModelRef.m_AnimFrame), m_AnimFrame_Backup(ModelRef.m_AnimFrame_Backup),
	m_bBlendAnim(ModelRef.m_bBlendAnim), m_BlendTimer(ModelRef.m_BlendTimer)
{
	//メッシュ情報確認
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];

	//頂点情報作成
	VS_DATA<VERTEX_MB> Model = Mesh.vsData;
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.InitSize(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//マテリアル情報作成
	m_Material = Mesh.MaterialData;
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, m_Material));

	//テクスチャバッファ作成
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MAX_TYPE));	//バッファ用配列
	TEX_LOADER::TEX_DATA NullImage = TEX_LOADER::LoadTexture("Asset/Texture/null.png");				//空画像
	std::string Tex_D = Mesh.Tex_D;																	//Diffuseテクスチャ名
	if (Tex_D.size() > 0) {
		for (auto& t : m_FileData.m_aTexPack) {
			if (t.Name == Tex_D) {
				aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::DIFFUSE)] = t.TexData;
				break;
			}
		}
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::DIFFUSE)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::SPECULAR)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::NORMAL)] = NullImage;
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	TEX_LOADER::ReleaseTexture(NullImage.pImageData);

	//ローカル情報作成
	m_pLocalData = std::make_unique<CBD_MTX_LOCAL>();
	AddBind(std::make_unique<CB_LOCAL>(m_Gfx.m_DX, *m_pLocalData));
}

MESH::~MESH() noexcept
{
}

//更新処理
void MESH::Update() noexcept
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//ローカル情報更新
	if (!m_bStatic) {
		if (m_FileData.aMesh[m_MeshIdx].aNoSkin.size() > 0)
			m_pLocalData->mtxSkin = m_FileData.aMesh[m_MeshIdx].aNoSkin[m_AnimID].aMatrix[m_AnimFrame];
	}
}

//書込み処理
void MESH::Draw(GRAPHIC& Gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<INSTANCE_DATA> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(Gfx, aInstData, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//インスタンス描画
	if (m_bStatic)
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	else
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong_Anim();
	(void)bDrawInstance;
	DRAWER::Draw(Gfx, true);
}

//インスタンス追加
int MESH::AddInstance()
{
	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aInstanceData, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return 0;
}
