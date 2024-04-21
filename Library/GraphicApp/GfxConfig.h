/**
 * @file GfxConfig.h
 * @brief Gfxデバッグ関連
 * @author 室谷イアン
 * @date 2024/04/21
 * @履歴 2024/04/21：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <string>
#include <d3d11_1.h>

//===== プロトタイプ宣言 =====
#ifdef _DEBUG

namespace n_GfxDebug
{
	HRESULT SetDebugObjectName_D3D11(ID3D11DeviceChild* resource, const std::string& name) noexcept;
	HRESULT SetDebugObjectName_D3D11(ID3D11DeviceChild* resource, std::nullptr_t) noexcept;
	HRESULT SetDebugObjectName_Dxgi(IDXGIObject* object, const std::string& name) noexcept;
	HRESULT SetDebugObjectName_Dxgi(IDXGIObject* object, std::nullptr_t) noexcept;
}

#else

namespace n_GfxDebug
{
	void SetDebugObjectName_D3D11(const ID3D11DeviceChild* resource, const std::string& name) noexcept;
	void SetDebugObjectName_D3D11(const ID3D11DeviceChild* resource, std::nullptr_t) noexcept;
	void SetDebugObjectName_Dxgi(const IDXGIObject* object, const std::string& name) noexcept;
	void SetDebugObjectName_Dxgi(const IDXGIObject* object, std::nullptr_t) noexcept;
}

#endif // _DEBUG
