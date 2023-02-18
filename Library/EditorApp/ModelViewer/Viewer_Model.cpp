
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Viewer_Model.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>
#include <Light/LightMgr.h>

namespace dx = DirectX;

//===== クラス実装 =====
VIEWER_MODEL::VIEWER_MODEL(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx.m_DX), m_ShaderMgr(Gfx.m_ShaderMgr), m_Viewer(Viewer), m_Loader(Loader), m_MeshIndex(MeshIndex), m_mtxLocal(), m_mtxWorld(), m_Material(), m_bNoBone(false),
	m_pMtxBone(), m_bDrawAnimation(m_Viewer.GetFlag_DrawAnimation()), m_AnimationID(m_Viewer.GetAnimationID()), m_AnimFrame(0), m_FrameCnt(0), m_AnimPause(m_Viewer.GetFlag_AnimPause()),
	m_Scale(Viewer.GetModelScale()), m_RotY(Viewer.GetModelRotation()), m_LightPos(Viewer.GetLightPos()), bUseNormalMap(false), bUseDispMap(false)
{
	//頂点情報作成
	VS_DATA<VERTEX_MNB> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx, Model.m_Vertices));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx, Model.m_Indices));

	//VS定数バッファ作成（変換行列）
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_LWVP>(m_Gfx, &cbData, *this, m_mtxLocal));

	//VS定数バッファ作成（骨情報）
	m_pMtxBone = std::make_unique<CBD_BONE>();
	AddBind(std::make_unique<CB_BONE>(m_Gfx, &cbData, *m_pMtxBone, true));

	//PS定数バッファ作成（マテリアル）
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx, &cbData, m_Material));

	//VS・PS定数バッファ作成（ライト）
	m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_Light, &cbData);

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//テクスチャデータ読込
	auto& MeshData = m_Loader.GetMesh(m_MeshIndex);
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));

	//Diffuseマップ
	if (MeshData.aTex_Diffuse.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Diffuse[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//Specularマップ
	if (MeshData.aTex_Specular.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Specular[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//Normalマップ
	if (MeshData.aTex_Normal.size() > 0) {
		bUseNormalMap = true;
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Normal[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//Displacementマップ
	if (MeshData.aTex_Displacement.size() > 0) {
		bUseDispMap = true;
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Displacement[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Displacement)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Displacement)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//テクスチャバッファ作成
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx, aData));
	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);	//データ解放

	//ローカル行列初期化
	dx::XMStoreFloat4x4(&m_mtxLocal, dx::XMMatrixIdentity());

	//ワールド行列初期化
	dx::XMStoreFloat4x4(&m_mtxWorld, dx::XMMatrixIdentity());

	//マテリアル情報初期化
	m_Material = MeshData.MaterialData;

	//骨の影響を受けるか確認
	if (MeshData.aNoSkinData.size() > 0)
		m_bNoBone = true;

	//ライト初期化
	m_Viewer.GetLightMgr().GetData().DirectionalLight.Color_D = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Viewer.GetLightMgr().GetData().DirectionalLight.Pos = { 1.0f, 1.0f, 1.0f, 0.0f };
}

VIEWER_MODEL::~VIEWER_MODEL() noexcept
{
	//ライトリセット
	m_Viewer.GetLightMgr().GetData().DirectionalLight.Color_D = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_Viewer.GetLightMgr().GetData().DirectionalLight.Pos = { 0.0f, 1.0f, 0.0f, 1.0f };
}

//更新処理
void VIEWER_MODEL::Update() noexcept
{
	//ワールド行列更新
	dx::XMMATRIX mtx = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale)
		* dx::XMMatrixRotationRollPitchYaw(0.0f, m_RotY, 0.0f);
	dx::XMStoreFloat4x4(&m_mtxWorld, mtx);

	//骨情報更新
	if (m_bDrawAnimation)
		UpdateBoneData(m_AnimationID);	//アニメーション再生中⇒情報更新
	else {

		//骨情報リセット
		for (auto& m : m_pMtxBone->mtxBone)
			dx::XMStoreFloat4x4(&m, dx::XMMatrixIdentity());

		//ローカル行列リセット
		dx::XMStoreFloat4x4(&m_mtxLocal, dx::XMMatrixIdentity());
	}

	//マテリアル更新
	if (m_Viewer.GetSpecularFlag())
		m_Material.Specular = m_Loader.GetMesh(m_MeshIndex).MaterialData.Specular;
	else
		m_Material.Specular = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_Material.Disp_MinLayerNum = m_Viewer.GetDispCoef().x;
	m_Material.Disp_MaxLayerNum = m_Viewer.GetDispCoef().y;
	m_Material.Disp_DepthScale = m_Viewer.GetDispCoef().z;										//視差マッピングの調整値を更新
	m_Loader.GetMesh(m_MeshIndex).MaterialData.Disp_MinLayerNum = m_Material.Disp_MinLayerNum;
	m_Loader.GetMesh(m_MeshIndex).MaterialData.Disp_MaxLayerNum = m_Material.Disp_MaxLayerNum;
	m_Loader.GetMesh(m_MeshIndex).MaterialData.Disp_DepthScale = m_Material.Disp_DepthScale;	//データをモデル側にも更新

	//ライト制御
	m_Viewer.GetLightMgr().GetData().DirectionalLight.Pos = { m_LightPos.x, m_LightPos.y, m_LightPos.z, 0.0f };
}

//描画処理
void VIEWER_MODEL::Draw(int InstanceNum) const noexcept
{
	//その他のバインド
	if (bUseNormalMap &&
		m_Viewer.GetNormalMapFlag()) {
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_Model_Normal);
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_Model_Normal);

		//視差マップあり・なし
		if (bUseDispMap &&
			m_Viewer.GetDispMapFlag())
			m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_Model_Disp);
		else
			m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_Model_Normal);
	}
	else {

		//ノーマルマップなし
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_Model);
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_Model);
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_Model);
	}
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_Tri);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::Sampler);

	//描画
	DRAWER::Draw(InstanceNum);
}

