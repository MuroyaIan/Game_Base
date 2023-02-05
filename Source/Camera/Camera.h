/**
 * @file Camera.h
 * @brief �J�����̐e�N���X
 * @author ���J�C�A��
 * @date 2022/09/26
 * @���� 2022/09/26�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Camera/CameraMgr.h>

//===== �N���X��` =====

//***** �J�����̐e�N���X *****
class CAMERA
{
public:

	//�v���g�^�C�v�錾
	explicit CAMERA(APP& App) noexcept;
	virtual ~CAMERA() noexcept;
	virtual void Update() noexcept = 0;					//�X�V����

	DirectX::XMFLOAT4X4 GetWorldMtx() const noexcept	//���[���h�s��擾
	{
		return m_mtxWorld;
	}
	DirectX::XMFLOAT4X4 GetViewMtx() const noexcept		//�r���[�s��擾
	{
		return m_mtxView;
	}
	DirectX::XMFLOAT4X4 GetProjMtx() const noexcept		//���e�s��擾
	{
		return m_mtxProj;
	}
	DirectX::XMFLOAT3 GetRotation() const noexcept		//��]���擾
	{
		return m_Rot;
	}

protected:

	//�ϐ��錾
	APP& m_App;							//App�Q��

	DirectX::XMFLOAT3 m_Pos;			//���W
	DirectX::XMFLOAT3 m_LookAt;			//�����_
	DirectX::XMFLOAT3 m_vUp;			//�A�b�v�x�N�g��

	short m_FOV;						//����p
	float g_AspectRatio;				//�c����
	float m_NearZ;
	float m_FarZ;						//�N���b�v����

	DirectX::XMFLOAT4X4 m_mtxWorld;		//���[���h�s��
	DirectX::XMFLOAT4X4 m_mtxView;		//�r���[�s��
	DirectX::XMFLOAT4X4 m_mtxProj;		//���e�s��

	DirectX::XMFLOAT3 m_Rot;			//��]
	float m_Sensitivity;				//���x

	//�v���g�^�C�v�錾
	void InitMatrix() noexcept;					//�s�񏉊���
	void UpdateProjMatrix() noexcept;			//���e�s��X�V
	virtual void ResetCamera() noexcept = 0;	//�J�������Z�b�g
};
