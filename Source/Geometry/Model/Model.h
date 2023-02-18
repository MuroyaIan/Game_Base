/**
 * @file Model.h
 * @brief ���f��
 * @author ���J�C�A��
 * @date 2022/09/08
 * @���� 2022/09/08�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <App.h>
#include <Geometry/Model/ModelRef.h>
#include <Geometry/Model/ModelMgr.h>

//===== �O���錾 =====
class BINDER;
class DRAWER;
struct CBD_BONE;

//===== �N���X��` =====

//***** ���f�� *****
class MODEL
{
public:

	//�v���g�^�C�v�錾
	explicit MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept;
	~MODEL() noexcept;
	void Update() noexcept;								//�X�V����
	void Draw() const noexcept;							//�����ݏ���
	int AddInstance();									//�C���X�^���X�ǉ�
	UINT GetPolygonNum() const noexcept;				//�|���S�����擾

	void ChangeAnimID(int id) noexcept					//�A�j���[�V�����ύX
	{
		if (m_AnimID != id && m_bBlendAnim == false) {	//�A�j���[�V�����ؑց��u�����h���ł͂Ȃ�

			//�A�j���[�V�����ԍ��̃o�b�N�A�b�v�X�V
			if (m_AnimID_Backup != m_AnimID)
				m_AnimID_Backup = m_AnimID;
			if (m_AnimFrame_Backup != m_AnimFrame)
				m_AnimFrame_Backup = m_AnimFrame;
			if (m_FrameCnt_Backup != m_FrameCnt)
				m_FrameCnt_Backup = m_FrameCnt;

			//�A�j���[�V�����ԍ��X�V
			float ratio = static_cast<float>(m_AnimFrame + 1) / m_FileData.aAnimFrame[m_AnimID];	//�u�����h�O�A�j���[�V�����̃t���[�������擾
			m_AnimID = id;
			m_AnimFrame = static_cast<int>(m_FileData.aAnimFrame[m_AnimID] * ratio - 1);			//�u�����h��A�j���[�V�����̃t���[���Z�o
			m_bBlendAnim = true;
		}
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	ModelRef::MODEL_PACK& m_FileData;				//�t�@�C�����
	std::vector<std::unique_ptr<DRAWER>> m_aMesh;	//���b�V���z��

	int m_InstanceNum;								//�C���X�^���X��
	std::vector<VSD_INSTANCE> m_aInstanceData;		//�C���X�^���X���

	bool m_bStatic;									//�ÓI���b�V�����ǂ���
	std::unique_ptr<BINDER> m_pBoneBuffer;			//�����p�o�C���_
	std::unique_ptr<CBD_BONE> m_pMtxBone;		//�����i�萔�o�b�t�@�p�j
	int m_AnimID;									//�A�j���[�V�����ԍ�
	int m_AnimID_Backup;							//�A�j���[�V�����ԍ��i�o�b�N�A�b�v�j
	int m_AnimFrame;								//�A�j���[�V�����Đ��t���[��
	int m_AnimFrame_Backup;							//�A�j���[�V�����Đ��t���[���i�o�b�N�A�b�v�j
	int m_FrameCnt;									//�Đ��t���[���v�Z�p
	int m_FrameCnt_Backup;							//�Đ��t���[���v�Z�p�i�o�b�N�A�b�v�j
	bool m_bBlendAnim;								//�u�����h���[�h
	int m_BlendTimer;								//�u�����h�p�^�C�}

	//�v���g�^�C�v�錾
	void UpdateAnimation() noexcept;				//�A�j���[�V�����X�V
	void UpdateAnimationBlending() noexcept;		//�A�j���[�V�����u�����h�X�V

	//�����w��
	friend class MESH;
};
