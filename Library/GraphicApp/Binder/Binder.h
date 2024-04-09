/**
 * @file Binder.h
 * @brief DirectXのバインド処理
 * @author 室谷イアン
 * @date 2022/06/24
 * @履歴 2022/06/24：ファイル作成
 *		 2022/11/21：処理改善
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
	explicit BINDER() noexcept;
	virtual ~BINDER() noexcept;
	virtual void Bind(const CT_GRAPHIC& Gfx) const = 0;	//バインド処理

protected:

	//プロトタイプ宣言
	template <typename D>
	void MapBuffer(const CT_GRAPHIC& Gfx, const D& Data, ID3D11Resource* pBuffer) const	//バッファ更新
	{
		//例外処理
		if (pBuffer == nullptr)
			throw ERROR_EX2("ポインタはNULLです。");

		//エラーハンドル
		HRESULT hr{};

		//更新処理
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);								//GPUのアクセスをロック
		ERROR_DX(hr);
		memcpy(msr.pData, &Data, sizeof(D));	//データ書込み
		GetContext(Gfx)->Unmap(pBuffer, 0u);	//GPUのアクセスを解放
	}

	template <typename D>
	void MapBuffer(const CT_GRAPHIC& Gfx, const std::vector<D>& aData, ID3D11Resource* pBuffer) const	//バッファ更新(配列)
	{
		//例外処理
		if (pBuffer == nullptr)
			throw ERROR_EX2("ポインタはNULLです。");

		//エラーハンドル
		HRESULT hr{};

		//更新処理
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);													//GPUのアクセスをロック
		ERROR_DX(hr);
		memcpy(msr.pData, aData.data(), sizeof(D) * aData.size());	//データ書込み
		GetContext(Gfx)->Unmap(pBuffer, 0u);						//GPUのアクセスを解放
	}

	static ID3D11Device* GetDevice(const CT_GRAPHIC& Gfx)				//デバイス取得
	{
		//例外処理
		if (Gfx.m_pDevice == nullptr)
			throw ERROR_EX2("ポインタはNULLです。");

		return Gfx.m_pDevice.Get();
	}

	static ID3D11DeviceContext* GetContext(const CT_GRAPHIC& Gfx)		//コンテキスト取得
	{
		//例外処理
		if (Gfx.m_pContext == nullptr)
			throw ERROR_EX2("ポインタはNULLです。");

		return Gfx.m_pContext.Get();
	}
};
