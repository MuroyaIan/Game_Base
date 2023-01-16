/**
 * @file Mouse.h
 * @brief �}�E�X����
 * @author ���J�C�A��
 * @date 2022/05/03
 * @���� 2022/05/03�F�N���X�쐬
 *		 2022/11/15�FOS�ʏ����ւ̑Ή�
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <utility>				//std::pair
#include <queue>				//FIFO�R���e�i

//===== �\���̒�` =====
struct MOUSE_INFO				//�}�E�X���
{
	//�ϐ��錾
	bool bLeftIsPressed;		//���N���b�N
	bool bRightIsPressed;		//�E�N���b�N
	int nPosX;
	int nPosY;					//�}�E�X���W
	bool bIsInWindow;			//�E�B���h�E���ɂ���

	//�R���X�g���N�^
	MOUSE_INFO() noexcept :
		bLeftIsPressed(false), bRightIsPressed(false),
		nPosX(0), nPosY(0), bIsInWindow(false)
	{}
	MOUSE_INFO(const MOUSE_INFO& Parent) noexcept :
		bLeftIsPressed(Parent.bLeftIsPressed),
		bRightIsPressed(Parent.bRightIsPressed),
		nPosX(Parent.nPosX), nPosY(Parent.nPosY),
		bIsInWindow(Parent.bIsInWindow)
	{}
	~MOUSE_INFO() noexcept {}
};

//===== �N���X��` =====

//***** �}�E�X��� ****
enum class MOUSE_STATUS
{
	L_Press,
	L_Release,
	R_Press,
	R_Release,
	WheelUp,
	WheelDown,
	Move,
	EnterWindow,
	LeaveWindow,
	Invalid
};

//***** �}�E�X�C�x���g ****
class MOUSE_EVENTS
{
public:

	//�v���g�^�C�v�錾
	explicit MOUSE_EVENTS() noexcept;
	MOUSE_EVENTS(MOUSE_STATUS Type, const MOUSE_INFO& Parent) noexcept;
	~MOUSE_EVENTS() noexcept;

	bool IsValid() const noexcept					//�}�E�X�C�x���g�L���m�F
	{
		return (m_Type != MOUSE_STATUS::Invalid) ? true : false;
	}
	MOUSE_STATUS GetType() const noexcept			//�}�E�X��Ԏ擾
	{
		return m_Type;
	}
	std::pair<int, int> GetPos() const noexcept		//�}�E�XXY���W�擾
	{
		return{ m_Info.nPosX, m_Info.nPosY };
	}
	int GetPosX() const noexcept
	{
		return m_Info.nPosX;
	}
	int GetPosY() const noexcept
	{
		return m_Info.nPosY;
	}
	bool IsInWindow() const noexcept				//�E�B���h�E���ɂ��邩�m�F
	{
		return m_Info.bIsInWindow;
	}
	bool LeftIsPressed() const noexcept				//�}�E�X���E�N���b�N�m�F
	{
		return m_Info.bLeftIsPressed;
	}
	bool RightIsPressed() const noexcept
	{
		return m_Info.bRightIsPressed;
	}

private:

	//�ϐ��錾
	MOUSE_STATUS m_Type;	//�}�E�X�̏��
	MOUSE_INFO m_Info;		//�}�E�X���
};

//***** �}�E�X���� *****
class MOUSE
{
public:

	//RawInput�p�\���́i�}�E�X�ω��ʁj
	struct RAW_DELTA
	{
		int x;
		int y;
		bool bClear;	//NULL�f�[�^�ł��邱�Ƃ������t���O

		RAW_DELTA(bool bClr = false) noexcept : x(0), y(0), bClear(bClr)
		{}
		RAW_DELTA(int xIn, int yIn, bool bClr = false) noexcept :
			x(xIn), y(yIn), bClear(bClr)
		{}
		~RAW_DELTA() noexcept
		{}
	};

	//�v���g�^�C�v�錾
	explicit MOUSE() noexcept;
	~MOUSE() noexcept;
	std::pair<int, int> GetPos() const noexcept;	//�}�E�X���W�擾
	int GetPosX() const noexcept;					//�}�E�XX���W�擾
	int GetPosY() const noexcept;					//�}�E�XY���W�擾
	int GetWheelVal() const noexcept;				//�z�C�[���l�擾
	bool IsInWindow() const noexcept;				//�E�B���h�E���ɂ��邩�m�F
	bool LeftIsPressed() const noexcept;			//�}�E�X���N���b�N�m�F
	bool RightIsPressed() const noexcept;			//�}�E�X�E�N���b�N�m�F
	MOUSE_EVENTS ReadBuffer() noexcept;				//�o�b�t�@�Ǎ���
	bool IsEmpty() const noexcept;					//�o�b�t�@�̋󂫊m�F
	void ClearBuffer() noexcept;					//�o�b�t�@�N���A
	RAW_DELTA ReadRawDelta() noexcept;				//RawInput�o�b�t�@�Ǎ���
	void SetRawInput(bool bUse) noexcept;			//RawInput�g�p����
	bool IsUsingRawInput() const noexcept;			//RawInput�g�p��Ԋm�F

private:

	//�ϐ��錾
	static constexpr unsigned int nBufferSize = 16u;	//�L���[�̃T�C�Y�i�t���[�����Ƃ̍ő��t���j
	std::queue<MOUSE_EVENTS> m_Buffer;					//�}�E�X�C�x���g�p�L���[
	MOUSE_INFO m_Info;									//�}�E�X���
	int m_WheelDelta;									//�z�C�[�������
	int m_WheelVal;										//�z�C�[���l
	std::queue<RAW_DELTA> m_RawDeltaBuffer;				//RawInput�p�L���[
	bool m_bUseRawInput;								//RawInput�g�p�t���O

	//�v���g�^�C�v�錾
	void TruncateBuffer() noexcept;					//�o�b�t�@�؂�̂�
	void MouseMove(int PosX, int PosY) noexcept;	//�}�E�X�ړ�
	void LeaveWindow() noexcept;					//�E�B���h�E�O�ɍs��
	void Enterwindow() noexcept;					//�E�B���h�E���ɓ���
	void LeftPressed() noexcept;					//���N���b�N�I��
	void LeftReleased() noexcept;					//���N���b�N�I�t
	void RightPressed() noexcept;					//�E�N���b�N�I��
	void RightReleased() noexcept;					//�E�N���b�N�I�t
	void WheelUp() noexcept;						//�z�C�[���A�b�v
	void WheelDown() noexcept;						//�z�C�[���_�E��
	void WheelProc(int nDelta) noexcept;			//�z�C�[������
	void TruncateRawInputBuffer() noexcept;			//RawInput�o�b�t�@�؂�̂�
	void GetRawDelta(int dx, int dy) noexcept;		//RawInput���擾

	//�����w��
	friend class WIN_WINDOW;
};
