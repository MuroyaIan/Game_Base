
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
GRAPHIC::GRAPHIC(HWND hWindow, float fWidth, float fHeight) :
	m_pAdapter(), m_pDevice(), m_pSwapChain(), m_pContext(), m_pRTView(), m_pDSView(),
	m_mtxView(), m_mtxProjection()
{
	//エラーハンドル
	HRESULT hr{};

	//DXGI初期化
	InitDXGI();

	//FeatureLevel設定
	D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
	D3D_FEATURE_LEVEL FeatureLevel{};

	//スワップチェーン設定
	DXGI_SWAP_CHAIN_DESC scd{};
	scd.BufferDesc.Width = static_cast<UINT>(fWidth);
	scd.BufferDesc.Height = static_cast<UINT>(fHeight);
	scd.BufferDesc.RefreshRate.Numerator = 0u;
	scd.BufferDesc.RefreshRate.Denominator = 0u;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1u;
	scd.SampleDesc.Quality = 0u;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1u;
	scd.OutputWindow = hWindow;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0u;

	//デバッグ設定
	UINT CreateDeviceFlag = 0u;

#ifdef _DEBUG

	CreateDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	//デバイス・スワップチェーン初期化
	hr = D3D11CreateDeviceAndSwapChain(
		m_pAdapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		CreateDeviceFlag,
		FeatureLevels,
		NumFeatureLevels,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pDevice,
		&FeatureLevel,
		&m_pContext
	);
	if (hr == E_INVALIDARG) {

		//dx11_1非対応の場合（dx11_0以下を試す）
		hr = D3D11CreateDeviceAndSwapChain(
			m_pAdapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			CreateDeviceFlag,
			nullptr,
			0u,
			D3D11_SDK_VERSION,
			&scd,
			&m_pSwapChain,
			&m_pDevice,
			&FeatureLevel,
			&m_pContext
		);
	}
	if (FeatureLevel != D3D_FEATURE_LEVEL_11_1 && FeatureLevel != D3D_FEATURE_LEVEL_11_0)
		throw ERROR_EX2("GPUはDX11非対応です。");
	ERROR_DX(hr);

	//RTV作成
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));	//バッファ取得
	ERROR_DX(hr);
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTView);
	ERROR_DX(hr);



	//DSステート作成
	D3D11_DEPTH_STENCIL_DESC dsd{};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	hr = m_pDevice->CreateDepthStencilState(&dsd, &pDSState);
	ERROR_DX(hr);
	m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);		//バインド処理

	//DSバッファ作成(テクスチャ)
	D3D11_TEXTURE2D_DESC tdDepth{};
	tdDepth.Width = static_cast<UINT>(fWidth);
	tdDepth.Height = static_cast<UINT>(fHeight);
	tdDepth.MipLevels = 1u;
	tdDepth.ArraySize = 1u;
	tdDepth.Format = DXGI_FORMAT_D32_FLOAT;
	tdDepth.SampleDesc.Count = 1u;
	tdDepth.SampleDesc.Quality = 0u;
	tdDepth.Usage = D3D11_USAGE_DEFAULT;
	tdDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	wrl::ComPtr<ID3D11Texture2D> pDSBuffer;
	hr = m_pDevice->CreateTexture2D(&tdDepth, nullptr, &pDSBuffer);
	ERROR_DX(hr);

	//DSV作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
	dsvd.Format = tdDepth.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0u;
	hr = m_pDevice->CreateDepthStencilView(pDSBuffer.Get(), &dsvd, &m_pDSView);
	ERROR_DX(hr);

	//描画モード設定
	SetDrawMode(DRAW_MODE::DRAW_3D);

	//ビューポート設定（ラスタライザ）
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = fWidth;
	vp.Height = fHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1u, &vp);



	//行列初期化
	dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixIdentity());
	dx::XMStoreFloat4x4(&m_mtxProjection, dx::XMMatrixIdentity());

#ifdef IMGUI

	//IMGUI初期化
	if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get()))
		throw ERROR_EX2("IMGUI初期化失敗");

	//フラグ初期化
	m_bDrawImGui = true;

