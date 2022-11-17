/**
 * @file DrawerRef.h
 * @brief �h�����[�̃e���v��
 * @author ���J�C�A��
 * @date 2022/07/05
 * @���� 2022/07/05�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Binder/IndexBuffer.h>

//===== �N���X��` =====

//***** �h�����[�e���v�� *****
template<class T>
class DRAWER_EX : public DRAWER
{
protected:

	//�v���g�^�C�v�錾
	DRAWER_EX() noexcept : DRAWER() {}
	~DRAWER_EX() noexcept override
	{
		for (auto& b : ms_aBinder)
			b.reset();
	}

	bool StaticIsInit() const noexcept						//�������m�F
	{
		return !ms_aBinder.empty();
	}

	void AddStaticBind(std::unique_ptr<BINDER> pBinder)		//�o�C���_�o�^�i�ÓI�j
	{
		//��O����
		if (pBinder == nullptr)
			throw ERROR_EX2("�y�x���z�|�C���^��NULL�ł��I");

		//�����C���f�b�N�X�̃o�C���h��h�~
		if (typeid(*pBinder) == typeid(INDEX_BUFFER)) {
			if (ms_pIndexBuffer != nullptr)
				throw ERROR_EX2("�y�x���zm_pIndexBuffer�͏㏑������܂��I");
			ms_pIndexBuffer = dynamic_cast<INDEX_BUFFER*>(pBinder.get());
		}

		//���L���ڍs
		ms_aBinder.push_back(std::move(pBinder));
	}

private:

	//�^�ȗ�
	using vec_upBinder = std::vector<std::unique_ptr<BINDER>>;

	//�ϐ��錾
	static vec_upBinder ms_aBinder;										//�o�C���_�̃|�C���^�z��i�ÓI�j
	static const INDEX_BUFFER* ms_pIndexBuffer;							//�C���f�b�N�X�o�b�t�@�̃|�C���^�i���L���Ȃ��A�d�����p�j

	//�v���g�^�C�v�錾
	const vec_upBinder& GetStaticBind() const noexcept override			//�ÓI�z��Q��
	{
		return ms_aBinder;
	}

	const INDEX_BUFFER& GetStaticIndexBuffer() const noexcept override	//�ÓI�C���f�b�N�X�o�b�t�@�Q��
	{
		return *ms_pIndexBuffer;
	}
};

//===== �ÓI�����o�ϐ� =====
template<class T>
std::vector<std::unique_ptr<BINDER>> DRAWER_EX<T>::ms_aBinder{};

template<class T>
const INDEX_BUFFER* DRAWER_EX<T>::ms_pIndexBuffer = nullptr;
