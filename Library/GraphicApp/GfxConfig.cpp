//===== インクルード部 =====
#include <GraphicApp/GfxConfig.h>

//===== 関数実装 =====
#ifdef _DEBUG

/**
 * D3DのObject名セット（グラフィックスデバッグ用）
 *
 * \param resource
 * \param name
 * \return HRESULT
 */
HRESULT n_GfxDebug::SetDebugObjectName_D3D11(ID3D11DeviceChild* resource, const std::string& name) noexcept
{
    return resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.length()), name.c_str());
}

/**
 * D3DのObject名クリア（グラフィックスデバッグ用）
 *
 * \param resource
 * \return HRESULT
 */
HRESULT n_GfxDebug::SetDebugObjectName_D3D11(ID3D11DeviceChild* resource, std::nullptr_t) noexcept
{
    return resource->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
}

/**
 * DXGIのObject名セット（グラフィックスデバッグ用）
 *
 * \param object
 * \param name
 * \return HRESULT
 */
HRESULT n_GfxDebug::SetDebugObjectName_Dxgi(IDXGIObject* object, const std::string& name) noexcept
{
    return object->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.length()), name.c_str());
}

/**
 * DXGIのObject名クリア（グラフィックスデバッグ用）
 *
 * \param object
 * \return HRESULT
 */
HRESULT n_GfxDebug::SetDebugObjectName_Dxgi(IDXGIObject* object, std::nullptr_t) noexcept
{
    return object->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
}

#else

void n_GfxDebug::SetDebugObjectName_D3D11(const ID3D11DeviceChild* resource, const std::string& name) noexcept
{
    UNREFERENCED_PARAMETER(resource);
    UNREFERENCED_PARAMETER(name);
}

void n_GfxDebug::SetDebugObjectName_D3D11(const ID3D11DeviceChild* resource, std::nullptr_t) noexcept
{
    UNREFERENCED_PARAMETER(resource);
}

void n_GfxDebug::SetDebugObjectName_Dxgi(const IDXGIObject* object, const std::string& name) noexcept
{
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(name);
}

void n_GfxDebug::SetDebugObjectName_Dxgi(const IDXGIObject* object, std::nullptr_t) noexcept
{
    UNREFERENCED_PARAMETER(object);
}

#endif // _DEBUG
