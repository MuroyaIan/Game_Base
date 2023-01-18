/**
 * @file Editor.h
 * @brief �G�f�B�^�N���X
 * @author ���J�C�A��
 * @date 2022/07/19
 * @���� 2022/07/19�F�t�@�C���쐬
 *					 ver0.2��FbxViewer����
 *					 ver0.4�˃X�P���g���A�j���[�V��������
 *					 ver0.5�˓��͏������P�A���̑��C��
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <App.h>

//===== �O���錾 =====
class VIEWER;

//===== �N���X��` =====

//***** �G�f�B�^ *****
class EDITOR
{
public:

	//�v���g�^�C�v�錾
	explicit EDITOR(APP& App) noexcept;
	~EDITOR() noexcept;
	void Update();						//�X�V����
	void Draw();						//�`�揈��

	bool IsEnabled() const noexcept		//��Ԋm�F
	{
		return m_bEnable;
	}

private:

	//�ϐ��錾
	bool m_bEnable;						//�G�f�B�^���
	APP& m_App;							//App�Q��
	bool m_bFBX_Viewer;					//FBX�r���[���[
	std::unique_ptr<VIEWER> m_pViewer;	//�r���[���p�|�C���^
};
