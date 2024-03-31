
//===== インクルード部 =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== クラス実装 =====
MODEL::MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(App.GetGfxPack()), m_ID(id), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_aMtxWorld(m_InstanceNum), m_bStatic(true), m_aAnimData(0)
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
void MODEL::Update()
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//アニメーション更新
	if (!m_bStatic) {
		for (auto& a : m_aAnimData) {
			if (!a.bBlendAnim)
				UpdateAnimation(a);				//ブレンドなし
			else
				UpdateAnimationBlending(a);		//ブレンドあり（1秒間）
		}
	}

	//インスタンス更新
	auto pInst = &m_aInstanceData[0];
	auto pMtxW = &m_aMtxWorld[0];
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		pInst->MtxWorld = m_FileData.InitMtxWorld;						//初期行列
		gMath::MtxMultiply4x4_AVX(&pInst->MtxWorld._11, &pMtxW->_11);	//ワールド行列
		gMath::MtxTranspose4x4_SSE(&pInst->MtxWorld._11);				//転置処理

		//アニメーション更新
		if (!m_bStatic) {
			pInst->AnimFrame = 0;
			pInst->AnimBlendFrame = 0;							//フレームリセット
			auto pAnim = &m_aAnimData[i];

			//フレーム更新
			auto pFrame = &m_FileData.aAnimFrame[0];
			for (int j = 0, jCnt = pAnim->ID; j < jCnt; j++) {	//IDに至るまでのフレーム数を加算
				pInst->AnimFrame += *pFrame;
				pFrame++;
			}
			pInst->AnimFrame += pAnim->CurrentFrame;			//現在のフレーム数を加算

			//フレーム更新（ブレンド用）
			if (pAnim->bBlendAnim) {
				pFrame = &m_FileData.aAnimFrame[0];
				for (int j = 0, jCnt = pAnim->BlendID; j < jCnt; j++) {
					pInst->AnimBlendFrame += *pFrame;
					pFrame++;
				}
				pInst->AnimBlendFrame += pAnim->BlendCurrentFrame;
				pInst->AnimLerp = pAnim->AnimLerp;
			}
			else
				pInst->AnimLerp = 0.0f;
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
void MODEL::Draw() noexcept
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
		AnimData.ID = rand() % 3;
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
void MODEL::UpdateAnimation(ANIM_PACK& AnimData) const noexcept
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
	//フレーム更新（ブレンド後）
	if (!m_FileData.aIsFPS_30[AnimData.BlendID]) {

		//60FPS
		AnimData.BlendCurrentFrame++;

		//フレームカウントリセット
		if (AnimData.BlendFrameCnt > 0)
			AnimData.BlendFrameCnt = 0;
	}
	else {

		//30FPS
		AnimData.BlendFrameCnt++;
		if (AnimData.BlendFrameCnt > 1) {
			AnimData.BlendFrameCnt = 0;

			//60FPSと同じ処理
			AnimData.BlendCurrentFrame++;
		}
	}

	//フレーム制御
	if (AnimData.BlendCurrentFrame > m_FileData.aAnimFrame[AnimData.BlendID] - 1)
		AnimData.BlendCurrentFrame = 0;

	//フレーム更新（ブレンド前）
	if (!AnimData.bBlendSync)
		UpdateAnimation(AnimData);
	else {

		//フレームの同期処理
		float FrameRatio = AnimData.BlendCurrentFrame / (m_FileData.aAnimFrame[AnimData.BlendID] - 1.0f);	//フレーム同期用の割合取得
		AnimData.CurrentFrame = static_cast<int>((m_FileData.aAnimFrame[AnimData.ID] - 1) * FrameRatio);	//ブレンド前のフレーム算出
	}

	//線形補間計算
	AnimData.AnimLerp = static_cast<float>(AnimData.BlendTimer) / AnimData.BlendTime;

	//ブレンド終了
	AnimData.BlendTimer++;
	if (AnimData.BlendTimer > AnimData.BlendTime) {

		//アニメーション情報上書き
		AnimData.ID = AnimData.BlendID;
		AnimData.CurrentFrame = AnimData.BlendCurrentFrame;
		AnimData.FrameCnt = AnimData.BlendFrameCnt;

		//ブレンド終了
		AnimData.bBlendAnim = false;
		AnimData.BlendID = 0;
		AnimData.BlendCurrentFrame = 0;
		AnimData.AnimLerp = 0.0f;
	}
}
