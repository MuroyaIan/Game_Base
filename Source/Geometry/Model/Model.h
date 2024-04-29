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
class CT_BINDER;

//===== �N���X��` =====

//***** ���f�� *****
class MODEL
{
public:

	//�e�N�X�`���p�b�N
	struct ANIM_PACK
	{
		int ID;					//�A�j���[�V�����ԍ�
		int CurrentFrame;		//�A�j���[�V�����Đ��t���[��
		int FrameCnt;			//�Đ��t���[���v�Z�p
		int BlendID;			//�A�j���[�V�����ԍ��i�u�����h�p�j
		int BlendCurrentFrame;	//�A�j���[�V�����Đ��t���[���i�u�����h�p�j
		int BlendFrameCnt;		//�Đ��t���[���v�Z�p�i�u�����h�p�j
		float AnimLerp;			//�A�j���[�V�����̐��`��ԁi�u�����h�p�j
		bool bBlendAnim;		//�u�����h���[�h
		int BlendTimer;			//�u�����h�p�^�C�}
		int BlendTime;			//�u�����h����
		bool bBlendSync;		//�u�����h�������[�h

		ANIM_PACK() noexcept :
			ID(0), CurrentFrame(0), FrameCnt(0),
			BlendID(0), BlendCurrentFrame(0), BlendFrameCnt(0), AnimLerp(0.0f),
			bBlendAnim(false), BlendTimer(0), BlendTime(0), bBlendSync(false)
		{}
		~ANIM_PACK() noexcept
		{}
	};

	//�v���g�^�C�v�錾
	explicit MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept;
	~MODEL() noexcept;
	void Update();																	//�X�V����
	void Draw() noexcept;															//�����ݏ���
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

	void ChangeAnimID(int id, int InstNum, float BlendTime = 1.0f, bool Sync = false) noexcept	//�A�j���[�V�����ύX
	{
		ANIM_PACK& AnimData = m_aAnimData[InstNum];

		//�A�j���[�V�����ؑց��u�����h���ł͂Ȃ�
		if (AnimData.ID != id && !AnimData.bBlendAnim) {

			//�u�����h���X�V
			AnimData.bBlendAnim = true;
			AnimData.BlendTimer = 0;
			AnimData.BlendTime = static_cast<int>(BlendTime * 60) - 1;
			AnimData.bBlendSync = Sync;
			AnimData.BlendID = id;
			AnimData.BlendCurrentFrame = 0;
			AnimData.BlendFrameCnt = 0;
			AnimData.AnimLerp = 0.0f;

			//�t���[���̓�������
			if (AnimData.bBlendSync) {
				float FrameRatio = AnimData.CurrentFrame / (m_FileData.aAnimFrame[AnimData.ID] - 1.0f);						//�t���[�������p�̊����擾
				AnimData.BlendCurrentFrame = static_cast<int>((m_FileData.aAnimFrame[AnimData.BlendID] - 1) * FrameRatio);	//�u�����h��̃t���[���Z�o
			}
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

	//�v���g�^�C�v�錾
	void UpdateAnimation(ANIM_PACK& AnimData) const noexcept;		//�A�j���[�V�����X�V
	void UpdateAnimationBlending(ANIM_PACK& AnimData) noexcept;		//�A�j���[�V�����u�����h�X�V

	//�����w��
	friend class MESH;
};
