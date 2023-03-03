/**
 * @file Drawer.h
 * @brief DirectX�̕`�揈��
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Graphic.h>

//===== �O���錾 =====
class BINDER;
class INDEX_BUFFER;
class VERTEX_BUFFER;

//===== �N���X��` =====

//***** �h�����[ *****
class DRAWER
{
public:

	//�R�s�[NG
	DRAWER(const DRAWER&) = delete;
	DRAWER& operator=(const DRAWER&) = delete;

	//�v���g�^�C�v�錾
	explicit DRAWER(const GRAPHIC& Gfx) noexcept;
	virtual ~DRAWER() noexcept;

	virtual void Update() = 0;																	//�X�V����
	virtual void Draw(int InstanceNum = 0);														//�`�揈��
	virtual UINT GetPolygonNum() const noexcept = 0;											//�|���S�����擾
	virtual DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept = 0;		//���[���h�s��擾
	virtual void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept = 0;	//���[���h�s��ݒ�

	virtual int AddInstance()																	//�C���X�^���X�ǉ�
	{
		throw ERROR_EX2("�g�p����ꍇ�͎q�N���X�Ōp�����Ă��������B");
	}

	virtual void ClearInstance()																//�C���X�^���X�N���A
	{
		throw ERROR_EX2("�g�p����ꍇ�͎q�N���X�Ōp�������������B");
	}

protected:

	//�ϐ��錾
	const GRAPHIC& m_Gfx;								//�O���t�B�b�N�Q�Ɛ�

	//�v���g�^�C�v�錾
	void AddBind(std::unique_ptr<BINDER> pBinder);		//�o�C���_�o�^
	UINT GetIndexNum() const noexcept;					//�C���f�b�N�X���擾

	VERTEX_BUFFER& GetVertexBuffer() const noexcept		//���_�o�b�t�@�Q��
	{
		return *m_pVertexBuffer;
	}

	void SetInstanceNum(int Num)						//�C���X�^���X���ݒ�
	{
		if (Num < 0)
			throw ERROR_EX2("�C���X�^���X����1�ȏ�łȂ���΂Ȃ�Ȃ��B");
		m_MaxInstanceNum = Num;
	}

private:

	//�ϐ��錾
	std::vector<std::unique_ptr<BINDER>> m_aBinder;		//�o�C���_�̃|�C���^�z��
	const INDEX_BUFFER* m_pIndexBuffer;					//�C���f�b�N�X�o�b�t�@�̃|�C���^�i���L���Ȃ��j
	VERTEX_BUFFER* m_pVertexBuffer;						//���_�o�b�t�@�̃|�C���^�i�C���X�^���X�X�V�p�j
	int m_MaxInstanceNum;								//�ő�C���X�^���X��

	//�v���g�^�C�v�錾
	void DrawProc(int InstanceNum) const noexcept;		//�`�揈���i�ŏI�����j
};
