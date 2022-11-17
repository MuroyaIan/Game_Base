/**
 * @file Binder.h
 * @brief DirectXのバインド処理
 * @author 室谷イアン
 * @date 2022/06/24
 * @履歴 2022/06/24：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Graphic.h>

//===== クラス定義 =====

//***** バインダ *****
class BINDER
{
public:

	//プロトタイプ宣言
	BINDER() noexcept;
	virtual ~BINDER() noexcept;
	virtual void Bind(GRAPHIC& Gfx) noexcept = 0;					//バインド処理

protected:

	//プロトタイプ宣言
	static ID3D11Device* GetDevice(GRAPHIC& Gfx) noexcept			//デバイス取得
	{
		return Gfx.m_pDevice.Get();
	}

	static ID3D11DeviceContext* GetContext(GRAPHIC& Gfx) noexcept	//コンテキスト取得
	{
		return Gfx.m_pContext.Get();
	}
};
