/**
 * @file vsdGrid.h
 * @brief �`��e���v���[�g
 * @author ���J�C�A��
 * @date 2022/07/13
 * @���� 2022/07/13�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/VertexShaderData.h>
#include <Tool/gMath.h>
#include <array>

//===== �N���X��` =====

//***** �`�� *****
class VSD_GRID
{
public:

	template<class V>
	static VS_DATA<V> MakeData(int DivX = 1, int DivY = 1)	//�f�[�^�쐬
	{
		//�O����
		namespace dx = DirectX;
		if (DivX < 1)
			throw ERROR_EX2("�̉���������1�ȏ�ɂ��Ă��������B");
		if (DivY < 1)
			throw ERROR_EX2("�̏c��������1�ȏ�ɂ��Ă��������B");

		//�ϐ��錾
		constexpr float fWidth = 1.0f;
		constexpr float fHeight = 1.0f;		//�����E�c��
		const int nVtxNumX = DivX + 1;
		const int nVtxNumY = DivY + 1;		//���E�c���_��

		//���_�쐬
		std::vector<V> aData(static_cast<size_t>(nVtxNumX) * nVtxNumY);
		const dx::XMVECTOR vBottomLeft = dx::XMVectorSet(-0.5f, -0.5f, 0.0f, 0.0f);		//�̍������_
		const float fUnitSizeX = fWidth / DivX;
		const float fUnitSizeY = fHeight / DivY;										//�̕����P�ʂ��Ƃ̉����E�c��
		for (int y = 0, nIdxCnt = 0; y < nVtxNumY; y++) {
			const float PosY = y * fUnitSizeY;											//�s���Ƃ̏c��
			for (int x = 0; x < nVtxNumX; x++) {
				const dx::XMVECTOR vVertex = dx::XMVectorAdd(vBottomLeft, dx::XMVectorSet(x * fUnitSizeX, PosY, 0.0f, 0.0f));
				dx::XMStoreFloat3(&aData[nIdxCnt].m_Pos, vVertex);
				nIdxCnt++;																//�C���f�b�N�X�J�E���^
			}
		}

		//�C���f�b�N�X�Z�o�p�����_��
		const auto CalcIdx = [nVtxNumX](int x, int y)
		{ return static_cast<UINT>(y * nVtxNumX + x); };

		//�C���f�b�N�X�쐬
		std::vector<UINT> Indices;
		Indices.reserve(static_cast<size_t>(DivX) * DivY * 4 + (static_cast<size_t>(DivX) + DivY) * 2);	//�T�C�Y�w��
		for (int y = 0; y < DivY; y++) {
			for (int x = 0; x < DivX; x++) {

				//���j�b�g���Ƃ̒��_�C���f�b�N�X���Z�o
				const std::array<UINT, 3> aIndex = {
					CalcIdx(x, y),
					CalcIdx(x + 1, y),
					CalcIdx(x, y + 1),
				};

				//�C���f�b�N�X�i�[
				Indices.emplace_back(aIndex[0]);
				Indices.emplace_back(aIndex[1]);
				Indices.emplace_back(aIndex[0]);
				Indices.emplace_back(aIndex[2]);
			}
		}
		for (int x = 0, y = DivY; x < DivX; x++) {

			//�C���f�b�N�X�i�[�i��ԏ�̍s�j
			Indices.emplace_back(CalcIdx(x, y));
			Indices.emplace_back(CalcIdx(x + 1, y));
		}
		for (int x = DivX, y = 0; y < DivY; y++) {

			//�C���f�b�N�X�i�[�i��ԉE�̗�j
			Indices.emplace_back(CalcIdx(x, y));
			Indices.emplace_back(CalcIdx(x, y + 1));
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices), false);
	}

private:

	//�v���g�^�C�v�錾
	explicit VSD_GRID() noexcept {}

	~VSD_GRID() noexcept {}
};
