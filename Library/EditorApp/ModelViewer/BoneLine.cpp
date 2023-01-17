
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/BoneLine.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>

namespace dx = DirectX;

//===== �v���g�^�C�v�錾 =====
void SetVertex(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone);															//���_�ݒ�
void SetVertexAnimation(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone, FBX_LOADER::SKIN_DATA& Skin, int& AnimFrame);		//���_�ݒ�i�A�j���[�V�����Đ����j

//===== �N���X���� =====
BONE_LINE::BONE_LINE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, DRAWER& BoneIn) :
	DRAWER(Gfx), m_Gfx(Gfx), m_ShaderMgr(ShaderMgr), m_Loader(Loader), m_mtxWorld(), m_Bone(BoneIn),
	m_bDrawAnimation(Viewer.GetFlag_DrawAnimation()), m_AnimationID(Viewer.GetAnimationID()), m_AnimFrame(Viewer.GetAnimationFrame())
{

	//���_���쐬
	VS_DATA<VERTEX> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices, true));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�ϊ��s��j
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_WVP>(Gfx, &cbData, *this));

	//PS�萔�o�b�t�@�쐬�i�@���̐F�j
	const dx::XMFLOAT4 cbNormalColor(0.5f, 1.0f, 1.0f, 1.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(Gfx, cbNormalColor, &cbData, false, true));

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

BONE_LINE::~BONE_LINE() noexcept
{
}

//�X�V����
void BONE_LINE::Update() noexcept
{
	//���_�X�V
	VS_DATA<VERTEX> vsData;
	std::vector<FBX_LOADER::BONE_DATA>& BoneData = m_Loader.GetSkeleton();
	if (m_bDrawAnimation) {
		for (auto& b : BoneData) {
			if (b.IsRoot)
				SetVertexAnimation(vsData, b, m_Loader.GetSkin()[m_AnimationID], m_AnimFrame);
		}
		GetVertexBuffer().UpdateBuffer(m_Gfx, vsData.m_Vertices, VERTEX_BUFFER::VB_TYPE::Vertex);
	}
	else {
		for (auto& b : BoneData) {
			if (b.IsRoot)
				SetVertex(vsData, b);
		}
		GetVertexBuffer().UpdateBuffer(m_Gfx, vsData.m_Vertices, VERTEX_BUFFER::VB_TYPE::Vertex);
	}

	//���[���h�s��X�V
	m_mtxWorld = m_Bone.GetWorldMatrix();
}

void BONE_LINE::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_LINE);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	DRAWER::Draw(InstanceNum);
}

//���_���쐬
VS_DATA<VERTEX> BONE_LINE::MakeData_VS() const
{
	//�f�[�^�擾
	VS_DATA<VERTEX> vsData;
	std::vector<FBX_LOADER::BONE_DATA>& BoneData = m_Loader.GetSkeleton();
	for (auto& b : BoneData) {
		if (b.IsRoot)
			SetVertex(vsData, b);
	}

	//�C���f�b�N�X�쐬
	for (size_t i = 0, Cnt = vsData.m_Vertices.size(); i < Cnt; i++)
		vsData.m_Indices.emplace_back(static_cast<unsigned short>(i));

	return VS_DATA<VERTEX>(std::move(vsData));
}

//���_�ݒ�
void SetVertex(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone)
{
	for (auto& b : Bone.aChildBone) {

		//�e�̍��W
		dx::XMFLOAT3 Pos{ -Bone.InitMatrix._41, Bone.InitMatrix._42, Bone.InitMatrix._43 };
		vsData.m_Vertices.emplace_back(Pos);

		//�q�̍��W
		Pos = { -b->InitMatrix._41, b->InitMatrix._42, b->InitMatrix._43 };
		vsData.m_Vertices.emplace_back(Pos);

		//�q�m�[�h�̐ݒ�֑���
		SetVertex(vsData, *b);
	}
}

//���_�ݒ�i�A�j���[�V�����Đ����j
void SetVertexAnimation(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone, FBX_LOADER::SKIN_DATA& Skin, int& AnimFrame)
{
	for (auto& b : Bone.aChildBone) {

		//�e�̍��W
		if (Skin.aFrameData[Bone.Index].aBoneMatrix.size() > 0) {									//�t���[����񂪂���ꍇ
			dx::XMFLOAT4X4& FrameData = Skin.aFrameData[Bone.Index].aBoneMatrix[AnimFrame];
			dx::XMFLOAT3 Pos{ -(FrameData._41), FrameData._42, FrameData._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}
		else {																						//�t���[����񂪂Ȃ��ꍇ�i�����p���j
			dx::XMFLOAT3 Pos{ -Bone.InitMatrix._41, Bone.InitMatrix._42, Bone.InitMatrix._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}

		//�q�̍��W
		if (Skin.aFrameData[b->Index].aBoneMatrix.size() > 0) {
			dx::XMFLOAT4X4& FrameDataChild = Skin.aFrameData[b->Index].aBoneMatrix[AnimFrame];
			dx::XMFLOAT3 Pos = { -(FrameDataChild._41), FrameDataChild._42, FrameDataChild._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}
		else {
			dx::XMFLOAT3 Pos = { -b->InitMatrix._41, b->InitMatrix._42, b->InitMatrix._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}

		//�q�m�[�h�̐ݒ�֑���
		SetVertexAnimation(vsData, *b, Skin, AnimFrame);
	}
}
