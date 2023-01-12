/**
 * @file vsdBox.h
 * @brief �{�b�N�X�`��e���v���[�g
 * @author ���J�C�A��
 * @date 2022/07/07
 * @���� 2022/07/07�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== �N���X��` =====

//***** �{�b�N�X�`�� *****
class VSD_BOX
{
public:

	//�v���g�^�C�v�錾
	template<class V>
	static VS_DATA<V> MakeData()				//�f�[�^�쐬
	{
		//������`
		constexpr float Size = 1.0f * 0.5f;		//�{�b�N�X�̕ӂ̒���(�W��1.0f)

		//���_�쐬
		std::vector<V> aData(8);
		aData[0].m_Pos = { -Size, -Size, -Size };
		aData[1].m_Pos = {  Size, -Size, -Size };
		aData[2].m_Pos = { -Size,  Size, -Size };
		aData[3].m_Pos = {  Size,  Size, -Size };
		aData[4].m_Pos = { -Size, -Size,  Size };
		aData[5].m_Pos = {  Size, -Size,  Size };
		aData[6].m_Pos = { -Size,  Size,  Size };
		aData[7].m_Pos = {  Size,  Size,  Size };

		//�C���f�b�N�X�쐬
		const std::vector<UINT> Indices = {
			0, 2, 1,	2, 3, 1,
			1, 3, 5,	3, 7, 5,
			2, 6, 3,	3, 6, 7,
			4, 5, 7,	4, 7, 6,
			0, 4, 2,	2, 4, 6,
			0, 1, 4,	1, 5, 4
		};

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData_Tex()			//�f�[�^�쐬�i�e�N�X�`������j
	{
		//������`
		constexpr float Size = 1.0f * 0.5f;		//�{�b�N�X�̕ӂ̒���(�W��1.0f)

		//���_�쐬
		std::vector<V> aData(14);
		aData[0].m_Pos  = { -Size, -Size, -Size };
		aData[1].m_Pos  = {  Size, -Size, -Size };
		aData[2].m_Pos  = { -Size,  Size, -Size };
		aData[3].m_Pos  = {  Size,  Size, -Size };
		aData[4].m_Pos  = { -Size, -Size,  Size };
		aData[5].m_Pos  = {  Size, -Size,  Size };
		aData[6].m_Pos  = { -Size,  Size,  Size };
		aData[7].m_Pos  = {  Size,  Size,  Size };
		aData[8].m_Pos  = { -Size, -Size, -Size };
		aData[9].m_Pos  = {  Size, -Size, -Size };
		aData[10].m_Pos = { -Size, -Size, -Size };
		aData[11].m_Pos = { -Size, -Size,  Size };
		aData[12].m_Pos = {  Size, -Size, -Size };
		aData[13].m_Pos = {  Size, -Size,  Size };
		aData[0].m_UV  = { 2.0f / 3.0f, 0.0f / 4.0f };
		aData[1].m_UV  = { 1.0f / 3.0f, 0.0f / 4.0f };
		aData[2].m_UV  = { 2.0f / 3.0f, 1.0f / 4.0f };
		aData[3].m_UV  = { 1.0f / 3.0f, 1.0f / 4.0f };
		aData[4].m_UV  = { 2.0f / 3.0f, 3.0f / 4.0f };
		aData[5].m_UV  = { 1.0f / 3.0f, 3.0f / 4.0f };
		aData[6].m_UV  = { 2.0f / 3.0f, 2.0f / 4.0f };
		aData[7].m_UV  = { 1.0f / 3.0f, 2.0f / 4.0f };
		aData[8].m_UV  = { 2.0f / 3.0f, 4.0f / 4.0f };
		aData[9].m_UV  = { 1.0f / 3.0f, 4.0f / 4.0f };
		aData[10].m_UV = { 3.0f / 3.0f, 1.0f / 4.0f };
		aData[11].m_UV = { 3.0f / 3.0f, 2.0f / 4.0f };
		aData[12].m_UV = { 0.0f / 3.0f, 1.0f / 4.0f };
		aData[13].m_UV = { 0.0f / 3.0f, 2.0f / 4.0f };

		//�C���f�b�N�X�쐬
		const std::vector<UINT> Indices = {
			 0,  2,  1,		 2,  3,  1,
			 4,  8,  5,		 5,  8,  9,
			 2,  6,  3,		 3,  6,  7,
			 4,  5,  7,		 4,  7,  6,
			 2, 10, 11,		 2, 11,  6,
			12,  3,  7,		12,  7, 13
		};

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData_Model()			//�f�[�^�쐬�i���f���p�j
	{
		//������`
		constexpr float Size = 1.0f * 0.5f;		//�{�b�N�X�̕ӂ̒���(�W��1.0f)

		//���_�쐬
		std::vector<V> aData(24);
		aData[0].m_Pos  = { -Size, -Size, -Size };
		aData[1].m_Pos  = {  Size, -Size, -Size };
		aData[2].m_Pos  = { -Size,  Size, -Size };
		aData[3].m_Pos  = {  Size,  Size, -Size };		//near
		aData[4].m_Pos  = {  Size, -Size,  Size };
		aData[5].m_Pos  = { -Size, -Size,  Size };
		aData[6].m_Pos  = {  Size,  Size,  Size };
		aData[7].m_Pos  = { -Size,  Size,  Size };		//far
		aData[8].m_Pos  = { -Size, -Size,  Size };
		aData[9].m_Pos  = { -Size, -Size, -Size };
		aData[10].m_Pos = { -Size,  Size,  Size };
		aData[11].m_Pos = { -Size,  Size, -Size };		//left
		aData[12].m_Pos = {  Size, -Size, -Size };
		aData[13].m_Pos = {  Size, -Size,  Size };
		aData[14].m_Pos = {  Size,  Size, -Size };
		aData[15].m_Pos = {  Size,  Size,  Size };		//right
		aData[16].m_Pos = { -Size, -Size,  Size };
		aData[17].m_Pos = {  Size, -Size,  Size };
		aData[18].m_Pos = { -Size, -Size, -Size };
		aData[19].m_Pos = {  Size, -Size, -Size };		//bottom
		aData[20].m_Pos = { -Size,  Size, -Size };
		aData[21].m_Pos = {  Size,  Size, -Size };
		aData[22].m_Pos = { -Size,  Size,  Size };
		aData[23].m_Pos = {  Size,  Size,  Size };		//top
		aData[0].m_UV  = { 1.0f / 3.0f, 1.0f / 4.0f };
		aData[1].m_UV  = { 2.0f / 3.0f, 1.0f / 4.0f };
		aData[2].m_UV  = { 1.0f / 3.0f, 0.0f / 4.0f };
		aData[3].m_UV  = { 2.0f / 3.0f, 0.0f / 4.0f };	//near
		aData[4].m_UV  = { 1.0f / 3.0f, 3.0f / 4.0f };
		aData[5].m_UV  = { 2.0f / 3.0f, 3.0f / 4.0f };
		aData[6].m_UV  = { 1.0f / 3.0f, 2.0f / 4.0f };
		aData[7].m_UV  = { 2.0f / 3.0f, 2.0f / 4.0f };	//far
		aData[8].m_UV  = { 2.0f / 3.0f, 2.0f / 4.0f };
		aData[9].m_UV  = { 3.0f / 3.0f, 2.0f / 4.0f };
		aData[10].m_UV = { 2.0f / 3.0f, 1.0f / 4.0f };
		aData[11].m_UV = { 3.0f / 3.0f, 1.0f / 4.0f };	//left
		aData[12].m_UV = { 0.0f / 3.0f, 2.0f / 4.0f };
		aData[13].m_UV = { 1.0f / 3.0f, 2.0f / 4.0f };
		aData[14].m_UV = { 0.0f / 3.0f, 1.0f / 4.0f };
		aData[15].m_UV = { 1.0f / 3.0f, 1.0f / 4.0f };	//right
		aData[16].m_UV = { 1.0f / 3.0f, 4.0f / 4.0f };
		aData[17].m_UV = { 2.0f / 3.0f, 4.0f / 4.0f };
		aData[18].m_UV = { 1.0f / 3.0f, 3.0f / 4.0f };
		aData[19].m_UV = { 2.0f / 3.0f, 3.0f / 4.0f };	//bottom
		aData[20].m_UV = { 1.0f / 3.0f, 2.0f / 4.0f };
		aData[21].m_UV = { 2.0f / 3.0f, 2.0f / 4.0f };
		aData[22].m_UV = { 1.0f / 3.0f, 1.0f / 4.0f };
		aData[23].m_UV = { 2.0f / 3.0f, 1.0f / 4.0f };	//top

		//�C���f�b�N�X�쐬
		const std::vector<UINT> Indices = {
			 0,  2,  1,		 2,  3,  1,
			 4,  6,  5,		 6,  7,  5,
			 8, 10,  9,		10, 11,  9,
			12, 14, 13,		14, 15, 13,
			16, 18, 17,		18, 19, 17,
			20, 22, 21,		22, 23, 21
		};

		VS_DATA<V> vsd(std::move(aData), std::move(Indices));
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//�v���g�^�C�v�錾
	explicit VSD_BOX() noexcept {}

	~VSD_BOX() noexcept {}
};
