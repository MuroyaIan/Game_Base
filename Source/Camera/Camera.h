/**
 * @file Camera.h
 * @brief カメラの親クラス
 * @author 室谷イアン
 * @date 2022/09/26
 * @履歴 2022/09/26：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Camera/CameraMgr.h>

//===== クラス定義 =====

//***** カメラの親クラス *****
class CAMERA
{
public:

	//プロトタイプ宣言
	explicit CAMERA(APP& App) noexcept;
	virtual ~CAMERA() noexcept;
	virtual void Update() noexcept = 0;					//更新処理

	DirectX::XMFLOAT4X4 GetWorldMtx() const noexcept	//ワールド行列取得
	{
		return m_mtxWorld;
	}
	DirectX::XMFLOAT4X4 GetViewMtx() const noexcept		//ビュー行列取得
	{
		return m_mtxView;
	}
	DirectX::XMFLOAT4X4 GetProjMtx() const noexcept		//投影行列取得
	{
		return m_mtxProj;
	}
	DirectX::XMFLOAT3 GetRotation() const noexcept		//回転情報取得
	{
		return m_Rot;
	}

protected:

	//変数宣言
	APP& m_App;							//App参照

	DirectX::XMFLOAT3 m_Pos;			//座標
	DirectX::XMFLOAT3 m_LookAt;			//注視点
	DirectX::XMFLOAT3 m_vUp;			//アップベクトル

	short m_FOV;						//視野角
	float g_AspectRatio;				//縦横比
	float m_NearZ;
	float m_FarZ;						//クリップ距離

	DirectX::XMFLOAT4X4 m_mtxWorld;		//ワールド行列
	DirectX::XMFLOAT4X4 m_mtxView;		//ビュー行列
	DirectX::XMFLOAT4X4 m_mtxProj;		//投影行列

	DirectX::XMFLOAT3 m_Rot;			//回転
	float m_Sensitivity;				//感度

	//プロトタイプ宣言
	void InitMatrix() noexcept;					//行列初期化
	void UpdateProjMatrix() noexcept;			//投影行列更新
	virtual void ResetCamera() noexcept = 0;	//カメラリセット
};
