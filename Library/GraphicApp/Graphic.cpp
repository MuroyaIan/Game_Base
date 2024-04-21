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

#ifdef _DEBUG

#pragma comment( lib, "dxguid.lib")

#endif // _DEBUG

//===== クラス実装 =====
CT_GRAPHIC::CT_GRAPHIC(CT_IF_WINDOW& pWindow, float fWidth, float fHeight) :
	m_Version_11_1(true), m_Viewport(), m_MsaaQuality(0u), m_EnableMsaa(false), m_pWindow(pWindow), m_MtxView(), m_MtxProjection()
{
	//DXGI初期化
	HRESULT l_hr{};
	HWND l_WinHandle = dynamic_cast<CT_IW_WIN&>(m_pWindow).GetHandle();
	ComPtr<IDXGIFactory> l_pDxgiFactory{};
	ComPtr<IDXGIAdapter> l_pDxgiAdapter{};

#ifdef _DEBUG

	l_hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&l_pDxgiFactory));	//ファクトリ作成
	ERROR_DX(l_hr);

#else

	l_hr = CreateDXGIFactory2(0u, IID_PPV_ARGS(&l_pDxgiFactory));
	ERROR_DX(l_hr);

#endif // _DEBUG

	InitGfxCard(l_pDxgiFactory, l_pDxgiAdapter);	//グラボ指定

	//デバイス用データ作成
    D3D_FEATURE_LEVEL l_FeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT l_NumFeatureLevel = ARRAYSIZE(l_FeatureLevels);

    D3D_FEATURE_LEVEL l_FeatureLevel{};

	//デバッグ設定
	UINT l_CreateDeviceFlags = 0u;

#ifdef _DEBUG

	l_CreateDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	//デバイス作成（D3D_FEATURE_LEVEL_11_1）
	l_hr = D3D11CreateDevice(
		l_pDxgiAdapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		l_CreateDeviceFlags,
		l_FeatureLevels,
		l_NumFeatureLevel,
		D3D11_SDK_VERSION,
		&m_pDevice,
		&l_FeatureLevel,
		&m_pContext
	);

	//D3D_FEATURE_LEVEL_11_0
	if (l_hr == E_INVALIDARG)
    {
		l_hr = D3D11CreateDevice(
			l_pDxgiAdapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			l_CreateDeviceFlags,
			&l_FeatureLevels[1],
			l_NumFeatureLevel - 1u,
			D3D11_SDK_VERSION,
			&m_pDevice,
			&l_FeatureLevel,
			&m_pContext
		);

#ifdef _DEBUG

		PRINT_D(L"Warning : [D3D_FEATURE_LEVEL_11_1] not supported.\n");

#endif // _DEBUG

    }
	ERROR_DX(l_hr);

	//MSAA確認
    l_hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MsaaQuality);
	ERROR_DX(l_hr);

#ifdef _DEBUG

	std::wostringstream l_oss{};
	l_oss << L"Info : MSAA support (QualityLevel : " << m_MsaaQuality << L")" << std::endl;
	PRINT_D(l_oss.str().c_str());

