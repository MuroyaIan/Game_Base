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
#include <dxgi1_6.h>
#include <d3d11.h>
#include <d3dcompiler.h>			//シェーダコンパイル
#include <DirectXMath.h>
#include <Startup.h>				//デバッグ識別

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>				//DXGIデバッグ

#endif // _DEBUG

//===== 定数・マクロ定義 =====
#define ERROR_DX(hr) if(hr != S_OK) throw ERROR_EX(hr)	//エラー出力(DirectX用)

//===== クラス定義 =====

//***** グラフィック *****
class GRAPHIC
{
public:

	//描画モード
	enum class DRAW_MODE
	{
		DRAW_2D,
		DRAW_3D
	};

	//コピーNG
	GRAPHIC(const GRAPHIC&) = delete;
	GRAPHIC& operator=(const GRAPHIC&) = delete;

	//プロトタイプ宣言
	explicit GRAPHIC(HWND hWindow, float fWidth, float fHeight);
	~GRAPHIC() noexcept(!IS_DEBUG);
	void BeginFrame(float R, float G, float B) const noexcept;				//フレーム開始
	void DrawIndexed(UINT IndexNum) const noexcept;							//インデックス描画
	void DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept;		//インスタンシング描画
	void EndFrame() const;													//フレーム終了⇒描画開始
	void SetDrawMode(DRAW_MODE Mode) const noexcept;						//描画モード設定

	void SetViewMtx(DirectX::XMFLOAT4X4 mtxView) noexcept			//ビュー行列へのアクセス
	{
		m_mtxView = mtxView;
	}
	DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
	{
		return m_mtxView;
	}
	void SetProjectionMtx(DirectX::XMFLOAT4X4 mtxProj) noexcept		//投影行列へのアクセス
	{
		m_mtxProjection = mtxProj;
	}
	DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
	{
		return m_mtxProjection;
	}

#ifdef IMGUI

	void SetImGuiMode(bool bEnable) noexcept						//ImGui描画ON/OFF
	{
		m_bDrawImGui = bEnable;
	}

	bool IsImGuiEnabled() const noexcept							//ImGui描画状態確認
	{
		return m_bDrawImGui;
	}

#endif // IMGUI

private:

	//変数宣言
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter;				//アダプター
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;					//デバイス
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;			//スワップチェーン
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;			//コンテキスト
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTView;		//ターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;		//深度・ステンシルビュー

	DirectX::XMFLOAT4X4 m_mtxView;			//ビュー行列（カメラ）
	DirectX::XMFLOAT4X4 m_mtxProjection;	//投影行列

#ifdef IMGUI

	bool m_bDrawImGui;						//ImGUI描画制御

#endif // IMGUI

	//プロトタイプ宣言
	void InitDXGI();						//DXGI初期化

	//権限指定
	friend class BINDER;
};
