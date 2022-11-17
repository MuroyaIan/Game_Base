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
#include <typeinfo>					//�|�C���^���f�[�^�̌^�m�F

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

	//�v���g�^�C�v�錾
	DRAWER() noexcept;
	virtual ~DRAWER() noexcept;
	UINT GetIndexNum() const noexcept;														//�C���f�b�N�X���擾

	virtual void Update() noexcept = 0;														//�X�V����
	virtual void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG);	//�����ݏ���
	virtual DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept = 0;	//�ό`�s��擾
	virtual UINT GetPolygonNum() const noexcept = 0;										//�|���S�����擾
	virtual int AddInstance()																//�C���X�^���X�ǉ�
	{
		throw ERROR_EX2("�q�N���X�ŃC���X�^���X���s���Ă��������B");
	}
	virtual void ClearInstance()															//�C���X�^���X�N���A
	{
		throw ERROR_EX2("�q�N���X�ŃC���X�^���X���N���A���Ă��������B");
	}

protected:

	//�v���g�^�C�v�錾
	void AddBind(std::unique_ptr<BINDER> pBinder);		//�o�C���_�o�^

	void SetInstanceNum(int Num)						//�C���X�^���X���ݒ�
	{
		if (Num < 0)
			throw ERROR_EX2("�C���X�^���X����1�ȏ�łȂ���΂Ȃ�Ȃ��B");
		m_InstanceNum = Num;
	}

	VERTEX_BUFFER& GetVertexBuffer() const		//���_�o�b�t�@�Q��
	{
		return *m_pVertexBuffer;
	}

	virtual const std::vector<std::unique_ptr<BINDER>>& GetStaticBind() const noexcept = 0;		//�ÓI�z��Q��
	virtual const INDEX_BUFFER& GetStaticIndexBuffer() const noexcept = 0;						//�ÓI�C���f�b�N�X�o�b�t�@�Q��

private:

	//�ϐ��錾
	std::vector<std::unique_ptr<BINDER>> m_aBinder;		//�o�C���_�̃|�C���^�z��
	const INDEX_BUFFER* m_pIndexBuffer;					//�C���f�b�N�X�o�b�t�@�̃|�C���^�i���L���Ȃ��j
	int m_InstanceNum;									//�C���X�^���X��
	VERTEX_BUFFER* m_pVertexBuffer;						//���_�o�b�t�@�̃|�C���^�i�C���X�^���X�X�V�p�j
};