#endif // _DEBUG

	//スワップチェーン作成
    ComPtr<IDXGIFactory1> l_pDxgiFactory1{};	//D3D11.0(DXGI1.1)
    ComPtr<IDXGIFactory2> l_pDxgiFactory2{};	//D3D11.1(DXGI1.2)
	l_hr = l_pDxgiFactory.As(&l_pDxgiFactory1);
	ERROR_DX(l_hr);
	l_hr = l_pDxgiFactory1.As(&l_pDxgiFactory2);
	if (l_pDxgiFactory2 != nullptr)
    {
		//D3D11.1が使用可能の場合
        l_hr = m_pDevice.As(&m_pDevice1);
		ERROR_DX(l_hr);
        l_hr = m_pContext.As(&m_pContext1);
		ERROR_DX(l_hr);

        //スワップチェーン用データ作成
        DXGI_SWAP_CHAIN_DESC1 l_scd1{};
        l_scd1.Width = static_cast<UINT>(fWidth);
        l_scd1.Height = static_cast<UINT>(fHeight);
        l_scd1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        if (m_EnableMsaa)
        {
			//MSAAオン
            l_scd1.SampleDesc.Count = 4u;
            l_scd1.SampleDesc.Quality = m_MsaaQuality - 1u;
        }
        else
        {
			//MSAAオフ
            l_scd1.SampleDesc.Count = 1u;
            l_scd1.SampleDesc.Quality = 0u;
        }
        l_scd1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        l_scd1.BufferCount = 1u;
        l_scd1.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        l_scd1.Flags = 0u;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC l_fd{};
        l_fd.RefreshRate.Numerator = 60u;
        l_fd.RefreshRate.Denominator = 1u;
        l_fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        l_fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        l_fd.Windowed = TRUE;

        //作成処理
        l_hr = l_pDxgiFactory2->CreateSwapChainForHwnd(m_pDevice.Get(), l_WinHandle, &l_scd1, &l_fd, nullptr, &m_pSwapChain1);
		ERROR_DX(l_hr);
        l_hr = m_pSwapChain1.As(&m_pSwapChain);
		ERROR_DX(l_hr);
    }
    else
    {
        //スワップチェーン用データ作成
        DXGI_SWAP_CHAIN_DESC l_scd{};
        l_scd.BufferDesc.Width = static_cast<UINT>(fWidth);
        l_scd.BufferDesc.Height = static_cast<UINT>(fHeight);
        l_scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        l_scd.BufferDesc.RefreshRate.Numerator = 60u;
        l_scd.BufferDesc.RefreshRate.Denominator = 1u;
        l_scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        l_scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        l_scd.Windowed = TRUE;
		l_scd.OutputWindow = l_WinHandle;
        if (m_EnableMsaa)
        {
			//MSAAオン
            l_scd.SampleDesc.Count = 4u;
            l_scd.SampleDesc.Quality = m_MsaaQuality - 1u;
        }
        else
        {
			//MSAAオフ
            l_scd.SampleDesc.Count = 1u;
            l_scd.SampleDesc.Quality = 0u;
        }
        l_scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        l_scd.BufferCount = 1u;
        l_scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        l_scd.Flags = 0u;

		//作成処理
        l_hr = l_pDxgiFactory1->CreateSwapChain(m_pDevice.Get(), &l_scd, &m_pSwapChain);
		ERROR_DX(l_hr);
		m_Version_11_1 = false;
    }

	//解像度関連初期化
	UpdateResolution(fWidth, fHeight);



	////DSステート作成
	//D3D11_DEPTH_STENCIL_DESC l_dsd{};
	//l_dsd.DepthEnable = TRUE;
	//l_dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//l_dsd.DepthFunc = D3D11_COMPARISON_LESS;
	//ComPtr<ID3D11DepthStencilState> l_pState_DepthStencil;
	//l_hr = m_pDevice->CreateDepthStencilState(&l_dsd, &l_pState_DepthStencil);
	//ERROR_DX(l_hr);
	//m_pContext->OMSetDepthStencilState(l_pState_DepthStencil.Get(), 1u);		//バインド処理

	////DSV作成
	//D3D11_DEPTH_STENCIL_VIEW_DESC l_desc_dsv{};
	//l_desc_dsv.Format = l_desc_ds.Format;
	//l_desc_dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//l_desc_dsv.Texture2D.MipSlice = 0u;
	//l_hr = m_pDevice->CreateDepthStencilView(l_pBuffer_DepthStencil.Get(), &l_desc_dsv, &m_pView_DepthStencil);
	//ERROR_DX(l_hr);



	//[alt + enter]禁止
    l_hr = l_pDxgiFactory->MakeWindowAssociation(l_WinHandle, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
	ERROR_DX(l_hr);

	//行列初期化
	dx::XMStoreFloat4x4(&m_MtxView, dx::XMMatrixIdentity());
	dx::XMStoreFloat4x4(&m_MtxProjection, dx::XMMatrixIdentity());

#ifdef _DEBUG

	//GFXデバッグ
	l_hr = n_GfxDebug::SetDebugObjectName_D3D11(m_pContext.Get(), "dxContext");
	ERROR_DX(l_hr);
	l_hr = n_GfxDebug::SetDebugObjectName_Dxgi(m_pSwapChain.Get(), "dxSwapChain");
	ERROR_DX(l_hr);

#endif // _DEBUG

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
    //終了処理
    if (m_pContext)
        m_pContext->ClearState();

#ifdef IMGUI

	//IMGUI終了
	ImGui_ImplDX11_Shutdown();

#endif // IMGUI

//#ifdef _DEBUG
//
//	//エラー確認
//	m_pBuffer_DepthStencil.Reset();
//	m_pView_DepthStencil.Reset();
//	m_pView_RenderTarget.Reset();
//	m_pSwapChain.Reset();
//	m_pContext.Reset();
//
//	{
//		ComPtr<ID3D11Debug> l_pDebug_Gfx{};
//		HRESULT l_hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&l_pDebug_Gfx));
//		ERROR_DX(l_hr);
//		l_hr = l_pDebug_Gfx->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//		ERROR_DX(l_hr);
//	}
//	m_pDevice.Reset();
//
//#endif // _DEBUG

}

