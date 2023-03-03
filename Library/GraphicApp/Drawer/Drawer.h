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
	DRAWER& operator=(const DRAWER&) = delete;

	//プロトタイプ宣言
	explicit DRAWER(const GRAPHIC& Gfx) noexcept;
	virtual ~DRAWER() noexcept;

	virtual void Update() = 0;																	//更新処理
	virtual void Draw(int InstanceNum = 0);														//描画処理
	virtual UINT GetPolygonNum() const noexcept = 0;											//ポリゴン数取得
	virtual DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept = 0;		//ワールド行列取得
	virtual void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept = 0;	//ワールド行列設定

	virtual int AddInstance()																	//インスタンス追加
	{
		throw ERROR_EX2("使用する場合は子クラスで継承してください。");
	}

	virtual void ClearInstance()																//インスタンスクリア
	{
		throw ERROR_EX2("使用する場合は子クラスで継承しください。");
	}

protected:

	//変数宣言
	const GRAPHIC& m_Gfx;								//グラフィック参照先

	//プロトタイプ宣言
	void AddBind(std::unique_ptr<BINDER> pBinder);		//バインダ登録
	UINT GetIndexNum() const noexcept;					//インデックス数取得

	VERTEX_BUFFER& GetVertexBuffer() const noexcept		//頂点バッファ参照
	{
		return *m_pVertexBuffer;
	}

	void SetInstanceNum(int Num)						//インスタンス数設定
	{
		if (Num < 0)
			throw ERROR_EX2("インスタンス数は1以上でなければならない。");
		m_MaxInstanceNum = Num;
	}

private:

	//変数宣言
	std::vector<std::unique_ptr<BINDER>> m_aBinder;		//バインダのポインタ配列
	const INDEX_BUFFER* m_pIndexBuffer;					//インデックスバッファのポインタ（所有権なし）
	VERTEX_BUFFER* m_pVertexBuffer;						//頂点バッファのポインタ（インスタンス更新用）
	int m_MaxInstanceNum;								//最大インスタンス数

	//プロトタイプ宣言
	void DrawProc(int InstanceNum) const noexcept;		//描画処理（最終処理）
};
