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
	explicit MESH(MODEL& ModelRef, int MeshIdx);
	~MESH() noexcept override;
	void Update() noexcept override;														//�X�V����
	void Draw(int InstanceNum = 0) const noexcept override;									//�����ݏ���
	int AddInstance() override;																//�C���X�^���X�ǉ�

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override		//���[���h�s��擾
	{
		return m_aInstanceData[InstanceIndex].MtxWorld;
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept override	//���[���h�s��ݒ�
	{
		m_aInstanceData[InstanceIndex].MtxWorld = mtxW;
	}

	UINT GetPolygonNum() const noexcept override											//�|���S�����擾
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	int& m_InstanceNum;								//�C���X�^���X���Q��
	std::vector<VSD_INSTANCE>& m_aInstanceData;		//�C���X�^���X���Q��
	CBD_MATERIAL m_Material;						//�}�e���A�����

	ModelRef::MODEL_PACK& m_FileData;				//�t�@�C�����
	int m_MeshIdx;									//���b�V���ԍ�

	bool& m_bStatic;								//�ÓI���b�V�����ǂ���
	std::unique_ptr<CBD_MTX_LOCAL> m_pLocalData;	//���[�J�����
	int& m_AnimID;									//�A�j���[�V�����ԍ�
	int& m_AnimFrame;								//�A�j���[�V�����Đ��t���[��
};
