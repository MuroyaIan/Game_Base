/**
 * @file Bone.h
 * @brief �W�I���g���`��i�|���S���\���j
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== �O���錾 =====
class VIEWER;
class FBX_LOADER;

//===== �N���X��` =====

//***** �W�I���g���`��i�|���S���\���j *****
class BONE : public DRAWER_EX<BONE>
{
public:

	//�v���g�^�C�v�錾
	BONE(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, INPUT_MGR& Input);
	~BONE() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//�����ݏ���
	int AddInstance() override;																	//�C���X�^���X�ǉ�
	void ClearInstance() override;																//�C���X�^���X�N���A

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		(void)InstanceIndex;
		return m_MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//�|���S�����擾
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

	float& GetScale() noexcept																	//���b�V���X�P�[���Q��
	{
		return m_Scale;
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	int m_InstanceNum;								//�C���X�^���X��
	FBX_LOADER& m_Loader;							//���[�_�Q��

	std::vector<DirectX::XMFLOAT4X4> m_aMtxLocal;	//���[�J���s��
	bool& m_bDrawAnimation;							//�A�j���[�V�����Đ�
	int& m_AnimationID;								//�A�j���[�V�����ԍ�
	int& m_AnimFrame;								//�Đ�����t���[����
	float m_Scale;									//���b�V���X�P�[��

	DirectX::XMFLOAT4X4 m_MtxLocal;					//���[�J���s��
	DirectX::XMFLOAT4X4 m_MtxWorld;					//���[���h�s��
	float& m_ModelScale;							//���f���X�P�[��

	INPUT_MGR& m_Input;								//���͎Q��
	DirectX::XMFLOAT3 m_Rot;						//��]
};
