/**
 * @file Viewer.h
 * @brief モデルビューワ
 * @author 室谷イアン
 * @date 2022/07/19
 * @履歴 2022/07/19：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <EditorApp/Editor.h>
#include <EditorApp/ModelViewer/Viewer_Camera.h>
#include <EditorApp/ModelViewer/ModelLoader.h>

//===== 前方宣言 =====
class DRAWER;
class BONE;

//===== クラス定義 =====

//***** モデルビューワ *****
class VIEWER
{
public:

	//プロトタイプ宣言
	explicit VIEWER(APP& App);
	~VIEWER() noexcept;
	void Update() noexcept;							//更新処理
	void Draw() const;								//描画処理
	int GetPolygonNum() const noexcept;				//ポリゴン数取得
	void LoadModel(bool bAnimOnly = false);			//モデル読込

	int GetDrawerNum() const noexcept				//ドロワー有効数取得
	{
		return static_cast<int>(m_aDrawer.size());
	}
	VIEWER_CAM& GetCamera() noexcept				//カメラ参照
	{
		return m_Camera;
	}
	FBX_LOADER& GetLoader() const noexcept			//FBXローダ参照
	{
		return *m_pLoader;
	}
	float& GetModelScale() noexcept					//モデルスケール参照
	{
		return m_Scale;
	}
	float& GetModelRotation() noexcept				//モデル回転(Y軸)参照
	{
		return m_RotY;
	}
	bool& GetFlag_DrawNormal() noexcept				//法線表示フラグ参照
	{
		return m_bDrawNormal;
	}
	bool& GetFlag_DrawSurface() noexcept			//ポリゴン表示切替フラグ参照
	{
		return m_bDrawSurface;
	}
	bool& GetFlag_DrawAnimation() noexcept			//アニメーション再生フラグ参照
	{
		return m_bDrawAnimation;
	}
	int& GetAnimationID() noexcept					//アニメーション番号参照
	{
		return m_AnimationID;
	}
	int& GetAnimationFrame() noexcept				//再生フレーム数参照
	{
		return m_AnimFrame;
	}
	bool& GetFlag_AnimPause() noexcept				//アニメーション一時停止フラグ参照
	{
		return m_bAnimPause;
	}
	bool& GetFlag_DrawBone() noexcept				//骨描画フラグ参照
	{
		return m_bDrawBone;
	}
	BONE& GetBone() const noexcept;					//骨メッシュ参照

private:

	//変数宣言
	GFX_PACK& m_Gfx;									//描画データ参照
	INPUT_MGR& m_Input;									//入力参照
	std::vector<std::unique_ptr<DRAWER>> m_aDrawer;		//描画配列
	VIEWER_CAM m_Camera;								//カメラ

	std::unique_ptr<FBX_LOADER> m_pLoader;				//FBXローダ
	float m_Scale;										//モデルスケール
	float m_RotY;										//モデル回転(Y軸)
	bool m_bDrawNormal;									//法線表示
	bool m_bDrawSurface;								//ポリゴン表示切替

	bool m_bDrawAnimation;								//アニメーション再生
	int m_AnimationID;									//アニメーション番号
	int m_AnimFrame;									//再生するフレーム数
	bool m_bAnimPause;									//アニメーション一時停止

	std::unique_ptr<DRAWER> m_Drawer_Bone;				//骨描画用
	std::unique_ptr<DRAWER> m_Drawer_BoneLine;			//骨ライン描画用
	bool m_bDrawBone;									//骨描画フラグ
};
