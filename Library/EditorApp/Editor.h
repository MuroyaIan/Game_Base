/**
 * @file Editor.h
 * @brief エディタクラス
 * @author 室谷イアン
 * @date 2022/07/19
 * @履歴 2022/07/19：ファイル作成
 *					 ver.02⇒FbxViewer実装
 *					 ver.04⇒スケルトンアニメーション実装
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <App.h>

//===== 前方宣言 =====
class VIEWER;

//===== クラス定義 =====

//***** エディタ *****
class EDITOR
{
public:

	//プロトタイプ宣言
	EDITOR(APP& App) noexcept;
	~EDITOR() noexcept;
	void Update();						//更新処理
	void Draw();						//描画処理

	bool IsEnabled() const noexcept		//状態確認
	{
		return m_bEnable;
	}

private:

	//変数宣言
	bool m_bEnable;						//エディタ状態
	APP& m_App;						//App参照
	bool m_bFBX_Viewer;					//FBXビューワー
	std::unique_ptr<VIEWER> m_pViewer;	//ビューワ用ポインタ
};
