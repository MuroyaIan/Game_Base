/**
 * @file Mouse.h
 * @brief �}�E�X����
 * @author ���J�C�A��
 * @date 2022/05/03
 * @���� 2022/05/03�F�N���X�쐬
 *		 2022/11/15�FOS�ʏ����ւ̑Ή�
 *		 2024/04/06�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <utility>				//std::pair
#include <queue>				//FIFO�R���e�i

//===== �\���̒�` =====

//***** �}�E�X��� *****
struct ST_MOUSE_INFO
{
	//�R�s�[�����[�u
	ST_MOUSE_INFO(const ST_MOUSE_INFO&) = default;
	ST_MOUSE_INFO& operator =(const ST_MOUSE_INFO&) = default;
	ST_MOUSE_INFO(ST_MOUSE_INFO&&) noexcept = default;
	ST_MOUSE_INFO& operator=(ST_MOUSE_INFO&&) noexcept = default;

	//�ϐ��錾
	bool ms_bLeftIsPressed;		//���N���b�N
	bool ms_bRightIsPressed;	//�E�N���b�N
	int ms_PosX;
	int ms_PosY;				//�}�E�X���W
	bool ms_bIsInWindow;		//�E�B���h�E���ɂ���

	//�R���X�g���N�^
	explicit ST_MOUSE_INFO() noexcept
		: ms_bLeftIsPressed(false)
		, ms_bRightIsPressed(false)
		, ms_PosX(0)
		, ms_PosY(0)
		, ms_bIsInWindow(false) {}

	~ST_MOUSE_INFO() noexcept = default;
};

//===== �N���X��` =====

//***** �}�E�X��� ****
enum class ET_MOUSE_STATUS
{
	me_L_Press,
	me_L_Release,
	me_R_Press,
	me_R_Release,
	me_WheelUp,
	me_WheelDown,
	me_Move,
	me_EnterWindow,
	me_LeaveWindow,
	me_Invalid
};

//***** �}�E�X�C�x���g ****
class CT_MOUSE_EVENTS
{
public:

	//�R�s�[�����[�u
	CT_MOUSE_EVENTS(const CT_MOUSE_EVENTS&) = default;
	CT_MOUSE_EVENTS& operator =(const CT_MOUSE_EVENTS&) = default;
	CT_MOUSE_EVENTS(CT_MOUSE_EVENTS&&) noexcept = default;
	CT_MOUSE_EVENTS& operator=(CT_MOUSE_EVENTS&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_MOUSE_EVENTS() noexcept;
	CT_MOUSE_EVENTS(const ET_MOUSE_STATUS& type, const ST_MOUSE_INFO& parent) noexcept;
	~CT_MOUSE_EVENTS() noexcept;

	[[nodiscard]] bool IsValid() const noexcept //�}�E�X�C�x���g�L���m�F
	{
		return m_Type != ET_MOUSE_STATUS::me_Invalid;
	}

	[[nodiscard]] ET_MOUSE_STATUS GetType() const noexcept //�}�E�X��Ԏ擾
	{
		return m_Type;
	}

	[[nodiscard]] std::pair<int, int> GetPos() const noexcept //�}�E�XXY���W�擾
	{
		return {m_Info.ms_PosX, m_Info.ms_PosY};
	}

	[[nodiscard]] int GetPosX() const noexcept
	{
		return m_Info.ms_PosX;
	}

	[[nodiscard]] int GetPosY() const noexcept
	{
		return m_Info.ms_PosY;
	}

	[[nodiscard]] bool IsInWindow() const noexcept //�E�B���h�E���ɂ��邩�m�F
	{
		return m_Info.ms_bIsInWindow;
	}

	[[nodiscard]] bool LeftIsPressed() const noexcept //�}�E�X���E�N���b�N�m�F
	{
		return m_Info.ms_bLeftIsPressed;
	}

	[[nodiscard]] bool RightIsPressed() const noexcept
	{
		return m_Info.ms_bRightIsPressed;
	}

private:

	//�ϐ��錾
	ET_MOUSE_STATUS m_Type; //�}�E�X�̏��
	ST_MOUSE_INFO m_Info;   //�}�E�X���
};

//***** �}�E�X���� *****
class CT_MOUSE
{
public:

	//RawInput�p�\���́i�}�E�X�ω��ʁj
	struct ST_RAW_DELTA
	{
		//�R�s�[�����[�u
		ST_RAW_DELTA(const ST_RAW_DELTA&) = default;
		ST_RAW_DELTA& operator =(const ST_RAW_DELTA&) = default;
		ST_RAW_DELTA(ST_RAW_DELTA&&) noexcept = default;
		ST_RAW_DELTA& operator=(ST_RAW_DELTA&&) noexcept = default;

		//�ϐ��錾
		int ms_X;
		int ms_Y;
		bool ms_bClear; //NULL�f�[�^�ł��邱�Ƃ������t���O

		//�R���X�g���N�^
		explicit ST_RAW_DELTA(const bool& bClr = false) noexcept
			: ms_X(0)
			, ms_Y(0)
			, ms_bClear(bClr) {}

		ST_RAW_DELTA(const int& xIn, const int& yIn, const bool& bClr = false) noexcept
			: ms_X(xIn)
			, ms_Y(yIn)
			, ms_bClear(bClr) {}

		~ST_RAW_DELTA() noexcept = default;
	};

	//�R�s�[�����[�u
	CT_MOUSE(const CT_MOUSE&) = default;
	CT_MOUSE& operator =(const CT_MOUSE&) = default;
	CT_MOUSE(CT_MOUSE&&) noexcept = default;
	CT_MOUSE& operator=(CT_MOUSE&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_MOUSE() noexcept;
	~CT_MOUSE() noexcept;
	[[nodiscard]] std::pair<int, int> GetPos() const noexcept; //�}�E�X���W�擾
	[[nodiscard]] int GetPosX() const noexcept;                //�}�E�XX���W�擾
	[[nodiscard]] int GetPosY() const noexcept;                //�}�E�XY���W�擾
	[[nodiscard]] int GetWheelVal() const noexcept;            //�z�C�[���l�擾
	[[nodiscard]] bool IsInWindow() const noexcept;            //�E�B���h�E���ɂ��邩�m�F
	[[nodiscard]] bool LeftIsPressed() const noexcept;         //�}�E�X���N���b�N�m�F
	[[nodiscard]] bool RightIsPressed() const noexcept;        //�}�E�X�E�N���b�N�m�F
	CT_MOUSE_EVENTS ReadBuffer() noexcept;                     //�o�b�t�@�Ǎ���
	[[nodiscard]] bool IsEmpty() const noexcept;               //�o�b�t�@�̋󂫊m�F
	void ClearBuffer() noexcept;                               //�o�b�t�@�N���A
	ST_RAW_DELTA ReadRawDelta() noexcept;                      //RawInput�o�b�t�@�Ǎ���
	void SetRawInput(const bool& bUse) noexcept;               //RawInput�g�p����
	[[nodiscard]] bool IsUsingRawInput() const noexcept;       //RawInput�g�p��Ԋm�F

private:

	//�ϐ��錾
	static constexpr unsigned int c_BufferSize = 16u; //�L���[�̃T�C�Y�i�t���[�����Ƃ̍ő��t���j
	std::queue<CT_MOUSE_EVENTS> m_Buffer;             //�}�E�X�C�x���g�p�L���[
	ST_MOUSE_INFO m_Info;                             //�}�E�X���
	int m_WheelDelta;                                 //�z�C�[�������
	int m_WheelVal;                                   //�z�C�[���l
	std::queue<ST_RAW_DELTA> m_RawDeltaBuffer;        //RawInput�p�L���[
	bool m_bUseRawInput;                              //RawInput�g�p�t���O

	//�v���g�^�C�v�錾
	void TruncateBuffer() noexcept;                            //�o�b�t�@�؂�̂�
	void MouseMove(const int& posX, const int& posY) noexcept; //�}�E�X�ړ�
	void LeaveWindow() noexcept;                               //�E�B���h�E�O�ɍs��
	void EnterWindow() noexcept;                               //�E�B���h�E���ɓ���
	void LeftPressed() noexcept;                               //���N���b�N�I��
	void LeftReleased() noexcept;                              //���N���b�N�I�t
	void RightPressed() noexcept;                              //�E�N���b�N�I��
	void RightReleased() noexcept;                             //�E�N���b�N�I�t
	void WheelUp() noexcept;                                   //�z�C�[���A�b�v
	void WheelDown() noexcept;                                 //�z�C�[���_�E��
	void WheelProc(const int& nDelta) noexcept;                //�z�C�[������
	void TruncateRawInputBuffer() noexcept;                    //RawInput�o�b�t�@�؂�̂�
	void GetRawDelta(const int& dx, const int& dy) noexcept;   //RawInput���擾

	//�����w��
	friend class CT_IW_WIN;
};
