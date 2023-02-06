/**
 * @file IM_GamePad.h
 * @brief �R���g���[������
 * @author ���J�C�A��
 * @date 2022/07/28
 * @���� 2022/07/28�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/WinEO.h>			//�G���[�o��
#include <Xinput.h>					//XBOX�R���g���[���[����
#include <DirectXMath.h>

//===== �萔�E�}�N����` =====
constexpr int DS4_UP		= 0;
constexpr int DS4_DOWN		= 1;
constexpr int DS4_LEFT		= 2;
constexpr int DS4_RIGHT		= 3;
constexpr int DS4_OPTIONS	= 4;
constexpr int DS4_SHARE		= 5;
constexpr int DS4_L3		= 6;
constexpr int DS4_R3		= 7;
constexpr int DS4_L1		= 8;
constexpr int DS4_R1		= 9;
constexpr int DS4_L2		= 10;
constexpr int DS4_R2		= 11;
constexpr int DS4_CROSS		= 12;
constexpr int DS4_CIRCLE	= 13;
constexpr int DS4_SQUARE	= 14;
constexpr int DS4_TRIANGLE	= 15;
constexpr int PAD_BTN_NUM	= 16;	//�ő�{�^����

enum eGAMEPAD_ID					//�R���g���[��ID�i�ő�ڑ���4�j
{
	PAD_1,
	PAD_2,
	PAD_3,
	PAD_4,

	MAX_PAD_NUM
};

//===== �O���錾 =====
class APP;

//===== �N���X��` =====

//***** �R���g���[������ *****
class INPUT_PAD
{
public:

	//* �v���g�^�C�v�錾 *
	explicit INPUT_PAD(APP& App) noexcept;
	~INPUT_PAD() noexcept;

	bool CheckPad(eGAMEPAD_ID nPadID) const noexcept;						//�R���g���[���[�ڑ��m�F
	bool GetPress(eGAMEPAD_ID nPadID, int nBtn) const noexcept;				//�v���X����
	bool GetTrigger(eGAMEPAD_ID nPadID, int nBtn) const noexcept;			//�g���K�[����
	bool GetRelease(eGAMEPAD_ID nPadID, int nBtn) const noexcept;			//�����[�X����
	bool GetRepeat(eGAMEPAD_ID nPadID, int nBtn, float fInterval = 0.5f);	//���s�[�g����i�f�t�H���g0.5�b�j

	void UseVibration(eGAMEPAD_ID nPadID, bool bStart) noexcept;			//�U���I���E�I�t
	void SetVibration(eGAMEPAD_ID nPadID, float fPower);
	void SetVibration_L(eGAMEPAD_ID nPadID, float fPower);
	void SetVibration_R(eGAMEPAD_ID nPadID, float fPower);					//�U�����x�ݒ�

	DirectX::XMFLOAT2 GetStickL3(eGAMEPAD_ID nPadID) const noexcept
	{
		return m_Val_L3[nPadID];
	}
	DirectX::XMFLOAT2 GetStickR3(eGAMEPAD_ID nPadID) const noexcept			//���E�X�e�B�b�N�̌X���ʎ擾
	{
		return m_Val_R3[nPadID];
	}
	float GetValL2(eGAMEPAD_ID nPadID) const noexcept
	{
		return m_Val_L2[nPadID];
	}
	float GetValR2(eGAMEPAD_ID nPadID) const noexcept						//���E�g���K�[�̉����ʎ擾
	{
		return m_Val_R2[nPadID];
	}
	float GetVibeL(eGAMEPAD_ID nPadID) const noexcept
	{
		return m_Vibe_L[nPadID];
	}
	float GetVibeR(eGAMEPAD_ID nPadID) const noexcept						//���E�U���l�擾
	{
		return m_Vibe_R[nPadID];
	}

private:

	using uShort = unsigned short;

	//* �ϐ��錾 *
	APP& m_App;											//App�Q��
	XINPUT_STATE m_State[MAX_PAD_NUM];					//PAD���
	DWORD m_PadUsed[MAX_PAD_NUM];						//PAD�ڑ����
	XINPUT_VIBRATION m_Vibration[MAX_PAD_NUM];			//�U�����
	bool m_bUseVibration[MAX_PAD_NUM];					//�U���g�p����

	uShort m_Press[MAX_PAD_NUM][PAD_BTN_NUM];			//������Ԕ���p
	uShort m_PrevPress[MAX_PAD_NUM][PAD_BTN_NUM];		//������Ԕ���p(�O����s��)
	uShort m_Trigger[MAX_PAD_NUM][PAD_BTN_NUM];			//�g���K��Ԕ���p
	uShort m_Release[MAX_PAD_NUM][PAD_BTN_NUM];			//�����[�X��Ԕ���p
	uShort m_Repeat[MAX_PAD_NUM][PAD_BTN_NUM];			//���s�[�g��Ԕ���p
	uShort m_RepeatCnt[MAX_PAD_NUM][PAD_BTN_NUM];		//���s�[�g�J�E���^
	uShort m_RepeatInterval[MAX_PAD_NUM][PAD_BTN_NUM];	//���s�[�g�Ԋu
	uShort* m_pPress;
	uShort* m_pPrevPress;
	uShort* m_pTrigger;
	uShort* m_pRelease;
	uShort* m_pRepeat;
	uShort* m_pRepeatCnt;
	uShort* m_pRepeatInterval;							//���[�v�����p�|�C���^

	float m_Val_L2[MAX_PAD_NUM];
	float m_Val_R2[MAX_PAD_NUM];						//���E�g���K�[�̓��͗ʁi0�`255�j
	DirectX::XMFLOAT2 m_Val_L3[MAX_PAD_NUM];
	DirectX::XMFLOAT2 m_Val_R3[MAX_PAD_NUM];			//���E�X�e�B�b�N�̌X���ʁi-32768�`32767�j�ˁ@������W�n
	float m_Vibe_L[MAX_PAD_NUM];
	float m_Vibe_R[MAX_PAD_NUM];						//���E�̐U���l�i0�`65535�j

	//* �v���g�^�C�v�錾 *
	void UpdateInputData(int nPadID) noexcept;			//���͏��X�V
	void Update() noexcept;								//�X�V����

	//�����w��
	friend class INPUT_MGR;
};
