/**
 * @file Bone.h
 * @brief ジオメトリ描画（ポリゴン表示）
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== 前方宣言 =====
class VIEWER;
class FBX_LOADER;

//===== クラス定義 =====

//***** ジオメトリ描画（ポリゴン表示） *****
class BONE : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit BONE(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, INPUT_MGR& Input);
	~BONE() noexcept override;
	void Update() noexcept override;														//更新処理
	void Draw(int InstanceNum = 0) const noexcept override;									//書込み処理
	int AddInstance() override;																//インスタンス追加
	void ClearInstance() override;															//インスタンスクリア

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override		//変形行列取得
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept override	//変形行列設定
	{
		(void)InstanceIndex;
		m_mtxWorld = mtxW;
	}

	UINT GetPolygonNum() const noexcept override											//ポリゴン数取得
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

	float& GetScale() noexcept																//メッシュスケール参照
	{
		return m_Scale;
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	int m_InstanceNum;								//インスタンス数
	FBX_LOADER& m_Loader;							//ローダ参照

	std::vector<DirectX::XMFLOAT4X4> m_aMtxBone;	//骨の姿勢行列(インスタンシング)
	bool& m_bDrawAnimation;							//アニメーション再生
	int& m_AnimationID;								//アニメーション番号
	int& m_AnimFrame;								//再生するフレーム数
	float m_Scale;									//メッシュスケール

	DirectX::XMFLOAT4X4 m_mtxWorld;					//ワールド行列
	float& m_ModelScale;							//モデルスケール

	INPUT_MGR& m_Input;								//入力参照
	float& m_RotY;									//回転(Y軸)
};
