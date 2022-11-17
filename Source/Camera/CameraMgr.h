/**
 * @file CameraMgr.h
 * @brief カメラマネージャ
 * @author 室谷イアン
 * @date 2022/08/21
 * @履歴 2022/08/21：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <App_Win64.h>

//===== 前方宣言 =====
class CAMERA;

//===== クラス定義 =====

//***** カメラマネージャ *****
class CAMERA_MGR
{
public:

	//カメラID
	enum class CAMERA_ID
	{
		TEST,
		FP,

		MAX_CAMERA
	};

	//プロトタイプ宣言
	CAMERA_MGR(APP_64& App) noexcept;
	~CAMERA_MGR() noexcept;
	void Update() noexcept;								//更新処理
	void Draw() const noexcept;							//描画処理
	void SetCamera(CAMERA_ID id) noexcept;				//カメラセット
	DirectX::XMFLOAT4X4 GetWorldMtx() const noexcept;	//ワールド行列取得
	DirectX::XMFLOAT3 GetRotation() const noexcept;		//回転情報取得

private:

	//変数宣言
	APP_64& m_App;										//App参照
	DirectX::XMFLOAT4X4 m_mtxView;						//ビュー行列
	DirectX::XMFLOAT4X4 m_mtxProj;						//投影行列
	std::vector<std::unique_ptr<CAMERA>> m_aCamera;		//カメラのポインタ配列
	CAMERA_ID m_CurrentCamera;							//現在使用中のカメラ
};
