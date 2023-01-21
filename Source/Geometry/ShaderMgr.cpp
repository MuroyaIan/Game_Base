
//===== インクルード部 =====
#include <Geometry/ShaderMgr.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>

//===== クラス実装 =====
SHADER_MGR::SHADER_MGR(GRAPHIC& Gfx) :
	m_DX(Gfx), m_aBinder(static_cast<int>(BINDER_ID::ID_MAX))
{
	//DEFAULT
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Default.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_DEFAULT)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_DEFAULT)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//VTX_BLEND
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_VertexBlend.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_VTX_BLEND)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",	0u, DXGI_FORMAT_R32G32B32_FLOAT,	0u, 0u,								D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "COLOR",		0u, DXGI_FORMAT_R8G8B8A8_UNORM,		0u, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_VTX_BLEND)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//TEXTURE
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Texture.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_TEXTURE)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT,	0u, 0u,								D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT,		0u, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_TEXTURE)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//MODEL
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Model.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_MODEL)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "NORMAL",	  0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "BINORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "TANGENT",  0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "BONE",	  0u, DXGI_FORMAT_R32_SINT,		   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "BONE",	  1u, DXGI_FORMAT_R32_SINT,		   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "BONE",	  2u, DXGI_FORMAT_R32_SINT,		   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "BONE",	  3u, DXGI_FORMAT_R32_SINT,		   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "WEIGHT",	  0u, DXGI_FORMAT_R32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "WEIGHT",	  1u, DXGI_FORMAT_R32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "WEIGHT",	  2u, DXGI_FORMAT_R32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "WEIGHT",	  3u, DXGI_FORMAT_R32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_MODEL)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//MODEL_BONE
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Model_Bone.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_MODEL_BONE)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT,	  0u, 0u,							D3D11_INPUT_PER_VERTEX_DATA,   0u },
			{ "COLOR",	  0u, DXGI_FORMAT_R8G8B8A8_UNORM,	  0u, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,   0u },
			{ "MTX_L",	  0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "MTX_L",	  1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "MTX_L",	  2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "MTX_L",	  3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_MODEL_BONE)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_VTX_BLEND
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_VertexBlend.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_VTX_BLEND)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "COLOR",	   0u, DXGI_FORMAT_R8G8B8A8_UNORM,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_VTX_BLEND)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_TEXTURE
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Texture.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_TEXTURE)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "TEXCOORD",  0u, DXGI_FORMAT_R32G32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_TEXTURE)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_PHONG
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Phong.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_PHONG)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "TEXCOORD",  0u, DXGI_FORMAT_R32G32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "NORMAL",	   0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_PHONG)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_PHONG_ANIM
	{
		//頂点シェーダ作成
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Phong_Anim.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_PHONG_ANIM)] = std::move(pVS);

		//入力レイアウト作成
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",	0u, DXGI_FORMAT_R32G32B32_FLOAT,	0u, 0u,							  D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "TEXCOORD",	0u, DXGI_FORMAT_R32G32_FLOAT,		0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "NORMAL",		0u, DXGI_FORMAT_R32G32B32_FLOAT,	0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "BONE",		0u, DXGI_FORMAT_R32_SINT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "BONE",		1u, DXGI_FORMAT_R32_SINT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "BONE",		2u, DXGI_FORMAT_R32_SINT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "BONE",		3u, DXGI_FORMAT_R32_SINT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "WEIGHT",		0u, DXGI_FORMAT_R32_FLOAT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "WEIGHT",		1u, DXGI_FORMAT_R32_FLOAT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "WEIGHT",		2u, DXGI_FORMAT_R32_FLOAT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "WEIGHT",		3u, DXGI_FORMAT_R32_FLOAT,			0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	 0u },
			{ "WORLD_MTX",	0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_PHONG_ANIM)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}



	//プリミティブトポロジー作成
	m_aBinder[static_cast<int>(BINDER_ID::PT_LINE)] = std::make_unique<TOPOLOGY>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)] = std::make_unique<TOPOLOGY>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//サンプラー作成
	m_aBinder[static_cast<int>(BINDER_ID::SAMPLER)] = std::make_unique<SAMPLER>(m_DX);

	//ピクセルシェーダ作成
	m_aBinder[static_cast<int>(BINDER_ID::PS_DEFAULT)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Default.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_ONE_COLOR)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_SingleColor.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_VTX_BLEND)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_VertexBlend.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_TEXTURE)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Texture.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_MODEL)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Model.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_PHONG)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Phong.cso");



	//定数バッファ作成（VP行列）
	m_aBinder[static_cast<int>(BINDER_ID::CB_VS_MTX_VP)] = std::make_unique<CB_MTX_VP>(m_DX, nullptr);

	{
		//定数バッファ作成（ポリゴン色）
		const CBD_COLOR cbColor{};
		m_aBinder[static_cast<int>(BINDER_ID::CB_PS_DEFAULT)] = std::make_unique<CONSTANT_BUFFER<CBD_COLOR>>(m_DX, cbColor, nullptr, false, true);
	}

	//定数バッファ作成（ライト情報）
	m_aBinder[static_cast<int>(BINDER_ID::CB_PS_LIGHT)] = std::make_unique<CONSTANT_BUFFER<LIGHT_MGR::LIGHT_PACK>>(m_DX, nullptr, false, true);
}

