/**
 * @file cbMaterial.h
 * @brief マテリアルバッファ
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== 前方宣言 =====
struct MATERIAL_DATA;

//===== クラス定義 =====

//***** マテリアルバッファ *****
class CB_MATERIAL : public BINDER
{
public:

	//プロトタイプ宣言
	CB_MATERIAL(GRAPHIC& Gfx, const MATERIAL_DATA& Material);
	~CB_MATERIAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//バインド処理

private:

	//変数宣言
	static std::unique_ptr<PIXEL_CBUFFER<MATERIAL_DATA>> m_pPcBuff;		//定数バッファのポインタ
	static int m_RefCount;												//定数バッファの利用数
	const MATERIAL_DATA& m_Material;									//マテリアル情報の提供先
};
