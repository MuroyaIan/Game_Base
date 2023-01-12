/**
 * @file vsdCone.h
 * @brief �p���`��e���v���[�g
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

//***** �p���`�� *****
class VSD_PRISM
{
public:

	//�v���g�^�C�v�錾
	template<class V>
	static VS_DATA<V> MakeData(int DivNum = 24)		//�f�[�^�쐬
	{
		//�O����
		namespace dx = DirectX;
		if (DivNum < 3)
			throw ERROR_EX2("��������3�ȏ�ɂ��Ă��������B");

		//�ϐ��錾
		const dx::XMVECTOR vBase = dx::XMVectorSet(0.0f, -0.5f, 0.5f, 0.0f);	//��p���_�x�N�g��
		const dx::XMVECTOR vOffset = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	//�p���̍���
		const float fDivAngle = 2.0f * fPI / DivNum;							//�����p��]�p(���W�A��)
		std::vector<V> aData;

		//��ʁi���j�̒��S���_�쐬
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, -0.5f, 0.0f };
		const UINT usDownCtrIdx = static_cast<UINT>(aData.size() - 1);	//�C���f�b�N�X�Q��

		//��ʁi��j�̒��S���_�쐬
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, 0.5f, 0.0f };
		const UINT usUpCtrIdx = static_cast<UINT>(aData.size() - 1);

		//��ʒ��_�쐬
		for (int i = 0; i < DivNum; i++) {

			//��ʁi���j
			{
				aData.emplace_back();
				dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));		//Y����]
				dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
			}

			//��ʁi��j
			{
				aData.emplace_back();
				dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));		//Y����]
				vVertex = dx::XMVectorAdd(vVertex, vOffset);													//�I�t�Z�b�g�ǉ�
				dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
			}
		}

		//���ʂ̃C���f�b�N�X���X�g�쐬
		std::vector<UINT> Indices;
		const UINT Mod = static_cast<UINT>(DivNum * 2);
		for (int i = 0; i < DivNum; i++) {
			const UINT Idx = i * 2;
			Indices.emplace_back(Idx + 2u);
			Indices.emplace_back((Idx + 2u) % Mod + 2u);
			Indices.emplace_back(Idx + 3u);
			Indices.emplace_back((Idx + 2u) % Mod + 2u);
			Indices.emplace_back((Idx + 3u) % Mod + 2u);
			Indices.emplace_back(Idx + 3u);
		}

		//��ʂ̃C���f�b�N�X���X�g�쐬
		for (int i = 0; i < DivNum; i++) {
			const UINT Idx = i * 2;
			Indices.emplace_back(Idx + 2u);
			Indices.emplace_back(usDownCtrIdx);
			Indices.emplace_back((Idx + 2u) % Mod + 2u);	//��ʁi���j
			Indices.emplace_back(usUpCtrIdx);
			Indices.emplace_back(Idx + 3u);
			Indices.emplace_back((Idx + 3u) % Mod + 2u);	//��ʁi��j
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int DivNum = 24)	//�f�[�^�쐬�i���f���p�j
	{
		VS_DATA<V> vsd = MakeData<V>(DivNum);
		vsd.ResetDataForModel();
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//�v���g�^�C�v�錾
	explicit VSD_PRISM() noexcept {}

	~VSD_PRISM() noexcept {}
};
