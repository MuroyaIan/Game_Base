/**
 * @file TextureMgr.h
 * @brief テクスチャ管理
 * @author 室谷イアン
 * @date 2022/08/12
 * @履歴 2022/08/12：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Tool/TextureLoader.h>
#include <GraphicApp/Graphic.h>

//===== 前方宣言 =====
class BINDER;

//===== クラス定義 =====

//***** テクスチャ管理 *****
class TEXTURE_MGR
{
public:

	//テクスチャ識別番号
	enum class TEX_ID
	{
		TEX_Null,
		TEX_TestPlane,
		TEX_TestBox,

		ID_Max
	};

	//テクスチャパック
	struct TEX_PACK
	{
		TEX_LOADER::TEX_DATA TexData;		//テクスチャ情報
		std::unique_ptr<BINDER> pBinder;	//バインダのポインタ
		int nUsedCount;						//使用数

		TEX_PACK() noexcept;
		~TEX_PACK() noexcept;
	};

	//プロトタイプ宣言
	explicit TEXTURE_MGR(GRAPHIC& Gfx);
	~TEXTURE_MGR() noexcept;
	void SetTextureOn(TEX_ID id);
	void SetTextureOff(TEX_ID id) noexcept;		//バッファ利用開始・終了
	void Bind(TEX_ID id) const;					//バインド処理

	TEX_PACK& GetTexPack(TEX_ID id) noexcept	//テクスチャパック参照
	{
		return m_aTexPack[static_cast<int>(id)];
	}

private:

	//変数宣言
	std::vector<TEX_PACK> m_aTexPack;			//テクスチャパック配列
	GRAPHIC& m_DX;								//DX参照

	static std::string aFilePath[static_cast<int>(TEX_ID::ID_Max)];
};
