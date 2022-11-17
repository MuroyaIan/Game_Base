/**
 * @file IndexBuffer.h
 * @brief インデックスバッファの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** インデックスバッファ *****
class INDEX_BUFFER : public BINDER
{
public:

	//プロトタイプ宣言
	INDEX_BUFFER(GRAPHIC& Gfx, const std::vector<unsigned short>& Indices);
	~INDEX_BUFFER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;				//バインド処理

	UINT GetIndexNum() const noexcept						//インデックス数取得
	{
		return m_Count;
	}

protected:

	//変数宣言
	UINT m_Count;											//インデックス数
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;	//ポインタ
};