SHADER_MGR::~SHADER_MGR() noexcept
{
}

//バインド処理
void SHADER_MGR::Bind(BINDER_ID id) const noexcept
{
	m_aBinder[static_cast<int>(id)]->Bind(m_DX);
}

//バインド処理（デフォルト）
void SHADER_MGR::Bind_Default() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_DEFAULT)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_DEFAULT)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_DEFAULT)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::CB_PS_DEFAULT)]->Bind(m_DX);
}

//バインド処理（頂点ブレンド）
void SHADER_MGR::Bind_VertexBlend() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_VTX_BLEND)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_VTX_BLEND)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_VTX_BLEND)]->Bind(m_DX);
}

//バインド処理（テクスチャ）
void SHADER_MGR::Bind_Texture() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_TEXTURE)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_TEXTURE)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::SAMPLER)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_TEXTURE)]->Bind(m_DX);
}

//バインド処理（モデル）
void SHADER_MGR::Bind_Model() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_MODEL)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_MODEL)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::SAMPLER)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_MODEL)]->Bind(m_DX);
}

//バインド処理（インスタンシング）
void SHADER_MGR::Bind_Instance() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_DEFAULT)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::CB_PS_DEFAULT)]->Bind(m_DX);
}

//バインド処理（インスタンシング＿頂点ブレンド）
void SHADER_MGR::Bind_Instance_VertexBlend() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_VTX_BLEND)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_VTX_BLEND)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_VTX_BLEND)]->Bind(m_DX);
}

//バインド処理（インスタンシング＿テクスチャ）
void SHADER_MGR::Bind_Instance_Texture() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_TEXTURE)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_TEXTURE)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::SAMPLER)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_TEXTURE)]->Bind(m_DX);
}

//バインド処理（インスタンシング＿Phongモデル）
void SHADER_MGR::Bind_Instance_Phong() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_PHONG)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_PHONG)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::SAMPLER)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_PHONG)]->Bind(m_DX);
}

//バインド処理（インスタンシング＿Phongモデル＿アニメ付）
void SHADER_MGR::Bind_Instance_Phong_Anim() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_INSTANCE_PHONG_ANIM)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_INSTANCE_PHONG_ANIM)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_TRI)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::SAMPLER)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_PHONG)]->Bind(m_DX);
}

//定数バッファポインタ登録
void SHADER_MGR::SetConstBufferPtr(BINDER_ID id, CB_PTR* cbPtr) const
{
	//登録処理
	switch (id) {
		case SHADER_MGR::BINDER_ID::CB_VS_MTX_VP:
			dynamic_cast<CB_MTX_VP*>(m_aBinder[static_cast<int>(id)].get())->SetBuffPtr(cbPtr);
			break;
		case SHADER_MGR::BINDER_ID::CB_PS_DEFAULT:
			dynamic_cast<CONSTANT_BUFFER<CBD_COLOR>*>(m_aBinder[static_cast<int>(id)].get())->SetBuffPtr(cbPtr);
			break;
		case SHADER_MGR::BINDER_ID::CB_PS_LIGHT:
			dynamic_cast<CONSTANT_BUFFER<LIGHT_MGR::LIGHT_PACK>*>(m_aBinder[static_cast<int>(id)].get())->SetBuffPtr(cbPtr);
			break;
		default:
			throw ERROR_EX2("BINDER_IDは定数バッファではありません。");
	}
}
