/**
 * @file Shape_Model.h
 * @brief 波描画
 * @author 室谷イアン
 * @date 2023/01/10
 * @履歴 2023/01/10：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>
#include <App.h>

//===== クラス定義 =====

//***** ジオメトリ描画（モデル表示） *****
class WAVE : public DRAWER
{
public:

	//プロトタイプ宣言
	WAVE(APP& App);
	~WAVE() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(int InstanceNum = -1) const noexcept override;		//書込み処理
	int AddInstance() override;																	//インスタンス追加

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		return m_aInstanceData[InstanceIndex].MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//ポリゴン数取得
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

	float ClacSineWave(float Dist, float Time, bool bLoop = true);			//正弦波による高さ計算
	float ClacRingWave(VECTOR2 OriPos, VECTOR2 Pos, float Time);			//環状波による高さ計算
	float ClacStraightWave(VECTOR2 vDirection, VECTOR2 vPos, float Time);	//直進波による高さ計算

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	int m_InstanceNum;								//インスタンス数
	std::vector<VSD_INSTANCE> m_aInstanceData;		//インスタンス情報
	CBD_MATERIAL m_Material;						//マテリアル情報

	INPUT_MGR& m_Input;								//入力参照
	std::vector<DirectX::XMFLOAT3> m_aInitPos;		//各粒子の初期位置
	float m_WaveLength;								//波の波長
	float m_WaveCoef;								//波係数（振幅を0-1で制御）
};
