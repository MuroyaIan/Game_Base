/**
 * @file ShaderMgr.h
 * @brief シェーダ管理
 * @author 室谷イアン
 * @date 2022/07/21
 * @履歴 2022/07/21：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Graphic.h>

//===== 前方宣言 =====
class BINDER;
struct CB_PTR;

//===== クラス定義 =====

//***** シェーダ管理 *****
class SHADER_MGR
{
public:

	//バインダ一覧
	enum class BINDER_ID
	{
		//頂点シェーダ
		VS_DEFAULT,
		VS_VTX_BLEND,
		VS_TEXTURE,

		VS_MODEL,
		VS_MODEL_NORMAL,
		VS_MODEL_BONE,

		VS_INSTANCE,
		VS_INSTANCE_VTX_BLEND,
		VS_INSTANCE_TEXTURE,
		VS_INSTANCE_PHONG,
		VS_INSTANCE_PHONG_ANIM,

		//入力レイアウト
		IL_DEFAULT,
		IL_VTX_BLEND,
		IL_TEXTURE,

		IL_MODEL,
		IL_MODEL_NORMAL,
		IL_MODEL_BONE,

		IL_INSTANCE,
		IL_INSTANCE_VTX_BLEND,
		IL_INSTANCE_TEXTURE,
		IL_INSTANCE_PHONG,
		IL_INSTANCE_PHONG_ANIM,

		//プリミティブトポロジー
		PT_LINE,
		PT_TRI,

		//サンプラー
		SAMPLER,

		//ピクセルシェーダ
		PS_DEFAULT,
		PS_ONE_COLOR,
		PS_VTX_BLEND,
		PS_TEXTURE,

		PS_MODEL,
		PS_MODEL_NORMAL,

		PS_PHONG,

		//定数バッファ
		CB_VS_MTX_VP,
		CB_PS_DEFAULT,
		CB_PS_LIGHT,

		ID_MAX
	};

	//プロトタイプ宣言
	SHADER_MGR(GRAPHIC& Gfx);
	~SHADER_MGR() noexcept;
	void Bind(BINDER_ID id) const noexcept;				//バインド処理
	void Bind_Default() const noexcept;					//バインド処理（デフォルト）
	void Bind_VertexBlend() const noexcept;				//バインド処理（頂点ブレンド）
	void Bind_Texture() const noexcept;					//バインド処理（テクスチャ）
	void Bind_Instance() const noexcept;				//バインド処理（インスタンシング）
	void Bind_Instance_VertexBlend() const noexcept;	//バインド処理（インスタンシング＿頂点ブレンド）
	void Bind_Instance_Texture() const noexcept;		//バインド処理（インスタンシング＿テクスチャ）
	void Bind_Instance_Phong() const noexcept;			//バインド処理（インスタンシング＿Phongモデル）
	void Bind_Instance_Phong_Anim() const noexcept;		//バインド処理（インスタンシング＿Phongモデル＿アニメ付）

	BINDER& GetBinder(BINDER_ID id) const noexcept		//バインダ参照を取得
	{
		return *m_aBinder[static_cast<int>(id)];
	}

	void SetConstBufferPtr(BINDER_ID id, CB_PTR* cbPtr) const;	//定数バッファポインタ登録

private:

	//変数宣言
	GRAPHIC& m_DX;										//DX参照
	std::vector<std::unique_ptr<BINDER>> m_aBinder;		//バインダのポインタ配列
};
