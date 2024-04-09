
//===== インクルード部 =====
#include <GraphicApp/Graphic.h>

#ifdef IMGUI
#
#	include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#	include <3rd_Party/ImGui/backends/imgui_impl_dx11.h>
#
#endif // IMGUI

//===== 追加ライブラリ =====
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dCompiler.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//===== クラス実装 =====
CT_GRAPHIC::CT_GRAPHIC(HWND hWindow, float fWidth, float fHeight) :
	m_pAdapter(), m_pDevice(), m_pSwapChain(), m_pContext(), m_pView_RenderTarget(), m_pView_DepthStencil(),
	m_MtxView(), m_MtxProjection()
{
	//エラーハンドル
	HRESULT l_hr{};

	//DXGI初期化
	InitDxgi();

	//FeatureLevel設定
	D3D_FEATURE_LEVEL l_FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT l_NumFeatureLevels = ARRAYSIZE(l_FeatureLevels);
	D3D_FEATURE_LEVEL l_FeatureLevel{};

	//スワップチェーン設定
	DXGI_SWAP_CHAIN_DESC l_scd{};
	l_scd.BufferDesc.Width = static_cast<UINT>(fWidth);
	l_scd.BufferDesc.Height = static_cast<UINT>(fHeight);
	l_scd.BufferDesc.RefreshRate.Numerator = 0u;
	l_scd.BufferDesc.RefreshRate.Denominator = 0u;
	l_scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	l_scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	l_scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	l_scd.SampleDesc.Count = 1u;
	l_scd.SampleDesc.Quality = 0u;
	l_scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	l_scd.BufferCount = 1u;
	l_scd.OutputWindow = hWindow;
	l_scd.Windowed = TRUE;
	l_scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	l_scd.Flags = 0u;

	//デバッグ設定
	UINT l_CreateDeviceFlag = 0u;

#ifdef _DEBUG

	l_CreateDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	//デバイス・スワップチェーン初期化
	l_hr = D3D11CreateDeviceAndSwapChain(
		m_pAdapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		l_CreateDeviceFlag,
		l_FeatureLevels,
		l_NumFeatureLevels,
		D3D11_SDK_VERSION,
		&l_scd,
		&m_pSwapChain,
		&m_pDevice,
		&l_FeatureLevel,
		&m_pContext
	);
	//l_hr = D3D11CreateDeviceAndSwapChain(
	//	nullptr,
	//	D3D_DRIVER_TYPE_HARDWARE,
	//	nullptr,
	//	l_CreateDeviceFlag,
	//	l_FeatureLevels,
	//	l_NumFeatureLevels,
	//	D3D11_SDK_VERSION,
	//	&l_scd,
	//	&m_pSwapChain,
	//	&m_pDevice,
	//	&l_FeatureLevel,
	//	&m_pContext
	//);
	if (l_hr == E_INVALIDARG) {

		//dx11_1非対応の場合（dx11_0以下を試す）
		l_hr = D3D11CreateDeviceAndSwapChain(
			m_pAdapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			l_CreateDeviceFlag,
			nullptr,
			0u,
			D3D11_SDK_VERSION,
			&l_scd,
			&m_pSwapChain,
			&m_pDevice,
			&l_FeatureLevel,
			&m_pContext
		);
	}
	if (l_FeatureLevel != D3D_FEATURE_LEVEL_11_1 && l_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
		throw ERROR_EX2("GPUはDX11非対応です。");
	ERROR_DX(l_hr);

	//RTV作成
	wrl::ComPtr<ID3D11Resource> l_pBackBuffer;
	l_hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&l_pBackBuffer));	//バッファ取得
	ERROR_DX(l_hr);
	l_hr = m_pDevice->CreateRenderTargetView(l_pBackBuffer.Get(), nullptr, &m_pView_RenderTarget);
	ERROR_DX(l_hr);



	//DSステート作成
	D3D11_DEPTH_STENCIL_DESC l_dsd{};
	l_dsd.DepthEnable = TRUE;
	l_dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	l_dsd.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> l_pState_DepthStencil;
	l_hr = m_pDevice->CreateDepthStencilState(&l_dsd, &l_pState_DepthStencil);
	ERROR_DX(l_hr);
	m_pContext->OMSetDepthStencilState(l_pState_DepthStencil.Get(), 1u);		//バインド処理

	//DSバッファ作成(テクスチャ)
	D3D11_TEXTURE2D_DESC l_Desc_DepthStencil{};
	l_Desc_DepthStencil.Width = static_cast<UINT>(fWidth);
	l_Desc_DepthStencil.Height = static_cast<UINT>(fHeight);
	l_Desc_DepthStencil.MipLevels = 1u;
	l_Desc_DepthStencil.ArraySize = 1u;
	l_Desc_DepthStencil.Format = DXGI_FORMAT_D32_FLOAT;
	l_Desc_DepthStencil.SampleDesc.Count = 1u;
	l_Desc_DepthStencil.SampleDesc.Quality = 0u;
	l_Desc_DepthStencil.Usage = D3D11_USAGE_DEFAULT;
	l_Desc_DepthStencil.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	wrl::ComPtr<ID3D11Texture2D> l_pBuffer_DepthStencil;
	l_hr = m_pDevice->CreateTexture2D(&l_Desc_DepthStencil, nullptr, &l_pBuffer_DepthStencil);
	ERROR_DX(l_hr);

	//DSV作成
	D3D11_DEPTH_STENCIL_VIEW_DESC l_Desc_DepthStencilView{};
	l_Desc_DepthStencilView.Format = l_Desc_DepthStencil.Format;
	l_Desc_DepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	l_Desc_DepthStencilView.Texture2D.MipSlice = 0u;
	l_hr = m_pDevice->CreateDepthStencilView(l_pBuffer_DepthStencil.Get(), &l_Desc_DepthStencilView, &m_pView_DepthStencil);
	ERROR_DX(l_hr);

	//描画モード設定
	SetDrawMode(ET_DRAW_MODE::me_Draw_3D);

	//ビューポート設定（ラスタライザ）
	D3D11_VIEWPORT l_vp{};
	l_vp.TopLeftX = 0.0f;
	l_vp.TopLeftY = 0.0f;
	l_vp.Width = fWidth;
	l_vp.Height = fHeight;
	l_vp.MinDepth = 0.0f;
	l_vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1u, &l_vp);



	//行列初期化
	dx::XMStoreFloat4x4(&m_MtxView, dx::XMMatrixIdentity());
	dx::XMStoreFloat4x4(&m_MtxProjection, dx::XMMatrixIdentity());

