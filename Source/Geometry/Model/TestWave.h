/**
 * @file Shape_Model.h
 * @brief �g�`��
 * @author ���J�C�A��
 * @date 2023/01/10
 * @���� 2023/01/10�F�t�@�C���쐬
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
class WAVE : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	WAVE(APP& App);
	~WAVE() noexcept override;
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

	float ClacSineWave(float Dist, float Time, bool bLoop = true);			//�����g�ɂ�鍂���v�Z
	float ClacRingWave(VECTOR2 OriPos, VECTOR2 Pos, float Time);			//��g�ɂ�鍂���v�Z
	float ClacStraightWave(VECTOR2 vDirection, VECTOR2 vPos, float Time);	//���i�g�ɂ�鍂���v�Z

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	int m_InstanceNum;								//�C���X�^���X��
	std::vector<VSD_INSTANCE> m_aInstanceData;		//�C���X�^���X���
	CBD_MATERIAL m_Material;						//�}�e���A�����

	INPUT_MGR& m_Input;								//���͎Q��
	std::vector<DirectX::XMFLOAT3> m_aInitPos;		//�e���q�̏����ʒu
	float m_WaveLength;								//�g�̔g��
	float m_WaveCoef;								//�g�W���i�U����0-1�Ő���j
};
