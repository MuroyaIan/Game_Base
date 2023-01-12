/**
 * @file vsdRef.h
 * @brief ���_�f�[�^�܂Ƃ�
 * @author ���J�C�A��
 * @date 2022/08/13
 * @���� 2022/08/13�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/vsdBox.h>
#include <GraphicApp/Drawer/vsdCone.h>
#include <GraphicApp/Drawer/vsdPrism.h>
#include <GraphicApp/Drawer/vsdSphere.h>
#include <GraphicApp/Drawer/vsdPlane.h>

//===== �\���̐錾 =====
struct VSD_INSTANCE						//�C���X�^���X���
{
	DirectX::XMFLOAT4X4 MtxWorld;		//���[���h�s��

	VSD_INSTANCE() noexcept : MtxWorld()
	{
		//���[���h�s�񏉊���
		DirectX::XMStoreFloat4x4(&MtxWorld, DirectX::XMMatrixIdentity());
	}
	~VSD_INSTANCE() noexcept {}
};

//===== �N���X��` =====

//***** ���_�f�[�^�쐬 *****
class VSD_MAKER
{
public:

	//�`��^�C�v
	enum class SHAPE
	{
		Box,
		Pyramid,
		Cone,
		Prism,
		Cylinder,
		Sphere,
		Plane,

		MaxNum
	};

	//�v���g�^�C�v�錾
	static VS_DATA<VERTEX> MakeData_Default(SHAPE Type)		//�f�t�H���g�f�[�^
	{
		switch (Type) {

			//��������
			case VSD_MAKER::SHAPE::Box:
				return VSD_BOX::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::Pyramid:
				return VSD_CONE::MakeData<VERTEX>(4);
			case VSD_MAKER::SHAPE::Cone:
				return VSD_CONE::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::Prism:
				return VSD_PRISM::MakeData<VERTEX>(3);
			case VSD_MAKER::SHAPE::Cylinder:
				return VSD_PRISM::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::Sphere:
				return VSD_SPHERE::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::Plane:
				return VSD_PLANE::MakeData<VERTEX>();
			default:
				throw ERROR_EX2("���_�f�[�^�쐬�G���[�F�^�C�v�����݂��܂���B");
		}
	}

	static VS_DATA<VERTEX_T> MakeData_Tex(SHAPE Type)		//UV��񂠂�
	{
		switch (Type) {

			//��������
			case VSD_MAKER::SHAPE::Box:
				return VSD_BOX::MakeData_Tex<VERTEX_T>();
			case VSD_MAKER::SHAPE::Plane:
				return VSD_PLANE::MakeData_Tex<VERTEX_T>();
			default:
				throw ERROR_EX2("���_�f�[�^�쐬�G���[�F�^�C�v�����݂��܂���B");
		}
	}

	static VS_DATA<VERTEX_M> MakeData_Model(SHAPE Type)		//���f���p(�f�[�^�쐬)
	{
		switch (Type) {

			//��������
			case VSD_MAKER::SHAPE::Box:
				return VSD_BOX::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::Pyramid:
				return VSD_CONE::MakeData_Model<VERTEX_M>(4);
			case VSD_MAKER::SHAPE::Cone:
				return VSD_CONE::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::Prism:
				return VSD_PRISM::MakeData_Model<VERTEX_M>(3);
			case VSD_MAKER::SHAPE::Cylinder:
				return VSD_PRISM::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::Sphere:
				return VSD_SPHERE::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::Plane:
				return VSD_PLANE::MakeData_Model<VERTEX_M>();
			default:
				throw ERROR_EX2("���_�f�[�^�쐬�G���[�F�^�C�v�����݂��܂���B");
		}
	}

private:

	//�v���g�^�C�v�錾
	explicit VSD_MAKER() noexcept {}
	~VSD_MAKER() noexcept {}
};
