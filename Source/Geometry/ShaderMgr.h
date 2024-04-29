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
class CT_BINDER;
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
		VS_Default,
		VS_VtxBlend,
		VS_Texture,

		VS_Model,
		VS_Model_Normal,
		VS_Model_Bone,

		VS_Instance,
		VS_Instance_VtxBlend,
		VS_Instance_Texture,
		VS_Instance_Phong,
		VS_Instance_Phong_Anim,
		VS_Instance_Phong_Anim_NoSkin,
		VS_Instance_PhongN,

		//入力レイアウト
		IL_Default,
		IL_VtxBlend,
		IL_Texture,

		IL_Model,
		IL_Model_Normal,
		IL_Model_Bone,

		IL_Instance,
		IL_Instance_VtxBlend,
		IL_Instance_Texture,
		IL_Instance_Phong,
		IL_Instance_Phong_Anim,
		IL_Instance_Phong_Anim_NoSkin,
		IL_Instance_PhongN,

		//プリミティブトポロジー
		PT_Line,
		PT_Tri,

		//サンプラー
		Sampler,

		//ピクセルシェーダ
		PS_Default,
		PS_OneColor,
		PS_VtxBlend,
		PS_Texture,

		PS_Model,
		PS_Model_Normal,
		PS_Model_Disp,

		PS_Phong,

		//定数バッファ
		CB_VS_MtxVP,
		CB_PS_Default,
		CB_Light,

		ID_Max
	};

	//プロトタイプ宣言
	explicit SHADER_MGR(CT_GRAPHIC& Gfx);
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

	CT_BINDER& GetBinder(BINDER_ID id) const noexcept		//バインダ参照を取得
	{
		return *m_aBinder[static_cast<int>(id)];
	}

	void SetConstBufferPtr(BINDER_ID id, CB_PTR* cbPtr) const;	//定数バッファポインタ登録

private:

	//変数宣言
	CT_GRAPHIC& m_DX;										//DX参照
	std::vector<std::unique_ptr<CT_BINDER>> m_aBinder;		//バインダのポインタ配列
};
