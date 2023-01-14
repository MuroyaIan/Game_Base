/**
 * @file cbMgr.h
 * @brief 定数バッファマネージャ
 * @author 室谷イアン
 * @date 2023/01/13
 * @履歴 2023/01/13：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== クラス定義 =====
class CBUFF_MGR : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CBUFF_MGR(CB_PTR& cbPtrRef) noexcept;
	~CBUFF_MGR() noexcept override;

	void Bind(const GRAPHIC& Gfx) const noexcept override;	//バインド処理

private:

	//変数宣言
	std::vector<ID3D11Buffer*> m_aBuffPtrVS;
	std::vector<ID3D11Buffer*> m_aBuffPtrPS;	//定数バッファのポインタ配列
	UINT m_BuffSizeVS;
	UINT m_BuffSizePS;							//定数バッファのサイズ
};
