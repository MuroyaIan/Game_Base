/**
 * @file cbMaterial.h
 * @brief マテリアルバッファ
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/25：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== 前方宣言 =====
struct CBD_MATERIAL;

//===== クラス定義 =====

//***** マテリアルバッファ *****
class CB_MATERIAL : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CB_MATERIAL(const GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MATERIAL& Material);
	~CB_MATERIAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//バインド処理

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept						//バッファポインタ設定
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//変数宣言
	static std::unique_ptr<CONSTANT_BUFFER<CBD_MATERIAL>> m_pCBuff;		//定数バッファのポインタ
	static int m_RefCount;												//定数バッファの利用数
	const CBD_MATERIAL& m_Material;										//マテリアル情報の提供先
};
