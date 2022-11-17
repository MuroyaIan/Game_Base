/**
 * @file TestPlayer.h
 * @brief 課題用プレイヤー
 * @author 室谷イアン
 * @date 2022/09/27
 * @履歴 2022/09/27：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <App_Win64.h>

//===== 前方宣言 =====
class MODEL;

//===== クラス定義 =====

//***** モデル *****
class PLAYER
{
public:

	//プロトタイプ宣言
	PLAYER(APP_64& App) noexcept;
	~PLAYER() noexcept;
	void Update() noexcept;							//更新処理
	void Draw() const noexcept(!IS_DEBUG);			//書込み処理

private:

	//変数宣言
	std::unique_ptr<MODEL> m_pModel;	//モデル
	INPUT_MGR& m_Input;					//入力参照

};
