/**
 * @file ModelRef.h
 * @brief モデルマネージャ
 * @author 室谷イアン
 * @date 2022/09/08
 * @履歴 2022/09/08：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Graphic.h>
#include <Tool/TextureLoader.h>

//===== 前方宣言 =====
namespace ModelRef {
	struct MODEL_PACK;
}
class TEXTURE;
class TEXTURE_ANIM;

//===== クラス定義 =====

//***** モデルローダ *****
class MODEL_MGR
{
public:

	//モデル番号
	enum class MODEL_ID
	{
		Tarantella,
		Unity,
		Cube,

		ID_Max
	};

	//テクスチャパック
	struct TEX_PACK
	{
		std::vector<std::string> aName;						//テクスチャ名
		std::vector<TEX_LOADER::TEX_DATA> aTexData;			//テクスチャ情報
		std::vector<std::unique_ptr<TEXTURE>> aTexBuffPtr;	//テクスチャバッファのポインタ
		std::unique_ptr<TEXTURE_ANIM> AnimBuffPtr;			//アニメーションバッファのポインタ
		std::vector<int> aUsedCount;						//使用数

		TEX_PACK() noexcept;
		~TEX_PACK() noexcept;
	};

	//プロトタイプ宣言
	explicit MODEL_MGR(CT_GRAPHIC& Gfx);
	~MODEL_MGR() noexcept;
	ID3D11ShaderResourceView* SetTextureOn(MODEL_ID id, std::string TexName);
	void SetTextureOff(MODEL_ID id, std::string TexName);						//テクスチャバッファ利用開始・終了
	ID3D11ShaderResourceView* SetAnimTexOn(MODEL_ID id);
	void SetAnimTexOff(MODEL_ID id) noexcept;									//アニメーションバッファ利用開始・終了

	ModelRef::MODEL_PACK& GetModelPack(MODEL_ID id) const noexcept				//モデルパック参照
	{
		return *m_aModelPackPtr[static_cast<int>(id)];
	}

private:

	//変数宣言
	std::vector<std::unique_ptr<ModelRef::MODEL_PACK>> m_aModelPackPtr;				//モデルパック配列
	std::vector<TEX_PACK> m_aTexPack;												//テクスチャパック配列
	CT_GRAPHIC& m_DX;																	//DX参照

	static std::string aModelName[static_cast<int>(MODEL_ID::ID_Max)];				//モデル名配列
	static std::vector<std::string> aAnimName[static_cast<int>(MODEL_ID::ID_Max)];	//アニメーション名配列
	static std::vector<bool> aAnimFPS_30[static_cast<int>(MODEL_ID::ID_Max)];		//アニメーションFPSフラグ

	//プロトタイプ宣言
	void LoadModel(MODEL_ID id);													//モデル読込
	void LoadTextureName(std::string TexName, TEX_PACK& DataRef) const noexcept;	//テクスチャ名読込
};
