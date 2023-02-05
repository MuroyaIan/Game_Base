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
#include <App.h>

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
		Test,
		FirstPerson,

		MAX_CAMERA
	};

	//プロトタイプ宣言
	explicit CAMERA_MGR(APP& App) noexcept;
	~CAMERA_MGR() noexcept;
	void Update() const noexcept;									//更新処理
	void SetCamera(CAMERA_ID id) noexcept;							//カメラセット
	DirectX::XMFLOAT4X4 GetWorldMtx(CAMERA_ID id) const noexcept;	//ワールド行列取得
	DirectX::XMFLOAT3 GetRotation(CAMERA_ID id) const noexcept;		//回転情報取得

private:

	//変数宣言
	APP& m_App;											//App参照
	std::vector<std::unique_ptr<CAMERA>> m_aCamera;		//カメラのポインタ配列
	CAMERA_ID m_CurrentCamera;							//現在使用中のカメラ
};