#endif // IMGUI

}

GRAPHIC::~GRAPHIC() noexcept(!IS_DEBUG)
{

#ifdef IMGUI

	//IMGUI終了
	ImGui_ImplDX11_Shutdown();

#endif // IMGUI

//#ifdef _DEBUG
//
//	//エラー確認
//	m_pDSView.Reset();
//	m_pRTView.Reset();
//	m_pContext.Reset();
//	m_pSwapChain.Reset();
//
//	{
//		wrl::ComPtr<ID3D11Debug> pDebugDX;
//		HRESULT hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&pDebugDX));
//		ERROR_DX(hr);
//		hr = pDebugDX->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//		ERROR_DX(hr);
//	}
//	m_pDevice.Reset();
//
//#endif // _DEBUG

}

//フレーム開始
void GRAPHIC::BeginFrame(float R, float G, float B) const noexcept
{
	//バッファクリア
	const float Color[] = { R, G, B, 1.0f };
	m_pContext->ClearRenderTargetView(m_pRTView.Get(), Color);
	m_pContext->ClearDepthStencilView(m_pDSView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

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
void GRAPHIC::DrawIndexed(UINT IndexNum) const noexcept
{
	//書込み処理
	m_pContext->DrawIndexed(IndexNum, 0u, 0);
}

//インスタンシング描画
void GRAPHIC::DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept
{
	//書込み処理
	m_pContext->DrawIndexedInstanced(IndexNum, InstanceNum, 0u, 0, 0u);
}

//フレーム終了
void GRAPHIC::EndFrame() const
{

#ifdef IMGUI

	//IMGUI用フレーム書込終了
	if (m_bDrawImGui) {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

#endif // IMGUI

	//エラーハンドル
	HRESULT hr{};

	//描画命令
	hr = m_pSwapChain->Present(1u, 0u);
	if (hr == DXGI_ERROR_DEVICE_REMOVED)
		throw ERROR_EX(m_pDevice->GetDeviceRemovedReason());
	else
		ERROR_DX(hr);
}

//描画モード設定
void GRAPHIC::SetDrawMode(DRAW_MODE Mode) const noexcept
{
	//ビューをバインド
	switch (Mode) {
		case GRAPHIC::DRAW_MODE::DRAW_2D:
			m_pContext->OMSetRenderTargets(1u, m_pRTView.GetAddressOf(), nullptr);
			break;
		case GRAPHIC::DRAW_MODE::DRAW_3D:
			m_pContext->OMSetRenderTargets(1u, m_pRTView.GetAddressOf(), m_pDSView.Get());
			break;
		default:
			break;
	}
}

//DXGI初期化
void GRAPHIC::InitDXGI()
{
	//エラーハンドル
	HRESULT hr{};

	//ファクトリ初期化
	wrl::ComPtr<IDXGIFactory> pFactory;
	wrl::ComPtr<IDXGIFactory6> pFactory6;

#ifdef _DEBUG

	hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&pFactory));	//デバッグ情報(DXGI)
	ERROR_DX(hr);

#else

	hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
	ERROR_DX(hr);

#endif // _DEBUG

	hr = pFactory->QueryInterface(IID_PPV_ARGS(&pFactory6));
	ERROR_DX(hr);

	//GPUデバイス指定(高パフォーマンス)
	hr = pFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_pAdapter));
	ERROR_DX(hr);

#ifdef _DEBUG

	//GPU情報取得
	IDXGIAdapter* pTempAdapter = nullptr;
	for (int i = 0; ; i++) {

		//アダプターのポインタを取得
		hr = pFactory6->EnumAdapters(static_cast<UINT>(i), &pTempAdapter);
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;

		//デバイス名出力
		DXGI_ADAPTER_DESC dad{};
		hr = pTempAdapter->GetDesc(&dad);
		ERROR_DX(hr);
		std::wostringstream oss;
		oss << "Info : GPU(" << i << ") " << dad.Description << std::endl;
		PrintD(oss.str().c_str());

		//メモリ解放
		pTempAdapter->Release();
		pTempAdapter = nullptr;
	}

#endif // _DEBUG

}
