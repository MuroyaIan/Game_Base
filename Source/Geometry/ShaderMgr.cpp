
//===== �C���N���[�h�� =====
#include <Geometry/ShaderMgr.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>

//===== �N���X���� =====
SHADER_MGR::SHADER_MGR(CT_GRAPHIC& Gfx) :
	m_DX(Gfx), m_aBinder(static_cast<int>(BINDER_ID::ID_Max))
{
	//DEFAULT
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Default.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Default)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Default)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//VTX_BLEND
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_VertexBlend.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_VtxBlend)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "POSITION",	0u, DXGI_FORMAT_R32G32B32_FLOAT,	0u, 0u,								D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "COLOR",		0u, DXGI_FORMAT_R8G8B8A8_UNORM,		0u, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_VtxBlend)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//TEXTURE
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Texture.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Texture)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT,	0u, 0u,								D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT,		0u, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Texture)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//MODEL
	const std::vector<D3D11_INPUT_ELEMENT_DESC> iedViewer = {
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
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Model.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Model)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		m_aBinder[static_cast<int>(BINDER_ID::IL_Model)] = std::make_unique<INPUT_LAYOUT>(m_DX, iedViewer, pvsBC);
	}

	//MODEL_NORMAL
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Model_Normal.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Model_Normal)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		m_aBinder[static_cast<int>(BINDER_ID::IL_Model_Normal)] = std::make_unique<INPUT_LAYOUT>(m_DX, iedViewer, pvsBC);
	}

	//MODEL_BONE
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Model_Bone.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Model_Bone)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT,	  0u, 0u,							D3D11_INPUT_PER_VERTEX_DATA,   0u },
			{ "COLOR",	  0u, DXGI_FORMAT_R8G8B8A8_UNORM,	  0u, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,   0u },
			{ "MTX_L",	  0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "MTX_L",	  1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "MTX_L",	  2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "MTX_L",	  3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Model_Bone)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_VTX_BLEND
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_VertexBlend.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_VtxBlend)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "COLOR",	   0u, DXGI_FORMAT_R8G8B8A8_UNORM,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_VtxBlend)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_TEXTURE
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Texture.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Texture)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "TEXCOORD",  0u, DXGI_FORMAT_R32G32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Texture)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_PHONG
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Phong.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Phong)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
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
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Phong)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_PHONG_ANIM
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Phong_Anim.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Phong_Anim)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",	 0u, DXGI_FORMAT_R32G32B32_FLOAT,	 0u, 0u,						   D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "TEXCOORD",	 0u, DXGI_FORMAT_R32G32_FLOAT,		 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "NORMAL",		 0u, DXGI_FORMAT_R32G32B32_FLOAT,	 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "BONE",		 0u, DXGI_FORMAT_R32_SINT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "BONE",		 1u, DXGI_FORMAT_R32_SINT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "BONE",		 2u, DXGI_FORMAT_R32_SINT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "BONE",		 3u, DXGI_FORMAT_R32_SINT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "WEIGHT",		 0u, DXGI_FORMAT_R32_FLOAT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "WEIGHT",		 1u, DXGI_FORMAT_R32_FLOAT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "WEIGHT",		 2u, DXGI_FORMAT_R32_FLOAT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "WEIGHT",		 3u, DXGI_FORMAT_R32_FLOAT,			 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "WORLD_MTX",	 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "ANIM_FRAME",	 0u, DXGI_FORMAT_R32_SINT,			 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "BLEND_FRAME", 0u, DXGI_FORMAT_R32_SINT,			 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "ANIM_LERP",	 0u, DXGI_FORMAT_R32_FLOAT,			 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Phong_Anim)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_PHONG_ANIM_NOSKIN
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_Phong_Anim_NoSkin.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Phong_Anim_NoSkin)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",	 0u, DXGI_FORMAT_R32G32B32_FLOAT,	 0u, 0u,						   D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "TEXCOORD",	 0u, DXGI_FORMAT_R32G32_FLOAT,		 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "NORMAL",		 0u, DXGI_FORMAT_R32G32B32_FLOAT,	 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0u },
			{ "WORLD_MTX",	 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX",	 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "ANIM_FRAME",	 0u, DXGI_FORMAT_R32_SINT,			 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Phong_Anim_NoSkin)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//INSTANCE_PHONG_NORMAL
	{
		//���_�V�F�[�_�쐬
		std::unique_ptr<VERTEX_SHADER> pVS = std::make_unique<VERTEX_SHADER>(m_DX, L"Asset/Shader/VS_Instance_PhongN.cso");
		auto pvsBC = pVS->GetBytecode();
		m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_PhongN)] = std::move(pVS);

		//���̓��C�A�E�g�쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, 0u,							 D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "TEXCOORD",  0u, DXGI_FORMAT_R32G32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "NORMAL",	   0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "BINORMAL",  0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "TANGENT",   0u, DXGI_FORMAT_R32G32B32_FLOAT,	   0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0u },
			{ "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
			{ "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
		};
		m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_PhongN)] = std::make_unique<INPUT_LAYOUT>(m_DX, ied, pvsBC);
	}

	//�v���~�e�B�u�g�|���W�[�쐬
	m_aBinder[static_cast<int>(BINDER_ID::PT_Line)] = std::make_unique<TOPOLOGY>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)] = std::make_unique<TOPOLOGY>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�T���v���[�쐬
	m_aBinder[static_cast<int>(BINDER_ID::Sampler)] = std::make_unique<SAMPLER>(m_DX);

	//�s�N�Z���V�F�[�_�쐬
	m_aBinder[static_cast<int>(BINDER_ID::PS_Default)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Default.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_OneColor)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_SingleColor.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_VtxBlend)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_VertexBlend.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_Texture)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Texture.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_Model)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Model.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_Model_Normal)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Model_Normal.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_Model_Disp)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Model_Displacement.cso");
	m_aBinder[static_cast<int>(BINDER_ID::PS_Phong)] = std::make_unique<PIXEL_SHADER>(m_DX, L"Asset/Shader/PS_Phong.cso");



	//�萔�o�b�t�@�쐬�iVP�s��j
	m_aBinder[static_cast<int>(BINDER_ID::CB_VS_MtxVP)] = std::make_unique<CB_MTX_VP>(m_DX, nullptr);

	{
		//�萔�o�b�t�@�쐬�i�|���S���F�j
		const CBD_COLOR cbColor{};
		m_aBinder[static_cast<int>(BINDER_ID::CB_PS_Default)] = std::make_unique<CONSTANT_BUFFER<CBD_COLOR>>(m_DX, cbColor, nullptr, false, true);
	}

	//�萔�o�b�t�@�쐬�i���C�g���j
	m_aBinder[static_cast<int>(BINDER_ID::CB_Light)] = std::make_unique<CONSTANT_BUFFER<LIGHT_MGR::LIGHT_PACK>>(m_DX, nullptr, true, true);
}

