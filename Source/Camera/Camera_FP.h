/**
 * @file Camera_FP.h
 * @brief FPカメラ
 * @author 室谷イアン
 * @date 2022/09/26
 * @履歴 2022/09/26：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Camera/Camera.h>

//===== クラス定義 =====

//***** FPカメラ *****
class CAMERA_FP : public CAMERA
{
public:

	//プロトタイプ宣言
	explicit CAMERA_FP(APP& App) noexcept;
	~CAMERA_FP() noexcept override;
	void Update() noexcept override;		//更新処理

private:

	//変数宣言
	INPUT_MGR& m_Input;						//入力参照

	//プロトタイプ宣言
	void UpdateMatrix() noexcept;			//行列更新
	void ResetCamera() noexcept override;	//カメラリセット
};
