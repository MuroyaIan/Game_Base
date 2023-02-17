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

//===== 前方宣言 =====
namespace ModelRef {
	struct MODEL_PACK;
	struct TEX_PACK;
}

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

		ID_Max
	};

	//プロトタイプ宣言
	MODEL_MGR();
	~MODEL_MGR() noexcept;

	ModelRef::MODEL_PACK& GetModelPack(MODEL_ID id) const	//モデルパック参照
	{
		return *m_aModelPackPtr[static_cast<int>(id)];
	}

private:

	//変数宣言
	std::vector<std::unique_ptr<ModelRef::MODEL_PACK>> m_aModelPackPtr;				//モデルパック配列

	static std::string aModelName[static_cast<int>(MODEL_ID::ID_Max)];				//モデル名配列
	static std::vector<std::string> aAnimName[static_cast<int>(MODEL_ID::ID_Max)];	//アニメーション名配列
	static std::vector<bool> aAnimFPS_30[static_cast<int>(MODEL_ID::ID_Max)];		//アニメーションFPSフラグ

	//プロトタイプ宣言
	void LoadModel(MODEL_ID id);															//モデル読込
	void LoadTextureName(std::string TexName, std::vector<ModelRef::TEX_PACK>& DataRef);	//テクスチャ名読込
};