/**
 * 解像度更新
 *
 * \param nWndPosX
 * \param nWndPosY
 * \param nWndWidth
 * \param nWndHeight
 * \return void
 */
void CT_GRAPHIC::SetResolution(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth, const int& nWndHeight)
{
	//Win側の更新
	m_pWindow.Transform(nWndPosX, nWndPosY, nWndWidth, nWndHeight);

	//DX側の更新
	const float l_Width = static_cast<float>(nWndWidth);
	const float l_Height = static_cast<float>(nWndHeight);
	UpdateResolution(l_Width, l_Height);
}

/**
 * フレーム開始
 *
 * \param r
 * \param g
 * \param b
 * \return void
 */
void CT_GRAPHIC::BeginFrame(const float& r, const float& g, const float& b) const noexcept
{
	//バッファクリア
	const float l_Color[] = { r, g, b, 1.0f };
	m_pContext->ClearRenderTargetView(m_pView_RenderTarget.Get(), l_Color);
	m_pContext->ClearDepthStencilView(m_pView_DepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

#ifdef IMGUI

	//IMGUI用フレーム書込開始
	if (m_bDrawImGui) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

#endif // IMGUI

}

/**
 * インデックス描画
 *
 * \param indexNum
 * \return void
 */
void CT_GRAPHIC::DrawIndexed(const UINT& indexNum) const noexcept
{
	//書込み処理
	m_pContext->DrawIndexed(indexNum, 0u, 0);
}

/**
 * インスタンシング描画
 *
 * \param indexNum
 * \param instanceNum
 * \return void
 */
void CT_GRAPHIC::DrawInstanced(const UINT& indexNum, const UINT& instanceNum) const noexcept
{
	//書込み処理
	m_pContext->DrawIndexedInstanced(indexNum, instanceNum, 0u, 0, 0u);
}

/**
 * フレーム終了
 *
 * \param
 * \return void
 */
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
	ERROR_DX(l_hr);
}

/**
 * 描画モード設定
 *
 * \param mode
 * \return void
 */
void CT_GRAPHIC::SetDrawMode(const ET_DRAW_MODE& mode) const noexcept
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

/**
 * 解像度更新
 *
 * \param fWidth
 * \param fHeight
 * \return void
 */
