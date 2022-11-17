/**
 * @file TestPlayer.h
 * @brief �ۑ�p�v���C���[
 * @author ���J�C�A��
 * @date 2022/09/27
 * @���� 2022/09/27�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <App_Win64.h>

//===== �O���錾 =====
class MODEL;

//===== �N���X��` =====

//***** ���f�� *****
class PLAYER
{
public:

	//�v���g�^�C�v�錾
	PLAYER(APP_64& App) noexcept;
	~PLAYER() noexcept;
	void Update() noexcept;							//�X�V����
	void Draw() const noexcept(!IS_DEBUG);			//�����ݏ���

private:

	//�ϐ��錾
	std::unique_ptr<MODEL> m_pModel;	//���f��
	INPUT_MGR& m_Input;					//���͎Q��

};
