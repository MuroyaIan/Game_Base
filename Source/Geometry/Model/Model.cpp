
//===== インクルード部 =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== クラス実装 =====
MODEL::MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(App.GetGfxPack()), m_ID(id), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_aMtxWorld(m_InstanceNum),
	m_bStatic(true), m_aAnimData(0), m_AnimID_Backup(0), m_AnimFrame_Backup(0), m_FrameCnt_Backup(0),
	m_bBlendAnim(false), m_BlendTimer(0)
{
	//アニメーション確認
	if (m_FileData.aAnimFrame.size() > 0)
		m_bStatic = false;

	//アニメーションバッファ作成
	if (!m_bStatic)
		m_Gfx.m_ModelMgr.SetAnimTexOn(m_ID);

	//メッシュ初期化
	for (size_t i = 0, Cnt = m_FileData.aMesh.size(); i < Cnt; i++)
		m_aMesh[i] = std::make_unique<MESH>(*this, static_cast<int>(i));
}

MODEL::~MODEL() noexcept
{
	//アニメーションバッファ解放
	if (!m_bStatic)
		m_Gfx.m_ModelMgr.SetAnimTexOff(m_ID);
}

//更新処理
void MODEL::Update() noexcept
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//アニメーション更新
	if (!m_bStatic) {

		if (!m_bBlendAnim) {
			for (auto& a : m_aAnimData)
				UpdateAnimation(a);
		}
		else {
			for (auto& a : m_aAnimData)
				UpdateAnimationBlending(a);		//ブレンド処理（0.2s秒間）
		}
	}

	//インスタンス情報更新
	auto pInst = &m_aInstanceData[0];
	auto pMtxW = &m_aMtxWorld[0];
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		pInst->MtxWorld = m_FileData.InitMtxWorld;						//初期行列
		gMath::MtxMultiply4x4_AVX(&pInst->MtxWorld._11, &pMtxW->_11);	//ワールド行列
		gMath::MtxTranspose4x4_SSE(&pInst->MtxWorld._11);				//転置処理

		//アニメーションフレーム更新
		if (!m_bStatic) {
			pInst->m_AnimFrame = 0;								//フレームリセット
			auto pAnim = &m_aAnimData[i];
			auto pFrame = &m_FileData.aAnimFrame[0];
			for (int j = 0, jCnt = pAnim->ID; j < jCnt; j++) {	//IDに至るまでのフレーム数を加算
				pInst->m_AnimFrame += *pFrame;
				pFrame++;
			}
			pInst->m_AnimFrame += pAnim->CurrentFrame;			//現在のフレーム数を加算
		}

		//ポインタ加算
		pInst++;
		pMtxW++;
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
	if (!m_bStatic) {
		ANIM_PACK AnimData;
		AnimData.ID = rand() % 2;
		AnimData.CurrentFrame = rand() % 60;
		m_aAnimData.push_back(std::move(AnimData));
	}

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
void MODEL::UpdateAnimation(ANIM_PACK& AnimData) noexcept
{
	//フレーム更新
	if (!m_FileData.aIsFPS_30[AnimData.ID]) {

		//60FPS
		AnimData.CurrentFrame++;

		//フレームカウントリセット
		if (AnimData.FrameCnt > 0)
			AnimData.FrameCnt = 0;
	}
	else {

		//30FPS
		AnimData.FrameCnt++;
		if (AnimData.FrameCnt > 1) {
			AnimData.FrameCnt = 0;

			//60FPSと同じ処理
			AnimData.CurrentFrame++;
		}
	}

	//フレーム制御
	if (AnimData.CurrentFrame > m_FileData.aAnimFrame[AnimData.ID] - 1)
		AnimData.CurrentFrame = 0;
}

//アニメーションブレンド更新
void MODEL::UpdateAnimationBlending(ANIM_PACK& AnimData) noexcept
{
	m_BlendTimer++;

	//フレーム更新
	if (!m_FileData.aIsFPS_30[AnimData.ID]) {

		//60FPS
		AnimData.CurrentFrame++;

		//フレームカウントリセット
		if (AnimData.FrameCnt > 0)
			AnimData.FrameCnt = 0;
	}
	else {

		//30FPS
		AnimData.FrameCnt++;
		if (AnimData.FrameCnt > 1) {
			AnimData.FrameCnt = 0;

			//60FPSと同じ処理
			AnimData.CurrentFrame++;
		}
	}

	//フレーム制御
	if (AnimData.CurrentFrame > m_FileData.aAnimFrame[AnimData.ID] - 1)
		AnimData.CurrentFrame = 0;

	//ブレンド前アニメーションの更新
	float ratio = static_cast<float>(AnimData.CurrentFrame + 1) / m_FileData.aAnimFrame[AnimData.ID];		//ブレンド後アニメーションのフレーム割合取得
	m_AnimFrame_Backup = static_cast<int>(m_FileData.aAnimFrame[m_AnimID_Backup] * ratio - 1);	//ブレンド前アニメーションのフレーム算出

	//骨情報更新
	//auto pMtxBone = &m_BoneData.mtxBone[0];
	//auto pMtxBoneRef = &m_FileData.aBone[0];
	//for (size_t i = 0, Cnt = m_FileData.aBone.size(); i < Cnt; i++) {
	//	if (pMtxBoneRef->aSkin[AnimData.ID].aMatrix.size() > 0) {
	//		//*pMtxBone = pMtxBoneRef->aSkin[AnimData.ID].aMatrix[AnimData.CurrentFrame];
	//		dx::XMMATRIX mtx1 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[AnimData.ID].aMatrix[AnimData.CurrentFrame]);
	//		dx::XMMATRIX mtx2 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[m_AnimID_Backup].aMatrix[m_AnimFrame_Backup]);
	//		float ratioTime = (m_BlendTimer + 1.0f) / 60.0f;
	//		dx::XMStoreFloat4x4(pMtxBone, mtx1 * ratioTime + mtx2 * (1.0f - ratioTime));
	//	}
	//	pMtxBone++;
	//	pMtxBoneRef++;
	//}

	//ブレンド終了制御
	if (m_BlendTimer > 59) {
		m_bBlendAnim = false;
		m_BlendTimer = 0;
	}
}
