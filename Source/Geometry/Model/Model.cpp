
//===== インクルード部 =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== クラス実装 =====
MODEL::MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(App.GetGfxPack()), m_ID(id), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_aMtxWorld(m_InstanceNum),
	m_bStatic(true), m_pBoneBuffer(), m_BoneData(), m_AnimID(1), m_AnimID_Backup(m_AnimID), m_AnimFrame(0), m_AnimFrame_Backup(m_AnimFrame), m_FrameCnt(0), m_FrameCnt_Backup(m_FrameCnt),
	m_bBlendAnim(false), m_BlendTimer(0)
{
	//アニメーション確認
	if (m_FileData.aAnimFrame.size() > 0)
		m_bStatic = false;

	//VS定数バッファ作成（骨情報）
	if (!m_bStatic)
		m_pBoneBuffer = std::make_unique<CB_BONE>(m_Gfx.m_DX, nullptr, m_BoneData);

	//メッシュ初期化
	for (size_t i = 0, Cnt = m_FileData.aMesh.size(); i < Cnt; i++)
		m_aMesh[i] = std::make_unique<MESH>(*this, static_cast<int>(i));
}

MODEL::~MODEL() noexcept
{
}

//更新処理
void MODEL::Update() noexcept
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//アニメーション更新
	if (!m_bStatic) {

		if (!m_bBlendAnim)
			UpdateAnimation();
		else
			UpdateAnimationBlending();	//ブレンド処理（0.2s秒間）

		//骨情報を更新
		m_pBoneBuffer->Bind(m_Gfx.m_DX);
	}

	//ワールド行列更新
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		m_aInstanceData[i].MtxWorld = m_FileData.InitMtxWorld;
		gMath::MtxMultiply4x4_AVX(&m_aInstanceData[i].MtxWorld._11, &m_aMtxWorld[i]._11);
	}

	//メッシュ更新
	for (auto& m : m_aMesh)
		m->Update();
}

//書込み処理
void MODEL::Draw() const noexcept
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//メッシュ描画
	for (auto& m : m_aMesh)
		m->Draw();
}

//インスタンス追加
int MODEL::AddInstance()
{
	m_InstanceNum++;

	//配列追加
	m_aInstanceData.push_back(VSD_INSTANCE{});
	dx::XMFLOAT4X4 mtxW{};
	dx::XMStoreFloat4x4(&mtxW, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(std::move(mtxW));

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
	auto pMtxBone = &m_BoneData.mtxBone[0];
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
	auto pMtxBone = &m_BoneData.mtxBone[0];
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
