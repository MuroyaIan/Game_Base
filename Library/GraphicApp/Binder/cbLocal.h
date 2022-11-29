/**
 * @file cbLocal.h
 * @brief 骨情報バッファ
 * @author 室谷イアン
 * @date 2022/09/29
 * @履歴 2022/09/29：ファイル作成
 *		 2022/11/25：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== 構造体宣言 =====
struct CBD_MTX_LOCAL
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxSkin;	//骨なしメッシュ用ローカル行列

	CBD_MTX_LOCAL() noexcept : mtxSkin()
	{
		DirectX::XMStoreFloat4x4(&mtxSkin, DirectX::XMMatrixIdentity());
	}

	~CBD_MTX_LOCAL() noexcept
	{}
};

//===== クラス定義 =====

//***** 骨情報バッファ *****
class CB_LOCAL : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CB_LOCAL(const GRAPHIC& Gfx, const CBD_MTX_LOCAL& LocalData);
	~CB_LOCAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;			//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VS_CBUFFER<CBD_MTX_LOCAL>> m_pCBuffVS;	//定数バッファのポインタ
	static int m_RefCount;											//定数バッファの利用数
	const CBD_MTX_LOCAL& m_LocalData;								//骨情報の提供先
};
