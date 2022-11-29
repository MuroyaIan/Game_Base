/**
 * @file cbMatrix_LWVP.h
 * @brief 変換行列の処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/25：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== 構造体宣言 =====
struct CBD_MTX_LWVP
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxLocal;	//ローカル行列
	DirectX::XMFLOAT4X4 mtxWorld;	//ワールド行列
	DirectX::XMFLOAT4X4 mtxView;	//ビュー行列
	DirectX::XMFLOAT4X4 mtxProj;	//投影行列

	CBD_MTX_LWVP() noexcept : mtxLocal(), mtxWorld(), mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxLocal, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}

	CBD_MTX_LWVP(DirectX::XMFLOAT4X4 mtxL, DirectX::XMFLOAT4X4 mtxW, DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept :
		mtxLocal(mtxL), mtxWorld(mtxW), mtxView(mtxV), mtxProj(mtxP)
	{}

	~CBD_MTX_LWVP() noexcept
	{}
};

//===== 前方宣言 =====
class DRAWER;

//===== クラス定義 =====

//***** 変換行列バッファ *****
class CB_MTX_LWVP : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CB_MTX_LWVP(const GRAPHIC& Gfx, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL);
	~CB_MTX_LWVP() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;			//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VS_CBUFFER<CBD_MTX_LWVP>> m_pCBuffVS;	//定数バッファのポインタ
	static int m_RefCount;											//定数バッファの利用数
	const DRAWER& m_Parent;											//所属するドロワー（has-a）
	const DirectX::XMFLOAT4X4& m_mtxL;								//ローカル行列の提供先
};
