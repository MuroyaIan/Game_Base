/**
 * @file Model.h
 * @brief モデル
 * @author 室谷イアン
 * @date 2022/09/08
 * @履歴 2022/09/08：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <App.h>
#include <Geometry/Model/ModelRef.h>
#include <Geometry/Model/ModelMgr.h>

//===== 前方宣言 =====
class DRAWER;
class CT_BINDER;

//===== クラス定義 =====

//***** モデル *****
class MODEL
{
public:

	//テクスチャパック
	struct ANIM_PACK
	{
		int ID;					//アニメーション番号
		int CurrentFrame;		//アニメーション再生フレーム
		int FrameCnt;			//再生フレーム計算用
		int BlendID;			//アニメーション番号（ブレンド用）
		int BlendCurrentFrame;	//アニメーション再生フレーム（ブレンド用）
		int BlendFrameCnt;		//再生フレーム計算用（ブレンド用）
		float AnimLerp;			//アニメーションの線形補間（ブレンド用）
		bool bBlendAnim;		//ブレンドモード
		int BlendTimer;			//ブレンド用タイマ
		int BlendTime;			//ブレンド期間
		bool bBlendSync;		//ブレンド同期モード

		ANIM_PACK() noexcept :
			ID(0), CurrentFrame(0), FrameCnt(0),
			BlendID(0), BlendCurrentFrame(0), BlendFrameCnt(0), AnimLerp(0.0f),
			bBlendAnim(false), BlendTimer(0), BlendTime(0), bBlendSync(false)
		{}
		~ANIM_PACK() noexcept
		{}
	};

	//プロトタイプ宣言
	explicit MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept;
	~MODEL() noexcept;
	void Update();																	//更新処理
	void Draw() noexcept;															//書込み処理
	int AddInstance();																//インスタンス追加
	UINT GetPolygonNum() const noexcept;											//ポリゴン数取得

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept		//ワールド行列取得
	{
		return m_aMtxWorld[InstanceIndex];
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept	//ワールド行列設定
	{
		m_aMtxWorld[InstanceIndex] = mtxW;
	}

	void ChangeAnimID(int id, int InstNum, float BlendTime = 1.0f, bool Sync = false) noexcept	//アニメーション変更
	{
		ANIM_PACK& AnimData = m_aAnimData[InstNum];

		//アニメーション切替＆ブレンド中ではない
		if (AnimData.ID != id && !AnimData.bBlendAnim) {

			//ブレンド情報更新
			AnimData.bBlendAnim = true;
			AnimData.BlendTimer = 0;
			AnimData.BlendTime = static_cast<int>(BlendTime * 60) - 1;
			AnimData.bBlendSync = Sync;
			AnimData.BlendID = id;
			AnimData.BlendCurrentFrame = 0;
			AnimData.BlendFrameCnt = 0;
			AnimData.AnimLerp = 0.0f;

			//フレームの同期処理
			if (AnimData.bBlendSync) {
				float FrameRatio = AnimData.CurrentFrame / (m_FileData.aAnimFrame[AnimData.ID] - 1.0f);						//フレーム同期用の割合取得
				AnimData.BlendCurrentFrame = static_cast<int>((m_FileData.aAnimFrame[AnimData.BlendID] - 1) * FrameRatio);	//ブレンド後のフレーム算出
			}
		}
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	MODEL_MGR::MODEL_ID m_ID;						//モデルID
	ModelRef::MODEL_PACK& m_FileData;				//モデル情報
	std::vector<std::unique_ptr<DRAWER>> m_aMesh;	//メッシュ配列

	int m_InstanceNum;								//インスタンス数
	std::vector<VSD_INSTANCE> m_aInstanceData;		//インスタンス情報
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//ワールド行列

	bool m_bStatic;									//静的メッシュかどうか
	std::vector<ANIM_PACK> m_aAnimData;				//アニメーション情報

	//プロトタイプ宣言
	void UpdateAnimation(ANIM_PACK& AnimData) const noexcept;		//アニメーション更新
	void UpdateAnimationBlending(ANIM_PACK& AnimData) noexcept;		//アニメーションブレンド更新

	//権限指定
	friend class MESH;
};
