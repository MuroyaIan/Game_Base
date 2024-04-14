/**
 * @file VertexShader.h
 * @brief 頂点シェーダの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/24：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** 頂点シェーダ *****
class VERTEX_SHADER : public BINDER
{
public:

	//プロトタイプ宣言
	explicit VERTEX_SHADER(const CT_GRAPHIC& Gfx, const std::wstring& Path);
	~VERTEX_SHADER() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理

	ID3DBlob* GetBytecode() const					//シェーダファイル取得
	{
		//例外処理
		if (m_pBlob == nullptr)
			throw ERROR_EX2("ポインタはNULLです。");

		return m_pBlob.Get();
	}

private:

	//変数宣言
	ComPtr<ID3DBlob> m_pBlob;						//シェーダリソース
	ComPtr<ID3D11VertexShader> m_pVertexShader;		//ポインタ
};
