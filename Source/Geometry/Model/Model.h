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
class BINDER;

//===== クラス定義 =====

//***** モデル *****
class MODEL
{
public:

	//テクスチャパック
	struct ANIM_PACK
	{
		int ID;				//アニメーション番号
		int CurrentFrame;	//アニメーション再生フレーム
		int FrameCnt;		//再生フレーム計算用

		ANIM_PACK() noexcept : ID(0), CurrentFrame(0), FrameCnt(0)
		{}
		~ANIM_PACK() noexcept
		{}
	};

	//プロトタイプ宣言
	explicit MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept;
	~MODEL() noexcept;
	void Update() noexcept;															//更新処理
	void Draw() const noexcept;														//書込み処理
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

	void ChangeAnimID(int id, int InstNum) noexcept									//アニメーション変更
	{
		ANIM_PACK& AnimData = m_aAnimData[InstNum];

		//アニメーション切替＆ブレンド中ではない
		if (AnimData.ID != id && m_bBlendAnim == false) {

			//アニメーション番号のバックアップ更新
			if (m_AnimID_Backup != AnimData.ID)
				m_AnimID_Backup = AnimData.ID;
			if (m_AnimFrame_Backup != AnimData.CurrentFrame)
				m_AnimFrame_Backup = AnimData.CurrentFrame;
			if (m_FrameCnt_Backup != AnimData.FrameCnt)
				m_FrameCnt_Backup = AnimData.FrameCnt;

			//アニメーション番号更新
			float ratio = static_cast<float>(AnimData.CurrentFrame + 1) / m_FileData.aAnimFrame[AnimData.ID];	//ブレンド前アニメーションのフレーム割合取得
			AnimData.ID = id;
			AnimData.CurrentFrame = static_cast<int>(m_FileData.aAnimFrame[AnimData.ID] * ratio - 1);			//ブレンド後アニメーションのフレーム算出
			m_bBlendAnim = true;
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
	int m_AnimID_Backup;							//アニメーション番号（バックアップ）
	int m_AnimFrame_Backup;							//アニメーション再生フレーム（バックアップ）
	int m_FrameCnt_Backup;							//再生フレーム計算用（バックアップ）

	bool m_bBlendAnim;								//ブレンドモード
	int m_BlendTimer;								//ブレンド用タイマ

	//プロトタイプ宣言
	void UpdateAnimation(ANIM_PACK& AnimData) noexcept;				//アニメーション更新
	void UpdateAnimationBlending(ANIM_PACK& AnimData) noexcept;		//アニメーションブレンド更新

	//権限指定
	friend class MESH;
};
