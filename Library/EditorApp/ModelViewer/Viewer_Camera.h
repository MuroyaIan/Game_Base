/**
 * @file Viewer_Camera.h
 * @brief �r���[���p�J����
 * @author ���J�C�A��
 * @date 2022/07/19
 * @���� 2022/07/19�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <EditorApp/Editor.h>

//===== �N���X��` =====

//***** �r���[���p�J���� *****
class VIEWER_CAM
{
public:

	//�v���g�^�C�v�錾
	VIEWER_CAM(APP& App) noexcept;
	~VIEWER_CAM() noexcept;
	void Update() noexcept;									//�X�V����
	void UpdateMatrix() noexcept;							//�s��X�V
	void ResetCamera() noexcept;							//�J�������Z�b�g

	DirectX::XMFLOAT3 GetRotation() const noexcept			//��]�p�i���]�j�擾
	{
		return m_Rot;
	}
	DirectX::XMFLOAT3 GetPosition() const noexcept			//�ʒu�擾
	{
		return m_Pos;
	}
	int& GetZoomSpeed() noexcept							//�Y�[�����x�擾
	{
		return m_ZoomSPD;
	}
	float GetCenterDistance() const noexcept				//���S�����擾
	{
		return m_CtrDist;
	}

private:

	//�J�������[�h
	enum class MODE
	{
		NONE,	//����Ȃ�
		ORBIT,	//�O���i���]�j���[�h
		TRACK,	//�����_�ړ��i���_�������j
	};

	//�ϐ��錾
	APP& m_App;						//App�Q��

	DirectX::XMFLOAT3 m_LookAt;			//�����_
	DirectX::XMFLOAT3 m_vUp;			//�A�b�v�x�N�g��

	short m_FOV;						//����p
	float g_AspectRatio;				//�c����
	float m_NearZ;
	float m_FarZ;						//�N���b�v����

	DirectX::XMFLOAT4X4 m_mtxWorld;		//���[���h�s��
	DirectX::XMFLOAT4X4 m_mtxView;		//�r���[�s��
	DirectX::XMFLOAT4X4 m_mtxProj;		//���e�s��

	MODE m_Mode;						//���샂�[�h
	DirectX::XMFLOAT2 m_MousePos_Old;	//�}�E�X���W�i���O�t���[���j
	DirectX::XMFLOAT3 m_Rot;			//��]�p�i���]�j
	int m_Level_RotSPD;					//���]���x
	DirectX::XMFLOAT3 m_Pos;			//3D���W
	int m_Level_Zoom;					//�Y�[�����x��
	int m_Backup_Wheel;					//�}�E�X�z�C�[���̑����
	int m_ZoomSPD;						//�Y�[�����x
	float m_CtrDist;					//���S����
	DirectX::XMFLOAT3 m_Offset;			//���Έړ���
};
