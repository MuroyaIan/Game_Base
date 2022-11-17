/**
 * @file Camera_Test.h
 * @brief テストカメラ
 * @author 室谷イアン
 * @date 2022/08/12
 * @履歴 2022/08/12：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Camera/Camera.h>

//===== クラス定義 =====

//***** テストカメラ *****
class CAMERA_TEST : public CAMERA
{
public:

	//プロトタイプ宣言
	CAMERA_TEST(APP_64& App) noexcept;
	~CAMERA_TEST() noexcept override;
	void Update() noexcept override;	//更新処理

private:

	//変数宣言
	INPUT_MGR& m_Input;					//入力参照

	//プロトタイプ宣言
	void ResetCamera() noexcept override;	//カメラリセット
	void UpdateMatrix() noexcept;			//行列更新
};
