/**
 * @file VertexShader.h
 * @brief 頂点シェーダの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
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
	VERTEX_SHADER(GRAPHIC& Gfx, const std::wstring& Path);
	~VERTEX_SHADER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;						//バインド処理

	ID3DBlob* GetBytecode() const noexcept							//シェーダファイル取得
	{
		return m_pBytecodeBlob.Get();
	}

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob;				//シェーダファイル読込み用
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;		//ポインタ
};
