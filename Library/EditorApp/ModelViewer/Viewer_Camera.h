/**
 * @file Viewer_Camera.h
 * @brief ビューワ用カメラ
 * @author 室谷イアン
 * @date 2022/07/19
 * @履歴 2022/07/19：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <EditorApp/Editor.h>

//===== クラス定義 =====

//***** ビューワ用カメラ *****
class VIEWER_CAM
{
public:

	//プロトタイプ宣言
	VIEWER_CAM(APP& App) noexcept;
	~VIEWER_CAM() noexcept;
	void Update() noexcept;									//更新処理
	void UpdateMatrix() noexcept;							//行列更新
	void ResetCamera() noexcept;							//カメラリセット

	DirectX::XMFLOAT3 GetRotation() const noexcept			//回転角（公転）取得
	{
		return m_Rot;
	}
	DirectX::XMFLOAT3 GetPosition() const noexcept			//位置取得
	{
		return m_Pos;
	}
	int& GetZoomSpeed() noexcept							//ズーム速度取得
	{
		return m_ZoomSPD;
	}
	float GetCenterDistance() const noexcept				//中心距離取得
	{
		return m_CtrDist;
	}

private:

	//カメラモード
	enum class MODE
	{
		NONE,	//操作なし
		ORBIT,	//軌道（公転）モード
		TRACK,	//注視点移動（視点も同期）
	};

	//変数宣言
	APP& m_App;						//App参照

	DirectX::XMFLOAT3 m_LookAt;			//注視点
	DirectX::XMFLOAT3 m_vUp;			//アップベクトル

	short m_FOV;						//視野角
	float g_AspectRatio;				//縦横比
	float m_NearZ;
	float m_FarZ;						//クリップ距離

	DirectX::XMFLOAT4X4 m_mtxWorld;		//ワールド行列
	DirectX::XMFLOAT4X4 m_mtxView;		//ビュー行列
	DirectX::XMFLOAT4X4 m_mtxProj;		//投影行列

	MODE m_Mode;						//操作モード
	DirectX::XMFLOAT2 m_MousePos_Old;	//マウス座標（直前フレーム）
	DirectX::XMFLOAT3 m_Rot;			//回転角（公転）
	int m_Level_RotSPD;					//公転速度
	DirectX::XMFLOAT3 m_Pos;			//3D座標
	int m_Level_Zoom;					//ズームレベル
	int m_Backup_Wheel;					//マウスホイールの操作量
	int m_ZoomSPD;						//ズーム速度
	float m_CtrDist;					//中心距離
	DirectX::XMFLOAT3 m_Offset;			//相対移動量
};
