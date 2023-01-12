
//===== インクルード部 =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== クラス実装 =====
MODEL::MODEL(GFX_PACK& Gfx, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(Gfx), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum),
	m_bStatic(true), m_pBoneBuffer(), m_pMtxBone(), m_AnimID(1), m_AnimID_Backup(m_AnimID), m_AnimFrame(0), m_AnimFrame_Backup(m_AnimFrame), m_FrameCnt(0), m_FrameCnt_Backup(m_FrameCnt),
	m_bBlendAnim(false), m_BlendTimer(0)
{
	//アニメーション確認
	if (m_FileData.aAnimFrame.size() > 0)
		m_bStatic = false;

	//メッシュ初期化
	for (size_t i = 0, Cnt = m_FileData.aMesh.size(); i < Cnt; i++)
		m_aMesh[i] = std::make_unique<MESH>(*this, static_cast<int>(i));

	//定数バッファ作成（骨情報）
	if (!m_bStatic) {
		m_pMtxBone = std::make_unique<CBD_BONE>();
		m_pBoneBuffer = std::make_unique<CB_BONE>(m_Gfx.m_DX, nullptr, *m_pMtxBone);
	}
}

MODEL::~MODEL() noexcept
{
}

//更新処理
void MODEL::Update() noexcept
{
	//アニメーション更新
	if (!m_bStatic) {

		if (!m_bBlendAnim)
			UpdateAnimation();
		else
			UpdateAnimationBlending();	//ブレンド処理（0.2s秒間）
	}

	//ワールド行列更新
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		m_aInstanceData[i].MtxWorld = m_FileData.InitMtxWorld;
		dx::XMFLOAT4X4 mtxW{};
		dx::XMStoreFloat4x4(&mtxW, dx::XMMatrixTranslation((i % 10) * 10.0f, 0.0f, (i / 10) * 10.0f));
		gMath::MtxMultiply4x4_AVX(&m_aInstanceData[i].MtxWorld._11, &mtxW._11);
	}

	//メッシュ更新
	for (auto& m : m_aMesh)
		m->Update();
}

//書込み処理
void MODEL::Draw() const noexcept
{
	//骨情報をバインド
	if (!m_bStatic)
		m_pBoneBuffer->Bind(m_Gfx.m_DX);

	//メッシュ描画
	for (auto& m : m_aMesh)
		m->Draw();
}

//インスタンス追加
int MODEL::AddInstance()
{
	m_InstanceNum++;

	//配列追加
	VSD_INSTANCE InstData{};
	InstData.MtxWorld = m_FileData.InitMtxWorld;
	m_aInstanceData.push_back(InstData);

	//メッシュのインスタンスを追加
	for (auto& m : m_aMesh)
		m->AddInstance();

	//インスタンスのインデックスを返す
	return m_InstanceNum - 1;
}

//ポリゴン数取得
UINT MODEL::GetPolygonNum() const noexcept
{
	UINT PolygonNum = 0;
	for (auto& m : m_aMesh)
		PolygonNum += m->GetPolygonNum();
	return PolygonNum;
}

//アニメーション更新
void MODEL::UpdateAnimation() noexcept
{
	//フレーム更新
	if (!m_FileData.aIsFPS_30[m_AnimID]) {

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

	//フレーム制御
	if (m_AnimFrame > m_FileData.aAnimFrame[m_AnimID] - 1)
		m_AnimFrame = 0;

	//骨情報更新
	auto pMtxBone = &m_pMtxBone->mtxBone[0];
	auto pMtxBoneRef = &m_FileData.aBone[0];
	for (size_t i = 0, Cnt = m_FileData.aBone.size(); i < Cnt; i++) {
		if (pMtxBoneRef->aSkin[m_AnimID].aMatrix.size() > 0)
			*pMtxBone = pMtxBoneRef->aSkin[m_AnimID].aMatrix[m_AnimFrame];
		pMtxBone++;
		pMtxBoneRef++;
	}
}

//アニメーションブレンド更新
void MODEL::UpdateAnimationBlending() noexcept
{
	m_BlendTimer++;

	//フレーム更新
	if (!m_FileData.aIsFPS_30[m_AnimID]) {

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

	//フレーム制御
	if (m_AnimFrame > m_FileData.aAnimFrame[m_AnimID] - 1)
		m_AnimFrame = 0;

	//ブレンド前アニメーションの更新
	float ratio = static_cast<float>(m_AnimFrame + 1) / m_FileData.aAnimFrame[m_AnimID];		//ブレンド後アニメーションのフレーム割合取得
	m_AnimFrame_Backup = static_cast<int>(m_FileData.aAnimFrame[m_AnimID_Backup] * ratio - 1);	//ブレンド前アニメーションのフレーム算出

	//骨情報更新
	auto pMtxBone = &m_pMtxBone->mtxBone[0];
	auto pMtxBoneRef = &m_FileData.aBone[0];
	for (size_t i = 0, Cnt = m_FileData.aBone.size(); i < Cnt; i++) {
		if (pMtxBoneRef->aSkin[m_AnimID].aMatrix.size() > 0) {
			//*pMtxBone = pMtxBoneRef->aSkin[m_AnimID].aMatrix[m_AnimFrame];
			dx::XMMATRIX mtx1 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[m_AnimID].aMatrix[m_AnimFrame]);
			dx::XMMATRIX mtx2 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[m_AnimID_Backup].aMatrix[m_AnimFrame_Backup]);
			float ratioTime = (m_BlendTimer + 1.0f) / 60.0f;
			dx::XMStoreFloat4x4(pMtxBone, mtx1 * ratioTime + mtx2 * (1.0f - ratioTime));
		}
		pMtxBone++;
		pMtxBoneRef++;
	}

	//ブレンド終了制御
	if (m_BlendTimer > 59) {
		m_bBlendAnim = false;
		m_BlendTimer = 0;
	}
}
