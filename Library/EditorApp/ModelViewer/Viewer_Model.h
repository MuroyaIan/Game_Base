/**
 * @file Viewer_Model.h
 * @brief ���f���`��
 * @author ���J�C�A��
 * @date 2022/07/19
 * @���� 2022/07/19�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <Geometry/ShaderMgr.h>
#include <GraphicApp/Drawer/vsdRef.h>

//===== �O���錾 =====
class VIEWER;
class FBX_LOADER;
struct CBD_MTX_BONE;
class INPUT_MGR;

//===== �N���X��` =====

//***** ���f���`�� *****
class VIEWER_MODEL : public DRAWER_EX<VIEWER_MODEL>
{
public:

	//�v���g�^�C�v�錾
	VIEWER_MODEL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex, INPUT_MGR& Input);
	~VIEWER_MODEL() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//�����ݏ���

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		(void)InstanceIndex;
		return m_MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//�|���S�����擾
	{
		return GetIndexNum() / 3;
	}

private:

	//�ϐ��錾
	SHADER_MGR& m_ShaderMgr;					//�V�F�[�_Mgr�Q��
	VIEWER& m_Viewer;							//�r���[���Q��
	FBX_LOADER& m_Loader;						//���[�_�Q��
	int m_MeshIndex;							//���b�V���ԍ�
	DirectX::XMFLOAT4X4 m_MtxLocal;				//���[�J���s��
	DirectX::XMFLOAT4X4 m_MtxWorld;				//���[���h�s��
	MATERIAL_DATA m_Material;					//�}�e���A�����
	bool m_bNoBone;								//���Ȃ����b�V��

	std::unique_ptr<CBD_MTX_BONE> m_pMtxBone;	//�����
	bool& m_bDrawAnimation;						//�A�j���[�V�����Đ�
	int& m_AnimationID;							//�A�j���[�V�����ԍ�
	int m_AnimFrame;							//�Đ�����t���[����
	int m_FrameCnt;								//�Đ��t���[���v�Z�p
	bool& m_AnimPause;							//�A�j���[�V�����ꎞ��~

	INPUT_MGR& m_Input;							//���͎Q��
	float& m_Scale;								//���f���X�P�[��
	DirectX::XMFLOAT3 m_Rot;					//��]

	//�v���g�^�C�v�錾
	VS_DATA<VERTEX_MB> MakeData_VS() const noexcept;	//���_���쐬
	void UpdateBoneData(int AnimID = 0) noexcept;		//�����X�V
};
