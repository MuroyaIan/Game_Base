/**
 * @file vsdSphere.h
 * @brief ���̌`��e���v���[�g
 * @author ���J�C�A��
 * @date 2022/07/10
 * @���� 2022/07/10�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/VertexShaderData.h>
#include <Tool/gMath.h>

//===== �N���X��` =====

//***** ���̌`�� *****
class VSD_SPHERE
{
public:

	//�v���g�^�C�v�錾
	template<class V>
	static VS_DATA<V> MakeTessellation(int LatDiv, int LongDiv)		//�e�b�Z���[�V�����쐬
	{
		//�O����
		namespace dx = DirectX;
		if (LatDiv < 3)
			throw ERROR_EX2("�ܓx��������3�ȏ�ɂ��Ă��������B");
		if (LongDiv < 3)
			throw ERROR_EX2("�o�x��������3�ȏ�ɂ��Ă��������B");

		//�ϐ��錾
		constexpr float fRadius = 0.5f;											//���a
		const dx::XMVECTOR vBase = dx::XMVectorSet(0.0f, fRadius, 0.0f, 0.0f);	//��p���_�x�N�g��
		const float fLatAngle = fPI / LatDiv;									//�ܓx�p(180�ŕ���)
		const float fLongAngle = 2.0f * fPI / LongDiv;							//�o�x�p(360�ŕ���)

		//���_�쐬
		std::vector<V> aData;
		for (int iLat = 1; iLat < LatDiv; iLat++) {
			const dx::XMVECTOR vLatBase = dx::XMVector3Transform(vBase, dx::XMMatrixRotationX(fLatAngle * iLat));		//�ܓx�̕��������Ƃ�
			for (int iLong = 0; iLong < LongDiv; iLong++) {
				aData.emplace_back();
				dx::XMVECTOR vVertex = dx::XMVector3Transform(vLatBase, dx::XMMatrixRotationY(fLongAngle * iLong));		//�o�x�̕��������̒��_���쐬
				dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
			}
		}

		//�k�ɓ_���_�쐬
		const UINT usIdxNP = (UINT)aData.size();
		aData.emplace_back();
		dx::XMStoreFloat3(&aData.back().m_Pos, vBase);

		//��ɓ_���_�쐬
		const UINT usIdxSP = (UINT)aData.size();
		aData.emplace_back();
		dx::XMStoreFloat3(&aData.back().m_Pos, dx::XMVectorNegate(vBase));

		//�C���f�b�N�X�Z�o�p�����_��
		const auto CalcIdx = [LongDiv](UINT iLat, UINT iLong)
		{ return static_cast<UINT>(iLat * LongDiv + iLong); };

		//�C���f�b�N�X���X�g�쐬
		std::vector<UINT> Indices;
		const UINT Mod = static_cast<UINT>(LongDiv);
		for (UINT iLat = 0; iLat < static_cast<UINT>(LatDiv - 2); iLat++) {
			for (UINT iLong = 0; iLong < static_cast<UINT>(LongDiv); iLong++) {
				Indices.emplace_back(CalcIdx(iLat,		iLong));
				Indices.emplace_back(CalcIdx(iLat + 1u, iLong));
				Indices.emplace_back(CalcIdx(iLat,		(iLong + 1u) % Mod));
				Indices.emplace_back(CalcIdx(iLat,		(iLong + 1u) % Mod));
				Indices.emplace_back(CalcIdx(iLat + 1u, iLong));
				Indices.emplace_back(CalcIdx(iLat + 1u, (iLong + 1u) % Mod));
			}
		}

		//�ɓ_����̃C���f�b�N�X���X�g�쐬
		const UINT usLatDiv = static_cast<UINT>(LatDiv);
		for (UINT iLong = 0; iLong < static_cast<UINT>(LongDiv); iLong++) {

			//�k�ɓ_
			Indices.push_back(usIdxNP);
			Indices.push_back(CalcIdx(0u, iLong));
			Indices.push_back(CalcIdx(0u, (iLong + 1u) % Mod));

			//��ɓ_
			Indices.push_back(CalcIdx(usLatDiv - 2u, (iLong + 1u) % Mod));
			Indices.push_back(CalcIdx(usLatDiv - 2u, iLong));
			Indices.push_back(usIdxSP);
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData(int LatDiv = 12, int LongDiv = 12)	//�f�[�^�쐬
	{
		return MakeTessellation<V>(LatDiv, LongDiv);
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int LatDiv = 12, int LongDiv = 12)		//�f�[�^�쐬�i���f���p�j
	{
		VS_DATA<V> vsd = MakeTessellation<V>(LatDiv, LongDiv);
		vsd.ResetDataForModel();
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//�v���g�^�C�v�錾
	VSD_SPHERE() noexcept {}
	~VSD_SPHERE() noexcept {}
};
