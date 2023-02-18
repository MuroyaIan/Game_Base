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
class BINDER;
class DRAWER;
struct CBD_BONE;

//===== クラス定義 =====

//***** モデル *****
class MODEL
{
public:

	//プロトタイプ宣言
	explicit MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept;
	~MODEL() noexcept;
	void Update() noexcept;								//更新処理
	void Draw() const noexcept;							//書込み処理
	int AddInstance();									//インスタンス追加
	UINT GetPolygonNum() const noexcept;				//ポリゴン数取得

	void ChangeAnimID(int id) noexcept					//アニメーション変更
	{
		if (m_AnimID != id && m_bBlendAnim == false) {	//アニメーション切替＆ブレンド中ではない

			//アニメーション番号のバックアップ更新
			if (m_AnimID_Backup != m_AnimID)
				m_AnimID_Backup = m_AnimID;
			if (m_AnimFrame_Backup != m_AnimFrame)
				m_AnimFrame_Backup = m_AnimFrame;
			if (m_FrameCnt_Backup != m_FrameCnt)
				m_FrameCnt_Backup = m_FrameCnt;

			//アニメーション番号更新
			float ratio = static_cast<float>(m_AnimFrame + 1) / m_FileData.aAnimFrame[m_AnimID];	//ブレンド前アニメーションのフレーム割合取得
			m_AnimID = id;
			m_AnimFrame = static_cast<int>(m_FileData.aAnimFrame[m_AnimID] * ratio - 1);			//ブレンド後アニメーションのフレーム算出
			m_bBlendAnim = true;
		}
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	ModelRef::MODEL_PACK& m_FileData;				//ファイル情報
	std::vector<std::unique_ptr<DRAWER>> m_aMesh;	//メッシュ配列

	int m_InstanceNum;								//インスタンス数
	std::vector<VSD_INSTANCE> m_aInstanceData;		//インスタンス情報

	bool m_bStatic;									//静的メッシュかどうか
	std::unique_ptr<BINDER> m_pBoneBuffer;			//骨情報用バインダ
	std::unique_ptr<CBD_BONE> m_pMtxBone;		//骨情報（定数バッファ用）
	int m_AnimID;									//アニメーション番号
	int m_AnimID_Backup;							//アニメーション番号（バックアップ）
	int m_AnimFrame;								//アニメーション再生フレーム
	int m_AnimFrame_Backup;							//アニメーション再生フレーム（バックアップ）
	int m_FrameCnt;									//再生フレーム計算用
	int m_FrameCnt_Backup;							//再生フレーム計算用（バックアップ）
	bool m_bBlendAnim;								//ブレンドモード
	int m_BlendTimer;								//ブレンド用タイマ

	//プロトタイプ宣言
	void UpdateAnimation() noexcept;				//アニメーション更新
	void UpdateAnimationBlending() noexcept;		//アニメーションブレンド更新

	//権限指定
	friend class MESH;
};
