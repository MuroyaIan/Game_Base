/**
 * @file IndexBuffer.h
 * @brief インデックスバッファの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/22：処理改善
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
	explicit INDEX_BUFFER(const GRAPHIC& Gfx, const std::vector<UINT>& aIndex);
	~INDEX_BUFFER() noexcept override;
	void Bind(const GRAPHIC& Gfx) const override;	//バインド処理

	UINT GetIndexNum() const noexcept				//インデックス数取得
	{
		return m_IndexNum;
	}

private:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;	//ポインタ
	UINT m_IndexNum;										//インデックス数
};
