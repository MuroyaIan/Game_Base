
//===== �C���N���[�h�� =====
#include <GraphicApp/Graphic.h>

#ifdef IMGUI
#
#	include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#	include <3rd_Party/ImGui/backends/imgui_impl_dx11.h>
#
#endif // IMGUI

//===== �ǉ����C�u���� =====
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dCompiler.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//===== �N���X���� =====
GRAPHIC::GRAPHIC(HWND hWindow, float fWidth, float fHeight) :
	m_pDevice(), m_pSwapChain(), m_pContext(), m_pRTView(), m_pDSView(),
	m_mtxView(), m_mtxProjection()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�t�@�N�g��������
	wrl::ComPtr<IDXGIFactory> pFactory;
	wrl::ComPtr<IDXGIFactory6> pFactory6;

#ifdef _DEBUG

	hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&pFactory));	//�f�o�b�O���(DXGI)
	ERROR_DX(hr);

#else

	hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
	ERROR_DX(hr);

#endif // _DEBUG

	hr = pFactory->QueryInterface(IID_PPV_ARGS(&pFactory6));
	ERROR_DX(hr);

	//GPU�f�o�C�X�w��(���p�t�H�[�}���X)
	wrl::ComPtr<IDXGIAdapter> pAdapter;
	hr = pFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pAdapter));
	ERROR_DX(hr);

#ifdef _DEBUG

	//GPU���擾
	IDXGIAdapter* pTempAdapter = nullptr;
	for (size_t i = 0; ; i++) {

		//�A�_�v�^�[�̃|�C���^���擾
		hr = pFactory6->EnumAdapters(static_cast<UINT>(i), &pTempAdapter);
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;

		//�f�o�C�X���o��
		DXGI_ADAPTER_DESC dad{};
		hr = pTempAdapter->GetDesc(&dad);
		ERROR_DX(hr);
		std::wostringstream oss;
		oss << "Info : GPU(" << i << ") " << dad.Description << std::endl;
		PrintD(oss.str().c_str());

		//���������
		pTempAdapter->Release();
		pTempAdapter = nullptr;
	}

#endif // _DEBUG

	//FeatureLevel�ݒ�
	D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
	D3D_FEATURE_LEVEL FeatureLevel{};

	//�f�B�X�N���v�^�쐬
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = 0u;
	sd.BufferDesc.Height = 0u;
	sd.BufferDesc.RefreshRate.Numerator = 0u;
	sd.BufferDesc.RefreshRate.Denominator = 0u;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1u;
	sd.SampleDesc.Quality = 0u;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1u;
	sd.OutputWindow = hWindow;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0u;

	//�f�o�b�O�ݒ�
	UINT CreateDeviceFlag = 0u;

#ifdef _DEBUG

	CreateDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	//�f�o�C�X�E�X���b�v�`�F�[��������
	hr = D3D11CreateDeviceAndSwapChain(
		pAdapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		CreateDeviceFlag,
		FeatureLevels,
		NumFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		&FeatureLevel,
		&m_pContext
	);
	if (hr == E_INVALIDARG) {

		//dx11_1��Ή��̏ꍇ�idx11_0�ȉ��������j
		hr = D3D11CreateDeviceAndSwapChain(
			pAdapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			CreateDeviceFlag,
			nullptr,
			0u,
			D3D11_SDK_VERSION,
			&sd,
			&m_pSwapChain,
			&m_pDevice,
			&FeatureLevel,
			&m_pContext
		);
	}
	if (FeatureLevel != D3D_FEATURE_LEVEL_11_1 && FeatureLevel != D3D_FEATURE_LEVEL_11_0)
		throw ERROR_EX2("GPU��DX11��Ή��ł��B");
	ERROR_DX(hr);

	//�o�b�N�o�b�t�@�A�N�Z�X�擾
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	ERROR_DX(hr);
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTView);
	ERROR_DX(hr);



	//�[�x�E�X�e���V���X�e�[�g�쐬
	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	ERROR_DX(hr);

	//�X�e�[�g���o�C���h
	m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);



	//�[�x�E�X�e���V���o�b�t�@�쐬
	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = static_cast<UINT>(fWidth);
	descDepth.Height = static_cast<UINT>(fHeight);
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	hr = m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	ERROR_DX(hr);

	//�r���[�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	hr = m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSView);
	ERROR_DX(hr);

	//�r���[���o�C���h
	m_pContext->OMSetRenderTargets(1u, m_pRTView.GetAddressOf(), m_pDSView.Get());



	//�r���[�|�[�g�ݒ�i���X�^���C�U�j
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = fWidth;
	vp.Height = fHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1u, &vp);



	//�s�񏉊���
	dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixIdentity());
	dx::XMStoreFloat4x4(&m_mtxProjection, dx::XMMatrixIdentity());

#ifdef IMGUI

	//IMGUI������
	if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get()))
		throw ERROR_EX2("IMGUI���������s");

	//�t���O������
	m_bDrawImGui = true;

#endif // IMGUI

}

GRAPHIC::~GRAPHIC() noexcept(!IS_DEBUG)
{

#ifdef IMGUI

	//IMGUI�I��
	ImGui_ImplDX11_Shutdown();

#endif // IMGUI

//#ifdef _DEBUG
//
//	//�G���[�m�F
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

//�t���[���J�n
void GRAPHIC::BeginFrame(float R, float G, float B) const noexcept
{
	//�o�b�t�@�N���A
	const float color[] = { R, G, B, 1.0f };
	m_pContext->ClearRenderTargetView(m_pRTView.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDSView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

#ifdef IMGUI

	//IMGUI�p�t���[�������J�n
	if (m_bDrawImGui) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

#endif // IMGUI

}

//�t���[���o�b�t�@������
void GRAPHIC::DrawIndexed(UINT IndexNum) const noexcept(!IS_DEBUG)
{
	//�����ݏ���
	m_pContext->DrawIndexed(IndexNum, 0u, 0);
}

//�C���X�^���V���O�`��
void GRAPHIC::DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept(!IS_DEBUG)
{
	//�����ݏ���
	m_pContext->DrawIndexedInstanced(IndexNum, InstanceNum, 0u, 0, 0u);
}

//�t���[���I��
void GRAPHIC::EndFrame() const
{

#ifdef IMGUI

	//IMGUI�p�t���[�������I��
	if (m_bDrawImGui) {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

#endif // IMGUI

	//�G���[�n���h��
	HRESULT hr{};

	//�`�施��
	hr = m_pSwapChain->Present(1u, 0u);
	if (hr == DXGI_ERROR_DEVICE_REMOVED)
		throw ERROR_EX(m_pDevice->GetDeviceRemovedReason());
	else
		ERROR_DX(hr);
}

//�`�惂�[�h�ݒ�
void GRAPHIC::SetDrawMode(DRAW_MODE Mode)
{
	//�r���[���o�C���h
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