void CT_GRAPHIC::UpdateResolution(const float& fWidth, const float& fHeight)
{
	//リソースクリア
    m_pView_RenderTarget.Reset();
    m_pView_DepthStencil.Reset();
    m_pBuffer_DepthStencil.Reset();

	//変数宣言
	HRESULT l_hr{};
	const UINT l_ClientWidth = static_cast<UINT>(fWidth);
	const UINT l_ClientHeight = static_cast<UINT>(fHeight);

	//RTV設定
    ComPtr<ID3D11Texture2D> l_BackBuffer{};
    l_hr = m_pSwapChain->ResizeBuffers(1u, l_ClientWidth, l_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0u);
	ERROR_DX(l_hr);
    l_hr = m_pSwapChain->GetBuffer(0u, IID_PPV_ARGS(&l_BackBuffer));
	ERROR_DX(l_hr);
    l_hr = m_pDevice->CreateRenderTargetView(l_BackBuffer.Get(), nullptr, &m_pView_RenderTarget);
	ERROR_DX(l_hr);

#ifdef _DEBUG

	//GFXデバッグ
	l_hr = n_GfxDebug::SetDebugObjectName_D3D11(l_BackBuffer.Get(), "BufferRTV[0]");
	ERROR_DX(l_hr);

#endif // _DEBUG

    l_BackBuffer.Reset();

	//DSV設定
	D3D11_TEXTURE2D_DESC l_dsb;
    l_dsb.Width = l_ClientWidth;
    l_dsb.Height = l_ClientHeight;
    l_dsb.MipLevels = 1u;
    l_dsb.ArraySize = 1u;
    l_dsb.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (m_EnableMsaa)
    {
		//MSAAオン
        l_dsb.SampleDesc.Count = 4u;
        l_dsb.SampleDesc.Quality = m_MsaaQuality - 1u;
    }
    else
    {
		//MSAAオフ
        l_dsb.SampleDesc.Count = 1u;
        l_dsb.SampleDesc.Quality = 0u;
    }
    l_dsb.Usage = D3D11_USAGE_DEFAULT;
    l_dsb.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    l_dsb.CPUAccessFlags = 0u;
    l_dsb.MiscFlags = 0u;
	l_hr = m_pDevice->CreateTexture2D(&l_dsb, nullptr, &m_pBuffer_DepthStencil);
	ERROR_DX(l_hr);
    l_hr = m_pDevice->CreateDepthStencilView(m_pBuffer_DepthStencil.Get(), nullptr, &m_pView_DepthStencil);
	ERROR_DX(l_hr);

	//描画モード設定
	SetDrawMode(ET_DRAW_MODE::me_Draw_3D);

	//ビューポート設定
    m_Viewport.TopLeftX = 0.0f;
    m_Viewport.TopLeftY = 0.0f;
    m_Viewport.Width = fWidth;
    m_Viewport.Height = fHeight;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    m_pContext->RSSetViewports(1u, &m_Viewport);
}

/**
 * グラボ設定初期化
 *
 * \param pFactory
 * \param pAdapter
 * \return void
 */
void CT_GRAPHIC::InitGfxCard(const ComPtr<IDXGIFactory>& pFactory, ComPtr<IDXGIAdapter>& pAdapter)
{
	//変数宣言
	HRESULT l_hr{};
	ComPtr<IDXGIFactory6> l_pFactory6{};

	//GPUデバイス指定(高パフォーマンス)
	l_hr = pFactory.As(&l_pFactory6);
	ERROR_DX(l_hr);
	l_hr = l_pFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pAdapter));
	ERROR_DX(l_hr);

#ifdef _DEBUG

	//GPU情報取得
	IDXGIAdapter* l_pTempAdapter{nullptr};
	for (int l_Cnt = 0; ; l_Cnt++) {

		//アダプターのポインタを取得
		l_hr = l_pFactory6->EnumAdapters(static_cast<UINT>(l_Cnt), &l_pTempAdapter);
		if (l_hr == DXGI_ERROR_NOT_FOUND)
			break;

		//デバイス名出力
		DXGI_ADAPTER_DESC l_dad{};
		l_hr = l_pTempAdapter->GetDesc(&l_dad);
		ERROR_DX(l_hr);
		std::wostringstream l_oss{};
		l_oss << L"Info : GPU(" << l_Cnt << L") " << l_dad.Description << std::endl;
		PRINT_D(l_oss.str().c_str());

		//メモリ解放
		l_pTempAdapter->Release();
		l_pTempAdapter = nullptr;
	}

#endif // _DEBUG

}
