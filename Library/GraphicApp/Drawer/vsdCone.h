/**
 * @file vsdCone.h
 * @brief �R�[���`��e���v���[�g
 * @author ���J�C�A��
 * @date 2022/07/09
 * @���� 2022/07/09�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/VertexShaderData.h>
#include <Tool/gMath.h>

//===== �N���X��` =====

//***** �R�[���`�� *****
class VSD_CONE
{
public:

	//�v���g�^�C�v�錾
	template<class V>
	static VS_DATA<V> MakeTessellation(int DivNum)	//�e�b�Z���[�V�����쐬
	{
		//�O����
		namespace dx = DirectX;
		if (DivNum < 3)
			throw ERROR_EX2("��������3�ȏ�ɂ��Ă��������B");

		//�ϐ��錾
		const dx::XMVECTOR vBase = dx::XMVectorSet(0.5f, -0.5f, 0.0f, 0.0f);	//��p���_�x�N�g��
		const float fDivAngle = 2.0f * fPI / DivNum;							//�����p��]�p(���W�A��)
		std::vector<V> aData;

		//�ꕔ�~�`��̒��_�쐬
		for (int i = 0; i < DivNum; i++) {
			aData.emplace_back();
			dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));		//Y����]
			dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
		}

		//�ꕔ�~�`�̒��S���_�쐬
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, -0.5f, 0.0f };
		const unsigned short usCtrIdx = static_cast<unsigned short>(aData.size() - 1);	//�C���f�b�N�X�Q��

		//�R�[���̐�[���_�쐬
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, 0.5f, 0.0f };
		const unsigned short usTopIdx = static_cast<unsigned short>(aData.size() - 1);

		//�ꕔ�~�`��̃C���f�b�N�X���X�g�쐬
		std::vector<unsigned short> Indices;
		const unsigned short Mod = static_cast<unsigned short>(DivNum);
		for (unsigned short i = 0; i < DivNum; i++) {
			Indices.push_back(usCtrIdx);
			Indices.push_back((i + 1u) % Mod);
			Indices.push_back(i);
		}

		//���̑��̃C���f�b�N�X���X�g�쐬
		for (unsigned short i = 0; i < DivNum; i++) {
			Indices.push_back(usTopIdx);
			Indices.push_back(i);
			Indices.push_back((i + 1u) % Mod);
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData(int DivNum = 24)		//�f�[�^�쐬
	{
		return MakeTessellation<V>(DivNum);
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int DivNum = 24)	//�f�[�^�쐬�i���f���p�j
	{
		VS_DATA<V> vsd = MakeTessellation<V>(DivNum);
		vsd.ResetDataForModel();
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//�v���g�^�C�v�錾
	VSD_CONE() noexcept {}
	~VSD_CONE() noexcept {}
};