SHADER_MGR::~SHADER_MGR() noexcept
{
}

//�o�C���h����
void SHADER_MGR::Bind(BINDER_ID id) const noexcept
{
	m_aBinder[static_cast<int>(id)]->Bind(m_DX);
}

//�o�C���h�����i�f�t�H���g�j
void SHADER_MGR::Bind_Default() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Default)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Default)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_Default)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::CB_PS_Default)]->Bind(m_DX);
}

//�o�C���h�����i���_�u�����h�j
void SHADER_MGR::Bind_VertexBlend() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_VtxBlend)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_VtxBlend)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_VtxBlend)]->Bind(m_DX);
}

//�o�C���h�����i�e�N�X�`���j
void SHADER_MGR::Bind_Texture() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Texture)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Texture)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::Sampler)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_Texture)]->Bind(m_DX);
}

//�o�C���h�����i�C���X�^���V���O�j
void SHADER_MGR::Bind_Instance() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Instance)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Instance)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_Default)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::CB_PS_Default)]->Bind(m_DX);
}

//�o�C���h�����i�C���X�^���V���O�Q���_�u�����h�j
void SHADER_MGR::Bind_Instance_VertexBlend() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_VtxBlend)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_VtxBlend)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_VtxBlend)]->Bind(m_DX);
}

//�o�C���h�����i�C���X�^���V���O�Q�e�N�X�`���j
void SHADER_MGR::Bind_Instance_Texture() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Texture)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Texture)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::Sampler)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_Texture)]->Bind(m_DX);
}

//�o�C���h�����i�C���X�^���V���O�QPhong���f���j
void SHADER_MGR::Bind_Instance_Phong() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Phong)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Phong)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::Sampler)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_Phong)]->Bind(m_DX);
}

//�o�C���h�����i�C���X�^���V���O�QPhong���f���Q�A�j���t�j
void SHADER_MGR::Bind_Instance_Phong_Anim() const noexcept
{
	m_aBinder[static_cast<int>(BINDER_ID::VS_Instance_Phong_Anim)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::IL_Instance_Phong_Anim)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PT_Tri)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::Sampler)]->Bind(m_DX);
	m_aBinder[static_cast<int>(BINDER_ID::PS_Phong)]->Bind(m_DX);
}

//�萔�o�b�t�@�|�C���^�o�^
void SHADER_MGR::SetConstBufferPtr(BINDER_ID id, CB_PTR* cbPtr) const
{
	//�o�^����
	switch (id) {
		case SHADER_MGR::BINDER_ID::CB_VS_MtxVP:
			dynamic_cast<CB_MTX_VP*>(m_aBinder[static_cast<int>(id)].get())->SetBuffPtr(cbPtr);
			break;
		case SHADER_MGR::BINDER_ID::CB_PS_Default:
			dynamic_cast<CONSTANT_BUFFER<CBD_COLOR>*>(m_aBinder[static_cast<int>(id)].get())->SetBuffPtr(cbPtr);
			break;
		case SHADER_MGR::BINDER_ID::CB_Light:
			dynamic_cast<CONSTANT_BUFFER<LIGHT_MGR::LIGHT_PACK>*>(m_aBinder[static_cast<int>(id)].get())->SetBuffPtr(cbPtr);
			break;
		default:
			throw ERROR_EX2("BINDER_ID�͒萔�o�b�t�@�ł͂���܂���B");
	}
}
