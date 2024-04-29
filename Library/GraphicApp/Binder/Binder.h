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
class CT_BINDER
{
public:

	//コピー＆ムーブ
	CT_BINDER(const CT_BINDER&) = delete;
	CT_BINDER& operator =(const CT_BINDER&) = delete;
	CT_BINDER(CT_BINDER&&) noexcept = delete;
	CT_BINDER& operator=(CT_BINDER&&) noexcept = delete;

	//プロトタイプ宣言
	explicit CT_BINDER() noexcept;
	virtual ~CT_BINDER() noexcept;
	virtual void Bind(const CT_GRAPHIC& gfx) const = 0;	//バインド処理

protected:

	//プロトタイプ宣言
	template <typename t_Data>
	static void MapBuffer(const CT_GRAPHIC& gfx, const t_Data& data, ID3D11Resource* pBuffer) //バッファ更新
	{
		//例外処理
		if (pBuffer == nullptr)
			throw ERROR_EX2("ポインタ(pBuffer)はNULLです。");

		//エラーハンドル
		HRESULT l_hr{};

		//更新処理
		D3D11_MAPPED_SUBRESOURCE l_msr{};
		l_hr = GetContext(gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&l_msr);								//GPUのアクセスをロック
		ERROR_DX(l_hr);
		memcpy(l_msr.pData, &data, sizeof(t_Data));	//データ書込み
		GetContext(gfx)->Unmap(pBuffer, 0u);	//GPUのアクセスを解放
	}

	template <typename t_Data>
	static void MapBuffer(const CT_GRAPHIC& gfx, const std::vector<t_Data>& aData, ID3D11Resource* pBuffer) //バッファ更新(配列)
	{
		//例外処理
		if (pBuffer == nullptr)
			throw ERROR_EX2("ポインタ(pBuffer)はNULLです");

		//エラーハンドル
		HRESULT l_hr{};

		//更新処理
		D3D11_MAPPED_SUBRESOURCE l_msr{};
		l_hr = GetContext(gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&l_msr);													//GPUのアクセスをロック
		ERROR_DX(l_hr);
		memcpy(l_msr.pData, aData.data(), sizeof(t_Data) * aData.size());	//データ書込み
		GetContext(gfx)->Unmap(pBuffer, 0u);						//GPUのアクセスを解放
	}

	static ID3D11Device* GetDevice(const CT_GRAPHIC& gfx)				//デバイス取得
	{
		//例外処理
		if (gfx.m_pDevice == nullptr)
			throw ERROR_EX2("ポインタ(m_pDevice)はNULLです");

		return gfx.m_pDevice.Get();
	}

	static ID3D11DeviceContext* GetContext(const CT_GRAPHIC& gfx)		//コンテキスト取得
	{
		//例外処理
		if (gfx.m_pContext == nullptr)
			throw ERROR_EX2("ポインタ(m_pDevice)はNULLです");

		return gfx.m_pContext.Get();
	}
};
