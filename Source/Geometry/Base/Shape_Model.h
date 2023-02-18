/**
 * @file Shape_Model.h
 * @brief �W�I���g���`��i���f���\���j
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>
#include <App.h>

//===== �N���X��` =====

//***** �W�I���g���`��i���f���\���j *****
class SHAPE_MODEL : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	explicit SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type);
	~SHAPE_MODEL() noexcept override;
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
	VSD_MAKER::SHAPE m_Type;						//�`��^�C�v
	int m_InstanceNum;								//�C���X�^���X��
	std::vector<VSD_INSTANCE> m_aInstanceData;		//�C���X�^���X���
	CBD_MATERIAL m_Material;						//�}�e���A�����
};
