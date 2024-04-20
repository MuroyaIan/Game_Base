/**
 * @file Graphic.h
 * @brief DirectX処理
 * @author 室谷イアン
 * @date 2022/06/12
 * @履歴 2022/06/12：ファイル作成
 *		 2022/11/21：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/WinWindow.h>
#include <wrl.h>				//COMポインタ
#include <dxgi1_6.h>
#include <d3d11.h>
#include <d3dcompiler.h>		//シェーダコンパイル
#include <DirectXMath.h>
#include <Startup.h>			//デバッグ識別

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>			//DXGIデバッグ

#endif // _DEBUG

//===== 定数・マクロ定義 =====
#define ERROR_DX(hr) if((hr) != S_OK) throw ERROR_EX(hr)	//エラー出力(DirectX用)

//===== エイリアス宣言 =====
template <class t_PtrType>
using ComPtr = Microsoft::WRL::ComPtr<t_PtrType>;			//COMポインター

//===== 名前空間 =====
namespace dx = DirectX;

//===== クラス定義 =====

//***** グラフィック *****
class CT_GRAPHIC
{
public:

	//描画モード
	enum class ET_DRAW_MODE
	{
		me_Draw_2D,	//2D
		me_Draw_3D	//3D
	};

	//コピー＆ムーブ
	CT_GRAPHIC(const CT_GRAPHIC&) = delete;
	CT_GRAPHIC& operator =(const CT_GRAPHIC&) = delete;
	CT_GRAPHIC(CT_GRAPHIC&&) noexcept = default;
	CT_GRAPHIC& operator=(CT_GRAPHIC&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_GRAPHIC(HWND hWindow, float fWidth, float fHeight);
	~CT_GRAPHIC() noexcept(!gc_IS_DEBUG);
	void BeginFrame(float r, float g, float b) const noexcept;				//フレーム開始
	void DrawIndexed(UINT indexNum) const noexcept;							//インデックス描画
	void DrawInstanced(UINT indexNum, UINT instanceNum) const noexcept;		//インスタンシング描画
	void EndFrame() const;													//フレーム終了⇒描画開始
	void SetDrawMode(ET_DRAW_MODE mode) const noexcept;						//描画モード設定

	void SetViewMtx(const dx::XMFLOAT4X4& mtxView) noexcept			//ビュー行列設定
	{
		m_MtxView = mtxView;
	}
	[[nodiscard]] dx::XMFLOAT4X4 GetViewMtx() const noexcept		//ビュー行列取得
	{
		return m_MtxView;
	}
	void SetProjectionMtx(const dx::XMFLOAT4X4& mtxProj) noexcept	//投影行列設定
	{
		m_MtxProjection = mtxProj;
	}
	[[nodiscard]] dx::XMFLOAT4X4 GetProjectionMtx() const noexcept	//投影行列取得
	{
		return m_MtxProjection;
	}

#ifdef IMGUI

	void SetImGuiMode(const bool bEnable) noexcept					//ImGui描画ON/OFF
	{
		m_bDrawImGui = bEnable;
	}

	[[nodiscard]] bool IsImGuiEnabled() const noexcept				//ImGui描画状態確認
	{
		return m_bDrawImGui;
	}

#endif // IMGUI

private:

	//変数宣言
	ComPtr<IDXGIAdapter> m_pAdapter;						//アダプター
	ComPtr<ID3D11Device> m_pDevice;							//デバイス
	ComPtr<IDXGISwapChain> m_pSwapChain;					//スワップチェーン
	ComPtr<ID3D11DeviceContext> m_pContext;					//コンテキスト
	ComPtr<ID3D11RenderTargetView> m_pView_RenderTarget;	//ターゲットビュー
	ComPtr<ID3D11DepthStencilView> m_pView_DepthStencil;	//深度・ステンシルビュー

	dx::XMFLOAT4X4 m_MtxView;			//ビュー行列（カメラ）
	dx::XMFLOAT4X4 m_MtxProjection;		//投影行列

#ifdef IMGUI

	bool m_bDrawImGui;					//ImGUI描画制御

#endif // IMGUI

	//プロトタイプ宣言
	void InitDxgi();					//DXGI初期化

	//権限指定
	friend class BINDER;
};
