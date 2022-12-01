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
#include <GraphicApp/Drawer/DrawerRef.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== �N���X��` =====

//***** �W�I���g���`��i���f���\���j *****
class SHAPE_MODEL : public DRAWER_EX<SHAPE_MODEL>
{
public:

	//�v���g�^�C�v�錾
	SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type);
	~SHAPE_MODEL() noexcept override;
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
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	VSD_MAKER::SHAPE m_Type;						//�`��^�C�v
	int m_InstanceNum;								//�C���X�^���X��
	std::vector<INSTANCE_DATA> m_aInstanceData;		//�C���X�^���X���
	MATERIAL_DATA m_Material;						//�}�e���A�����

	struct MTX_DATA
	{
		float m_r, dt;
		float m_roll, m_pitch, m_yaw;
		float m_theta, m_phi, m_chi;				//�ʒu���

		float m_droll, m_dpitch, m_dyaw;
		float m_dtheta, m_dphi, m_dchi;				//��]���x(�f���^/sec)
	};
	std::vector<MTX_DATA> m_aMtxData;				//�s��v�Z�p���
};
