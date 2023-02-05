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
#include <App.h>

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
		Test,
		FirstPerson,

		MAX_CAMERA
	};

	//�v���g�^�C�v�錾
	explicit CAMERA_MGR(APP& App) noexcept;
	~CAMERA_MGR() noexcept;
	void Update() const noexcept;									//�X�V����
	void SetCamera(CAMERA_ID id) noexcept;							//�J�����Z�b�g
	DirectX::XMFLOAT4X4 GetWorldMtx(CAMERA_ID id) const noexcept;	//���[���h�s��擾
	DirectX::XMFLOAT3 GetRotation(CAMERA_ID id) const noexcept;		//��]���擾

private:

	//�ϐ��錾
	APP& m_App;											//App�Q��
	std::vector<std::unique_ptr<CAMERA>> m_aCamera;		//�J�����̃|�C���^�z��
	CAMERA_ID m_CurrentCamera;							//���ݎg�p���̃J����
};