//頂点情報作成
VS_DATA<VERTEX_MNB> VIEWER_MODEL::MakeData_VS() const noexcept
{
	VS_DATA<VERTEX_MN>& DataRef = m_Loader.GetMesh(m_MeshIndex).vsData;
	VS_DATA<VERTEX_MNB> vsData;
	vsData.m_Indices = DataRef.m_Indices;		//インデックス
	for (auto& v : DataRef.m_Vertices) {		//頂点情報

		//頂点情報
		VERTEX_MNB Vertex;
		Vertex.m_Pos = v.m_Pos;
		Vertex.m_UV = v.m_UV;
		Vertex.m_Normal = v.m_Normal;
		Vertex.m_Binormal = v.m_Binormal;
		Vertex.m_Tangent = v.m_Tangent;
		vsData.m_Vertices.push_back(std::move(Vertex));
	}

	//骨比重
	for (auto& idx : m_Loader.GetMesh(m_MeshIndex).aIndexBuffer) {	//頂点バッファインデックス配列

		//対応する頂点へ反映
		size_t BoneNum = idx.aBoneID.size();	//頂点ごとの骨の数
		for (auto& idxNum : idx.aIndex) {

			//骨が影響しない場合
			if (BoneNum == 0) {
				vsData.m_Vertices[idxNum].m_BoneWeight[0] = 1.0f;
				continue;
			}

			//骨比重を格納
			for (size_t k = 0; k < 4; k++) {	//4つ以上の骨を破棄
				if (k >= BoneNum)				//例外処理（骨が4つ未満の場合）
					break;
				vsData.m_Vertices[idxNum].m_BoneID[k] = idx.aBoneID[k];
				vsData.m_Vertices[idxNum].m_BoneWeight[k] = idx.aWeight[k];
			}
		}
	}
	return VS_DATA<VERTEX_MNB>(std::move(vsData));
}

//骨情報更新
void VIEWER_MODEL::UpdateBoneData(int AnimID) noexcept
{
	//再生フレーム数更新
	auto pAnimData = &m_Loader.GetAnimation()[AnimID];
	if (!m_AnimPause) {
		if (!pAnimData->bIsFPS_30) {

			//60FPS
			m_AnimFrame++;

			//フレームカウント制御
			if (m_FrameCnt > 0)
				m_FrameCnt = 0;
		}
		else {

			//フレームカウント制御
			m_FrameCnt++;
			if (m_FrameCnt > 1) {
				m_FrameCnt = 0;

				//30FPS
				m_AnimFrame++;
			}
		}
	}
	else
		m_AnimFrame = m_Viewer.GetAnimationFrame();		//アニメーション一時停止⇒フレーム位置を保持

	//再生フレーム数の範囲制御
	if (m_AnimFrame < pAnimData->StartFrame)
		m_AnimFrame = pAnimData->StartFrame;
	if (m_AnimFrame >= pAnimData->StopFrame)
		m_AnimFrame = pAnimData->StartFrame;
	m_Viewer.GetAnimationFrame() = m_AnimFrame;			//ビューワ情報へ反映

	//骨行列更新
	auto pMtxBone = &m_pMtxBone->mtxBone[0];
	auto pMtxBoneRef = &m_Loader.GetSkin()[AnimID].aFrameData[0];
	for (size_t i = 0, Cnt = m_Loader.GetSkin()[AnimID].aFrameData.size(); i < Cnt; i++) {
		if (pMtxBoneRef->aMatrix.size() > 0)
			*pMtxBone = pMtxBoneRef->aMatrix[m_AnimFrame];
		pMtxBone++;
		pMtxBoneRef++;
	}

	//ローカル行列更新（骨なしメッシュ）
	if (m_bNoBone)
		m_mtxLocal = m_Loader.GetMesh(m_MeshIndex).aNoSkinData[AnimID].aMatrix[m_AnimFrame];
}
