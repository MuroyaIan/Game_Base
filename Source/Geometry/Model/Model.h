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
class DRAWER;
class BINDER;

//===== �N���X��` =====

//***** ���f�� *****
class MODEL
{
public:

	//�e�N�X�`���p�b�N
	struct ANIM_PACK
	{
		int ID;				//�A�j���[�V�����ԍ�
		int CurrentFrame;	//�A�j���[�V�����Đ��t���[��
		int FrameCnt;		//�Đ��t���[���v�Z�p

		ANIM_PACK() noexcept : ID(0), CurrentFrame(0), FrameCnt(0)
		{}
		~ANIM_PACK() noexcept
		{}
	};

	//�v���g�^�C�v�錾
	explicit MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept;
	~MODEL() noexcept;
	void Update() noexcept;															//�X�V����
	void Draw() const noexcept;														//�����ݏ���
	int AddInstance();																//�C���X�^���X�ǉ�
	UINT GetPolygonNum() const noexcept;											//�|���S�����擾

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept		//���[���h�s��擾
	{
		return m_aMtxWorld[InstanceIndex];
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept	//���[���h�s��ݒ�
	{
		m_aMtxWorld[InstanceIndex] = mtxW;
	}

	void ChangeAnimID(int id, int InstNum) noexcept									//�A�j���[�V�����ύX
	{
		ANIM_PACK& AnimData = m_aAnimData[InstNum];

		//�A�j���[�V�����ؑց��u�����h���ł͂Ȃ�
		if (AnimData.ID != id && m_bBlendAnim == false) {

			//�A�j���[�V�����ԍ��̃o�b�N�A�b�v�X�V
			if (m_AnimID_Backup != AnimData.ID)
				m_AnimID_Backup = AnimData.ID;
			if (m_AnimFrame_Backup != AnimData.CurrentFrame)
				m_AnimFrame_Backup = AnimData.CurrentFrame;
			if (m_FrameCnt_Backup != AnimData.FrameCnt)
				m_FrameCnt_Backup = AnimData.FrameCnt;

			//�A�j���[�V�����ԍ��X�V
			float ratio = static_cast<float>(AnimData.CurrentFrame + 1) / m_FileData.aAnimFrame[AnimData.ID];	//�u�����h�O�A�j���[�V�����̃t���[�������擾
			AnimData.ID = id;
			AnimData.CurrentFrame = static_cast<int>(m_FileData.aAnimFrame[AnimData.ID] * ratio - 1);			//�u�����h��A�j���[�V�����̃t���[���Z�o
			m_bBlendAnim = true;
		}
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	MODEL_MGR::MODEL_ID m_ID;						//���f��ID
	ModelRef::MODEL_PACK& m_FileData;				//���f�����
	std::vector<std::unique_ptr<DRAWER>> m_aMesh;	//���b�V���z��

	int m_InstanceNum;								//�C���X�^���X��
	std::vector<VSD_INSTANCE> m_aInstanceData;		//�C���X�^���X���
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//���[���h�s��

	bool m_bStatic;									//�ÓI���b�V�����ǂ���
	std::vector<ANIM_PACK> m_aAnimData;				//�A�j���[�V�������
	int m_AnimID_Backup;							//�A�j���[�V�����ԍ��i�o�b�N�A�b�v�j
	int m_AnimFrame_Backup;							//�A�j���[�V�����Đ��t���[���i�o�b�N�A�b�v�j
	int m_FrameCnt_Backup;							//�Đ��t���[���v�Z�p�i�o�b�N�A�b�v�j

	bool m_bBlendAnim;								//�u�����h���[�h
	int m_BlendTimer;								//�u�����h�p�^�C�}

	//�v���g�^�C�v�錾
	void UpdateAnimation(ANIM_PACK& AnimData) noexcept;				//�A�j���[�V�����X�V
	void UpdateAnimationBlending(ANIM_PACK& AnimData) noexcept;		//�A�j���[�V�����u�����h�X�V

	//�����w��
	friend class MESH;
};
