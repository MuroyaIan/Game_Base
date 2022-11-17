/**
 * @file Drawer.h
 * @brief DirectXの描画処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Graphic.h>
#include <typeinfo>					//ポインタ内データの型確認

//===== 前方宣言 =====
class BINDER;
class INDEX_BUFFER;
class VERTEX_BUFFER;

//===== クラス定義 =====

//***** ドロワー *****
class DRAWER
{
public:

	//コピーNG
	DRAWER(const DRAWER&) = delete;

	//プロトタイプ宣言
	DRAWER() noexcept;
	virtual ~DRAWER() noexcept;
	UINT GetIndexNum() const noexcept;														//インデックス数取得

	virtual void Update() noexcept = 0;														//更新処理
	virtual void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG);	//書込み処理
	virtual DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept = 0;	//変形行列取得
	virtual UINT GetPolygonNum() const noexcept = 0;										//ポリゴン数取得
	virtual int AddInstance()																//インスタンス追加
	{
		throw ERROR_EX2("子クラスでインスタンス化行ってください。");
	}
	virtual void ClearInstance()															//インスタンスクリア
	{
		throw ERROR_EX2("子クラスでインスタンスをクリアしてください。");
	}

protected:

	//プロトタイプ宣言
	void AddBind(std::unique_ptr<BINDER> pBinder);		//バインダ登録

	void SetInstanceNum(int Num)						//インスタンス数設定
	{
		if (Num < 0)
			throw ERROR_EX2("インスタンス数は1以上でなければならない。");
		m_InstanceNum = Num;
	}

	VERTEX_BUFFER& GetVertexBuffer() const		//頂点バッファ参照
	{
		return *m_pVertexBuffer;
	}

	virtual const std::vector<std::unique_ptr<BINDER>>& GetStaticBind() const noexcept = 0;		//静的配列参照
	virtual const INDEX_BUFFER& GetStaticIndexBuffer() const noexcept = 0;						//静的インデックスバッファ参照

private:

	//変数宣言
	std::vector<std::unique_ptr<BINDER>> m_aBinder;		//バインダのポインタ配列
	const INDEX_BUFFER* m_pIndexBuffer;					//インデックスバッファのポインタ（所有権なし）
	int m_InstanceNum;									//インスタンス数
	VERTEX_BUFFER* m_pVertexBuffer;						//頂点バッファのポインタ（インスタンス更新用）
};
