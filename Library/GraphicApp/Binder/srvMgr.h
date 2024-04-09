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
#include <GraphicApp/Binder/Binder.h>

//===== 構造体宣言 =====
struct SRV_PTR												//SRVのポインタ情報
{
	//変数宣言
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrVS;
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrPS;		//ポインタ配列

	SRV_PTR() noexcept : m_aSrvPtrVS(0), m_aSrvPtrPS(0)
	{}

	~SRV_PTR() noexcept
	{}
};

//===== クラス定義 =====
class SRV_MGR : public BINDER
{
public:

	//プロトタイプ宣言
	explicit SRV_MGR(SRV_PTR& srvPtrRef) noexcept;
	~SRV_MGR() noexcept override;

	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理

private:

	//変数宣言
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrVS;
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrPS;		//SRVのポインタ配列
	UINT m_SrvSizeVS;
	UINT m_SrvSizePS;										//SRVのサイズ
};
