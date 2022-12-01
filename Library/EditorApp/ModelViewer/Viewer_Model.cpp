
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Viewer_Model.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Tool/gMath.h>
#include <EditorApp/ModelViewer/Viewer.h>
#include <Tool/Input/InputMgr.h>

namespace dx = DirectX;

//===== クラス実装 =====
VIEWER_MODEL::VIEWER_MODEL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex, INPUT_MGR& Input) :
	DRAWER_EX(Gfx), m_ShaderMgr(ShaderMgr), m_Viewer(Viewer), m_Loader(Loader), m_MeshIndex(MeshIndex), m_MtxLocal(), m_MtxWorld(), m_Material(), m_bNoBone(false),
	m_pMtxBone(), m_bDrawAnimation(m_Viewer.GetFlag_DrawAnimation()), m_AnimationID(m_Viewer.GetAnimationID()), m_AnimFrame(0), m_FrameCnt(0), m_AnimPause(m_Viewer.GetFlag_AnimPause()),
	m_Input(Input), m_Scale(Viewer.GetModelScale()), m_Rot()
{
	//頂点情報作成
	VS_DATA<VERTEX_MB> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//定数バッファ作成（変換行列）
	AddBind(std::make_unique<CB_MTX_LWVP>(Gfx, *this, m_MtxLocal));

	//定数バッファ作成（骨情報）
	m_pMtxBone = std::make_unique<CBD_BONE>();
	AddBind(std::make_unique<CB_BONE>(Gfx, *m_pMtxBone, true));

	//定数バッファ作成（マテリアル）
	AddBind(std::make_unique<CB_MATERIAL>(Gfx, m_Material));

	//テクスチャバッファ作成
	auto& MeshData = m_Loader.GetMesh(m_MeshIndex);
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MAX_TYPE));
	if (MeshData.aTex_Diffuse.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Diffuse[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::DIFFUSE)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::DIFFUSE)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	if (MeshData.aTex_Specular.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Specular[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::SPECULAR)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::SPECULAR)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::NORMAL)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	AddBind(std::make_unique<TEXTURE_MODEL>(Gfx, aData));

	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);

	//ローカル行列初期化
	dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());

	//ワールド行列初期化
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());

	//マテリアル情報初期化
	m_Material = MeshData.MaterialData;

	//メッシュ情報初期化
	if (MeshData.aNoSkinData.size() > 0)
		m_bNoBone = true;
}

VIEWER_MODEL::~VIEWER_MODEL() noexcept
{
}

//更新処理
void VIEWER_MODEL::Update() noexcept
{
	//回転制御
	if (m_Input.m_KB.GetPress(VK_A))
		m_Rot.y += gMath::GetRad(2);
	else if (m_Input.m_KB.GetPress(VK_D))
		m_Rot.y -= gMath::GetRad(2);
	if (m_Input.m_KB.GetPress(VK_W))
		m_Rot.x += gMath::GetRad(2);
	else if (m_Input.m_KB.GetPress(VK_S))
		m_Rot.x -= gMath::GetRad(2);
	if (m_Input.m_KB.GetPress(VK_R)) {
		m_Rot.x = 0.0f;
		m_Rot.y = 0.0f;
	}

	//ワールド行列更新
	dx::XMMATRIX mtx = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale)
		* dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	dx::XMStoreFloat4x4(&m_MtxWorld, mtx);

	//骨情報更新
	if (m_bDrawAnimation)
		UpdateBoneData(m_AnimationID);
	else {

		//骨情報リセット
		for (auto& m : m_pMtxBone->mtxBone)
			dx::XMStoreFloat4x4(&m, dx::XMMatrixIdentity());

		//ローカル行列リセット
		dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());
	}
}

void VIEWER_MODEL::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind_Model();
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::SAMPLER);
	DRAWER::Draw(InstanceNum);
}

//頂点情報作成
VS_DATA<VERTEX_MB> VIEWER_MODEL::MakeData_VS() const noexcept
{
	VS_DATA<VERTEX_M>& DataRef = m_Loader.GetMesh(m_MeshIndex).vsData;
	VS_DATA<VERTEX_MB> vsData;
	vsData.m_Indices = DataRef.m_Indices;		//インデックス
	for (auto& v : DataRef.m_Vertices) {		//頂点情報

		//頂点情報
		VERTEX_MB Vertex;
		Vertex.m_Pos = v.m_Pos;
		Vertex.m_Normal = v.m_Normal;
		Vertex.m_UV = v.m_UV;
		vsData.m_Vertices.emplace_back(Vertex);
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
	return VS_DATA<VERTEX_MB>(std::move(vsData));
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

			//フレームカウントリセット
			if (m_FrameCnt > 0)
				m_FrameCnt = 0;
		}
		else {

			//30FPS
			m_FrameCnt++;
			if (m_FrameCnt > 1) {
				m_FrameCnt = 0;

				//60FPSと同じ処理
				m_AnimFrame++;
			}
		}
	}
	else
		m_AnimFrame = m_Viewer.GetAnimationFrame();
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
		m_MtxLocal = m_Loader.GetMesh(m_MeshIndex).aNoSkinData[AnimID].aMatrix[m_AnimFrame];
}