#ifdef IMGUI

	//IMGUI初期化
	if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get()))
		throw ERROR_EX2("IMGUI初期化失敗");

	//フラグ初期化
	m_bDrawImGui = true;

#endif // IMGUI

}

CT_GRAPHIC::~CT_GRAPHIC() noexcept(!gc_IS_DEBUG)
{

#ifdef IMGUI

	//IMGUI終了
	ImGui_ImplDX11_Shutdown();

#endif // IMGUI

//#ifdef _DEBUG
//
//	//エラー確認
//	m_pView_DepthStencil.Reset();
//	m_pView_RenderTarget.Reset();
//	m_pContext.Reset();
//	m_pSwapChain.Reset();
//
//	{
//		wrl::ComPtr<ID3D11Debug> l_pDebug_Gfx;
//		HRESULT l_hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&l_pDebug_Gfx));
//		ERROR_DX(l_hr);
//		l_hr = l_pDebug_Gfx->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//		ERROR_DX(l_hr);
//	}
//	m_pDevice.Reset();
//
//#endif // _DEBUG

}

//フレーム開始
void CT_GRAPHIC::BeginFrame(const float r, const float g, const float b) const noexcept
{
	//バッファクリア
	const float Color[] = { r, g, b, 1.0f };
	m_pContext->ClearRenderTargetView(m_pView_RenderTarget.Get(), Color);
	m_pContext->ClearDepthStencilView(m_pView_DepthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

#ifdef IMGUI

	//IMGUI用フレーム書込開始
	if (m_bDrawImGui) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

#endif // IMGUI

}

//インデックス描画
void CT_GRAPHIC::DrawIndexed(const UINT indexNum) const noexcept
{
	//書込み処理
	m_pContext->DrawIndexed(indexNum, 0u, 0);
}

//インスタンシング描画
void CT_GRAPHIC::DrawInstanced(const UINT indexNum, const UINT instanceNum) const noexcept
{
	//書込み処理
	m_pContext->DrawIndexedInstanced(indexNum, instanceNum, 0u, 0, 0u);
}

//フレーム終了
void CT_GRAPHIC::EndFrame() const
{

#ifdef IMGUI

	//IMGUI用フレーム書込終了
	if (m_bDrawImGui) {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

#endif // IMGUI

	//描画命令
	const HRESULT l_hr = m_pSwapChain->Present(1u, 0u);
	if (l_hr == DXGI_ERROR_DEVICE_REMOVED)
		throw ERROR_EX(m_pDevice->GetDeviceRemovedReason());
	else
		ERROR_DX(l_hr);
}

//描画モード設定
void CT_GRAPHIC::SetDrawMode(const ET_DRAW_MODE mode) const noexcept
{
	//ビューをバインド
	switch (mode) {
		case ET_DRAW_MODE::me_Draw_2D:
			m_pContext->OMSetRenderTargets(1u, m_pView_RenderTarget.GetAddressOf(), nullptr);
			break;
		case ET_DRAW_MODE::me_Draw_3D:
			m_pContext->OMSetRenderTargets(1u, m_pView_RenderTarget.GetAddressOf(), m_pView_DepthStencil.Get());
			break;
		default:
			break;
	}
}

//DXGI初期化
void CT_GRAPHIC::InitDxgi()
{
	//エラーハンドル
	HRESULT l_hr{};

	//ファクトリ初期化
	wrl::ComPtr<IDXGIFactory> l_pFactory;
	wrl::ComPtr<IDXGIFactory6> l_pFactory6;

#ifdef _DEBUG

	l_hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&l_pFactory));	//デバッグ情報(DXGI)
	ERROR_DX(l_hr);

#else

	hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
	ERROR_DX(hr);

#endif // _DEBUG

	l_hr = l_pFactory->QueryInterface(IID_PPV_ARGS(&l_pFactory6));
	ERROR_DX(l_hr);

	//GPUデバイス指定(高パフォーマンス)
	l_hr = l_pFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_pAdapter));
	ERROR_DX(l_hr);

#ifdef _DEBUG

	//GPU情報取得
	IDXGIAdapter* l_PTempAdapter = nullptr;
	for (int l_Cnt = 0; ; l_Cnt++) {

		//アダプターのポインタを取得
		l_hr = l_pFactory6->EnumAdapters(static_cast<UINT>(l_Cnt), &l_PTempAdapter);
		if (l_hr == DXGI_ERROR_NOT_FOUND)
			break;

		//デバイス名出力
		DXGI_ADAPTER_DESC l_dad{};
		l_hr = l_PTempAdapter->GetDesc(&l_dad);
		ERROR_DX(l_hr);
		std::wostringstream l_oss;
		l_oss << "Info : GPU(" << l_Cnt << ") " << l_dad.Description << std::endl;
		PRINT_D(l_oss.str().c_str());

		//メモリ解放
		l_PTempAdapter->Release();
		l_PTempAdapter = nullptr;
	}

#endif // _DEBUG

}
