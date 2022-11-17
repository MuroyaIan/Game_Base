/**
 * @file CameraMgr.h
 * @brief �J�����}�l�[�W��
 * @author ���J�C�A��
 * @date 2022/08/21
 * @���� 2022/08/21�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <App_Win64.h>

//===== �O���錾 =====
class CAMERA;

//===== �N���X��` =====

//***** �J�����}�l�[�W�� *****
class CAMERA_MGR
{
public:

	//�J����ID
	enum class CAMERA_ID
	{
		TEST,
		FP,

		MAX_CAMERA
	};

	//�v���g�^�C�v�錾
	CAMERA_MGR(APP_64& App) noexcept;
	~CAMERA_MGR() noexcept;
	void Update() noexcept;								//�X�V����
	void Draw() const noexcept;							//�`�揈��
	void SetCamera(CAMERA_ID id) noexcept;				//�J�����Z�b�g
	DirectX::XMFLOAT4X4 GetWorldMtx() const noexcept;	//���[���h�s��擾
	DirectX::XMFLOAT3 GetRotation() const noexcept;		//��]���擾

private:

	//�ϐ��錾
	APP_64& m_App;										//App�Q��
	DirectX::XMFLOAT4X4 m_mtxView;						//�r���[�s��
	DirectX::XMFLOAT4X4 m_mtxProj;						//���e�s��
	std::vector<std::unique_ptr<CAMERA>> m_aCamera;		//�J�����̃|�C���^�z��
	CAMERA_ID m_CurrentCamera;							//���ݎg�p���̃J����
};
