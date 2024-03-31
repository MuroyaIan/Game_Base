
//===== インクルード部 =====
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
MESH::MESH(MODEL& ModelRef, int MeshIdx) :
	DRAWER(ModelRef.m_Gfx.m_DX), m_Gfx(ModelRef.m_Gfx),
	m_InstanceNum(ModelRef.m_InstanceNum), m_aInstanceData(ModelRef.m_aInstanceData), m_Material(),
	m_ModelRef(ModelRef), m_FileData(ModelRef.m_FileData), m_MeshIdx(MeshIdx),
	m_bStatic(ModelRef.m_bStatic), m_NoSkinBuffPtr()
{
	//メッシュ情報確認
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];

	//頂点情報作成
	VS_DATA<VERTEX_MNB> vsdModel = Mesh.vsData;
	if (!m_bStatic && Mesh.aNoSkin.size() == 0) {

		//骨あり
		VS_DATA<VERTEX_MB> Model;
		Model.m_Vertices.resize(vsdModel.m_Indices.size());
		for (size_t i = 0, Cnt = Model.m_Vertices.size(); i < Cnt; i++) {
			Model.m_Vertices[i].m_Pos = vsdModel.m_Vertices[i].m_Pos;
			Model.m_Vertices[i].m_UV = vsdModel.m_Vertices[i].m_UV;
			Model.m_Vertices[i].m_Normal = vsdModel.m_Vertices[i].m_Normal;
			for (int j = 0; j < 4; j++) {
				Model.m_Vertices[i].m_BoneID[j] = vsdModel.m_Vertices[i].m_BoneID[j];
				Model.m_Vertices[i].m_BoneWeight[j] = vsdModel.m_Vertices[i].m_BoneWeight[j];
			}
		}
		AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));
	}
	else {

		//骨なし
		VS_DATA<VERTEX_M> Model;
		Model.m_Vertices.resize(vsdModel.m_Indices.size());
		for (size_t i = 0, Cnt = Model.m_Vertices.size(); i < Cnt; i++) {
			Model.m_Vertices[i].m_Pos = vsdModel.m_Vertices[i].m_Pos;
			Model.m_Vertices[i].m_UV = vsdModel.m_Vertices[i].m_UV;
			Model.m_Vertices[i].m_Normal = vsdModel.m_Vertices[i].m_Normal;
		}
		AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));
	}

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, vsdModel.m_Indices));

	//VS定数バッファ作成（カメラ）
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS定数バッファ作成（マテリアル）
	m_Material = Mesh.MaterialData;
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, &cbData, m_Material));

	//VS・PS定数バッファ作成（ライト）
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_Light, &cbData);

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//VSテクスチャバッファ作成
	SRV_PTR SrvData;
	if (!m_bStatic) {

		//アニメーション行列
		auto& NoSkinRef = Mesh.aNoSkin;
		if (NoSkinRef.size() > 0) {

			//骨情報を読込
			std::vector<dx::XMFLOAT4X4> aMtxBone(0);
			for (auto& ad : NoSkinRef) {
				for (auto& md : ad.aMatrix)
					aMtxBone.push_back(md);
			}

			//テクスチャ作成用データ
			TEX_DATA_ANIM AnimData;
			AnimData.pAnimData = &aMtxBone.data()->_11;
			for (auto& f : m_FileData.aAnimFrame)
				AnimData.nWidth += f;
			AnimData.nHeight = 1;

			//バッファ作成
			m_NoSkinBuffPtr = std::make_unique<TEXTURE_ANIM>(m_Gfx.m_DX, AnimData);
			SrvData.m_aSrvPtrVS.push_back(m_NoSkinBuffPtr->GetSrvPtr());					//骨なし
		}
		else
			SrvData.m_aSrvPtrVS.push_back(m_Gfx.m_ModelMgr.SetAnimTexOn(m_ModelRef.m_ID));	//骨あり
	}

	//PSテクスチャバッファ作成
	if (Mesh.Tex_D.size() > 0)
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_ModelMgr.SetTextureOn(m_ModelRef.m_ID, Mesh.Tex_D));		//Diffuseテクスチャ
	else
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));	//空画像
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	AddBind(std::make_unique<SRV_MGR>(SrvData));
}

MESH::~MESH() noexcept
{
	//テクスチャバッファ解放
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];
	if (Mesh.Tex_D.size() > 0)
		m_Gfx.m_ModelMgr.SetTextureOff(m_ModelRef.m_ID, Mesh.Tex_D);
	else
		m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
}

//更新処理
void MESH::Update()
{
	//インスタンス更新
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aInstanceData, VERTEX_BUFFER::VB_TYPE::Instance);
}

//描画処理
void MESH::Draw(int InstanceNum) noexcept
{
	//例外処理
	(void)InstanceNum;

	//インスタンス描画
	if (m_bStatic)
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong();	//アニメーションなし
	else {

		//アニメーションあり
		if (m_NoSkinBuffPtr == nullptr) {

			//骨あり
			m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_Instance_Phong_Anim);
			m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_Instance_Phong_Anim);
		}
		else {

			//骨なし
			m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_Instance_Phong_Anim_NoSkin);
			m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_Instance_Phong_Anim_NoSkin);
		}
		m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_Tri);
		m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::Sampler);
		m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_Phong);
	}
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int MESH::AddInstance()
{
	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return 0;
}
