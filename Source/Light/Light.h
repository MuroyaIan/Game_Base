/**
 * @file Light.h
 * @brief ライト親クラス
 * @author 室谷イアン
 * @date 2022/08/20
 * @履歴 2022/08/20：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <App.h>

//===== 前方宣言 =====

#ifdef IMGUI

class MESH_LIGHT;

#endif // IMGUI

//===== クラス定義 =====

//***** ライト親クラス *****
class LIGHT
{
public:

	//ライト情報
	struct LIGHT_DIRECTIONAL		//平行光源
	{
		DirectX::XMFLOAT4 Pos;		//ワールド座標
		DirectX::XMFLOAT4 Color_D;	//光の色（拡散色, wは強度）

		explicit LIGHT_DIRECTIONAL() noexcept :
			Pos(0.0f, 1.0f, 0.0f, 1.0f), Color_D(0.0f, 0.0f, 0.0f, 0.0f)
		{}
		~LIGHT_DIRECTIONAL() noexcept
		{}
	};

	struct LIGHT_POINT				//点光源
	{
		DirectX::XMFLOAT4 Pos;		//ワールド座標
		DirectX::XMFLOAT4 Color_D;	//光の色（拡散色, wは強度）

		float AttConst;
		float AttLinear;
		float AttQuadratic;			//光の減衰
		float Pad;					//仮置き

		explicit LIGHT_POINT() noexcept :
			Pos(0.0f, 0.0f, 0.0f, 1.0f), Color_D(0.0f, 0.0f, 0.0f, 0.0f),
			AttConst(1.0f), AttLinear(0.045f), AttQuadratic(0.0075f), Pad(1.0f)
		{}
		~LIGHT_POINT() noexcept
		{}
	};

	//プロトタイプ宣言
	explicit LIGHT(APP& App) noexcept;
	virtual ~LIGHT() noexcept;
	virtual void Update() const noexcept = 0;	//更新処理

protected:

	//変数宣言

#ifdef IMGUI

	MESH_LIGHT& m_Mesh;			//描画メッシュ参照
	int m_MeshIndex;			//メッシュインデックス
	DirectX::XMFLOAT3 m_Scale;	//サイズ

#endif // IMGUI

};
