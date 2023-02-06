/**
 * @file Light.h
 * @brief ���C�g�e�N���X
 * @author ���J�C�A��
 * @date 2022/08/20
 * @���� 2022/08/20�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <App.h>

//===== �O���錾 =====

#ifdef IMGUI

class MESH_LIGHT;

#endif // IMGUI

//===== �N���X��` =====

//***** ���C�g�e�N���X *****
class LIGHT
{
public:

	//���C�g���
	struct LIGHT_DIRECTIONAL		//���s����
	{
		DirectX::XMFLOAT4 Pos;		//���[���h���W
		DirectX::XMFLOAT4 Color_D;	//���̐F�i�g�U�F, w�͋��x�j

		explicit LIGHT_DIRECTIONAL() noexcept :
			Pos(0.0f, 1.0f, 0.0f, 1.0f), Color_D(0.0f, 0.0f, 0.0f, 0.0f)
		{}
		~LIGHT_DIRECTIONAL() noexcept
		{}
	};

	struct LIGHT_POINT				//�_����
	{
		DirectX::XMFLOAT4 Pos;		//���[���h���W
		DirectX::XMFLOAT4 Color_D;	//���̐F�i�g�U�F, w�͋��x�j

		float AttConst;
		float AttLinear;
		float AttQuadratic;			//���̌���
		float Pad;					//���u��

		explicit LIGHT_POINT() noexcept :
			Pos(0.0f, 0.0f, 0.0f, 1.0f), Color_D(0.0f, 0.0f, 0.0f, 0.0f),
			AttConst(1.0f), AttLinear(0.045f), AttQuadratic(0.0075f), Pad(1.0f)
		{}
		~LIGHT_POINT() noexcept
		{}
	};

	//�v���g�^�C�v�錾
	explicit LIGHT(APP& App) noexcept;
	virtual ~LIGHT() noexcept;
	virtual void Update() const noexcept = 0;	//�X�V����

protected:

	//�ϐ��錾

#ifdef IMGUI

	MESH_LIGHT& m_Mesh;			//�`�惁�b�V���Q��
	int m_MeshIndex;			//���b�V���C���f�b�N�X
	DirectX::XMFLOAT3 m_Scale;	//�T�C�Y

#endif // IMGUI

};
