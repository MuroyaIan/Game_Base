/**
 * @file DrawerRef.h
 * @brief ドロワーのテンプレ
 * @author 室谷イアン
 * @date 2022/07/05
 * @履歴 2022/07/05：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Binder/IndexBuffer.h>

//===== クラス定義 =====

//***** ドロワーテンプレ *****
template<class T>
class DRAWER_EX : public DRAWER
{
protected:

	//プロトタイプ宣言
	DRAWER_EX() noexcept : DRAWER() {}
	~DRAWER_EX() noexcept override
	{
		for (auto& b : ms_aBinder)
			b.reset();
	}

	bool StaticIsInit() const noexcept						//初期化確認
	{
		return !ms_aBinder.empty();
	}

	void AddStaticBind(std::unique_ptr<BINDER> pBinder)		//バインダ登録（静的）
	{
		//例外処理
		if (pBinder == nullptr)
			throw ERROR_EX2("【警告】ポインタはNULLです！");

		//複数インデックスのバインドを防止
		if (typeid(*pBinder) == typeid(INDEX_BUFFER)) {
			if (ms_pIndexBuffer != nullptr)
				throw ERROR_EX2("【警告】m_pIndexBufferは上書きされます！");
			ms_pIndexBuffer = dynamic_cast<INDEX_BUFFER*>(pBinder.get());
		}

		//所有権移行
		ms_aBinder.push_back(std::move(pBinder));
	}

private:

	//型省略
	using vec_upBinder = std::vector<std::unique_ptr<BINDER>>;

	//変数宣言
	static vec_upBinder ms_aBinder;										//バインダのポインタ配列（静的）
	static const INDEX_BUFFER* ms_pIndexBuffer;							//インデックスバッファのポインタ（所有権なし、重複利用）

	//プロトタイプ宣言
	const vec_upBinder& GetStaticBind() const noexcept override			//静的配列参照
	{
		return ms_aBinder;
	}

	const INDEX_BUFFER& GetStaticIndexBuffer() const noexcept override	//静的インデックスバッファ参照
	{
		return *ms_pIndexBuffer;
	}
};

//===== 静的メンバ変数 =====
template<class T>
std::vector<std::unique_ptr<BINDER>> DRAWER_EX<T>::ms_aBinder{};

template<class T>
const INDEX_BUFFER* DRAWER_EX<T>::ms_pIndexBuffer = nullptr;
