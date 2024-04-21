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
#include <wrl.h>					//COMポインタ
#include <dxgi1_6.h>				//DXGI
#include <GraphicApp/GfxConfig.h>	//Gfxデバッグ
#include <d3dcompiler.h>			//シェーダコンパイル
#include <DirectXMath.h>
#include <Startup.h>				//デバッグ識別

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>				//DXGIデバッグ

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
	explicit CT_GRAPHIC(CT_IF_WINDOW& pWindow, float fWidth, float fHeight);
	~CT_GRAPHIC() noexcept(!gc_IS_DEBUG);
	void SetResolution(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth, const int& nWndHeight);	//解像度設定
	void BeginFrame(const float& r, const float& g, const float& b) const noexcept;								//フレーム開始
	void DrawIndexed(const UINT& indexNum) const noexcept;														//インデックス描画
	void DrawInstanced(const UINT& indexNum, const UINT& instanceNum) const noexcept;							//インスタンシング描画
	void EndFrame() const;																						//フレーム終了⇒描画開始
	void SetDrawMode(const ET_DRAW_MODE& mode) const noexcept;													//描画モード設定

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
	ComPtr<ID3D11Device> m_pDevice;							//デバイス
	ComPtr<ID3D11DeviceContext> m_pContext;					//コンテキスト
	ComPtr<IDXGISwapChain> m_pSwapChain;					//スワップチェーン
	ComPtr<ID3D11Device1> m_pDevice1;
	ComPtr<ID3D11DeviceContext1> m_pContext1;
	ComPtr<IDXGISwapChain1> m_pSwapChain1;					//[DX11.1]バージョン用
	bool m_Version_11_1;									//バージョン確認用

	ComPtr<ID3D11RenderTargetView> m_pView_RenderTarget;	//RTV
	ComPtr<ID3D11DepthStencilView> m_pView_DepthStencil;	//DSV
	ComPtr<ID3D11Texture2D> m_pBuffer_DepthStencil;			//DSバッファ
    D3D11_VIEWPORT m_Viewport;								//ビューポート

	UINT m_MsaaQuality;										//MSAAのサポートレベル
	bool m_EnableMsaa;										//MSAA使用

	CT_IF_WINDOW& m_pWindow;								//Winクラスの参照
	dx::XMFLOAT4X4 m_MtxView;								//ビュー行列（カメラ）
	dx::XMFLOAT4X4 m_MtxProjection;							//投影行列

#ifdef IMGUI

	bool m_bDrawImGui;										//ImGUI描画制御

#endif // IMGUI

	//プロトタイプ宣言
	void UpdateResolution(const float& fWidth, const float& fHeight);								//解像度更新
	static void InitGfxCard(const ComPtr<IDXGIFactory>& pFactory, ComPtr<IDXGIAdapter>& pAdapter);	//グラボ初期化

	//権限指定
	friend class BINDER;
};
