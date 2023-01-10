/**
 * @file Mesh.h
 * @brief ���b�V���`��
 * @author ���J�C�A��
 * @date 2022/09/08
 * @���� 2022/09/08�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <Geometry/Model/Model.h>

//===== �O���錾 =====
struct CBD_MTX_LOCAL;

//===== �N���X��` =====

//***** ���b�V���`�� *****
class MESH : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	MESH(MODEL& ModelRef, int MeshIdx);
	~MESH() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(int InstanceNum = -1) const noexcept override;		//�����ݏ���
	int AddInstance() override;																	//�C���X�^���X�ǉ�

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		return m_aInstanceData[InstanceIndex].MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//�|���S�����擾
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

private:

	//�ϐ��錾
	ModelRef::MODEL_PACK& m_FileData;				//�t�@�C�����
	int m_MeshIdx;									//���b�V���ԍ�
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	int& m_InstanceNum;								//�C���X�^���X���Q��
	std::vector<INSTANCE_DATA>& m_aInstanceData;	//�C���X�^���X���Q��
	CBD_MATERIAL m_Material;						//�}�e���A�����

	bool& m_bStatic;								//�ÓI���b�V�����ǂ���
	std::unique_ptr<CBD_MTX_LOCAL> m_pLocalData;		//���[�J�����
	int& m_AnimID;									//�A�j���[�V�����ԍ�
	int& m_AnimID_Backup;							//�A�j���[�V�����ԍ��i�o�b�N�A�b�v�j
	int& m_AnimFrame;								//�A�j���[�V�����Đ��t���[��
	int& m_AnimFrame_Backup;						//�A�j���[�V�����Đ��t���[���i�o�b�N�A�b�v�j
	bool& m_bBlendAnim;								//�u�����h���[�h
	int& m_BlendTimer;								//�u�����h�p�^�C�}
};
