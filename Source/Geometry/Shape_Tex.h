/**
 * @file Shape_Tex.h
 * @brief �W�I���g���`��i�|���S���\���j
 * @author ���J�C�A��
 * @date 2022/08/13
 * @���� 2022/08/13�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== �N���X��` =====

//***** �W�I���g���`��i�e�N�X�`������j *****
class SHAPE_TEX : public DRAWER_EX<SHAPE_TEX>
{
public:

	//�v���g�^�C�v�錾
	SHAPE_TEX(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type, TEXTURE_MGR::TEX_ID Tex);
	~SHAPE_TEX() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//�����ݏ���
	int AddInstance() override;																	//�C���X�^���X�ǉ�

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		return m_aMtxWorld[InstanceIndex];
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
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//���[���h�s��
	TEXTURE_MGR::TEX_ID m_Tex;						//�e�N�X�`�����

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
