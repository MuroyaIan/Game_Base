/**
 * @file Topology.h
 * @brief トポロジーの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/21：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** トポロジー *****
class TOPOLOGY : public BINDER
{
public:

	//プロトタイプ宣言
	explicit TOPOLOGY(D3D11_PRIMITIVE_TOPOLOGY Type) noexcept;
	~TOPOLOGY() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;	//バインド処理

protected:

	//変数宣言
	D3D11_PRIMITIVE_TOPOLOGY m_Type;						//トポロジータイプ
};
