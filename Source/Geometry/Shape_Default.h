/**
 * @file Shape_Default.h
 * @brief �W�I���g���`��i�|���S���\���j
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== �N���X��` =====

//***** �W�I���g���`��i�|���S���\���j *****
class SHAPE_DEFAULT : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	explicit SHAPE_DEFAULT(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type);
	~SHAPE_DEFAULT() noexcept override;
	void Update() noexcept override;														//�X�V����
	void Draw(int InstanceNum = 0) const noexcept override;									//�����ݏ���
	int AddInstance() override;																//�C���X�^���X�ǉ�

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override		//�ό`�s��擾
	{
		return m_aMtxWorld[InstanceIndex];
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept override	//�ό`�s��ݒ�
	{
		m_aMtxWorld[InstanceIndex] = mtxW;
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
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//���[���h�s��
};
