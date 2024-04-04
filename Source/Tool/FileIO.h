/**
 * @file FileIO.h
 * @brief �t�@�C�����o��
 * @author ���J�C�A��
 * @date 2022/09/01
 * @���� 2022/09/01�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <fstream>						//�t�@�C�����o��
#include <sstream>
#include <WinApp/ErrorOutput_Win.h>

//===== �\���̐錾 =====
struct MODEL_BIN			//���f���o�C�i��(xxx.bin)
{
	int MeshNum;			//���b�V������(xxx_Mesh.bin)
	int BoneNum;			//������(xxxAnim.bin)
	int NoSkinNum;			//���Ȃ����b�V������(xxx_NoSkin.bin)

	explicit MODEL_BIN() noexcept : MeshNum(0), BoneNum(0), NoSkinNum(0)
	{}
	~MODEL_BIN() noexcept {}
};

struct MESH_BIN					//���b�V���o�C�i��(xxx_Mesh.bin)
{
	int NameSize;				//���O������̃T�C�Y
	int VertexNum;				//���_����(xxx_Mesh0_Data.bin)
	int NameSize_Diffuse;
	int NameSize_Specular;
	int NameSize_Normal;
	int NameSize_Displacement;	//�e�N�X�`�����̃T�C�Y

	explicit MESH_BIN() noexcept :
		NameSize(0), VertexNum(0),
		NameSize_Diffuse(0), NameSize_Specular(0), NameSize_Normal(0), NameSize_Displacement(0)
	{}
	~MESH_BIN() noexcept {}
};

struct BONE_BIN					//���o�C�i��(xxx_Bone.bin)
{
	int NameSize;				//���O������̃T�C�Y��(xxx_Bone0_Data.bin)

	explicit BONE_BIN() noexcept : NameSize(0)
	{}
	~BONE_BIN() noexcept {}
};

struct ANIM_BIN					//�A�j���[�V�����o�C�i��(Anim_xxx.bin)
{
	int FrameNum;				//�t���[������(Anim_xxx_Data.bin)

	explicit ANIM_BIN() noexcept : FrameNum(0)
	{}
	~ANIM_BIN() noexcept {}
};

//===== �N���X��` =====
class FILE_IO
{
public:

	//�v���g�^�C�v�錾
	template<typename T>
	static void SaveFile(const char* FilePath, T* Ptr)	//���o����
	{
		//�t�@�C�����J��
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::out);

		//��O����
		if (!File) {
			std::ostringstream oss;
			oss << "�t�@�C���i" << FilePath << "�j���J���܂���ł����B";
			throw ERROR_EX2(oss.str().c_str());
		}

		//�f�[�^���o
		File.write((char*)Ptr, sizeof(T));
		File.close();
	}

	template<typename T>
	static void SaveFile(const char* FilePath, std::vector<T>& Array)	//���o�����i�z��j
	{
		//�t�@�C�����J��
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::out);

		//��O����
		if (!File) {
			std::ostringstream oss;
			oss << "�t�@�C���i" << FilePath << "�j���J���܂���ł����B";
			throw ERROR_EX2(oss.str().c_str());
		}

		//�f�[�^���o
		File.write((char*)Array.data(), sizeof(T) * Array.size());
		File.close();
	}

	static void SaveFile(const char* FilePath, std::string& Text)	//���o�����i������j
	{
		//�t�@�C�����J��
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::out);

		//��O����
		if (!File) {
			std::ostringstream oss;
			oss << "�t�@�C���i" << FilePath << "�j���J���܂���ł����B";
			throw ERROR_EX2(oss.str().c_str());
		}

		//�f�[�^���o
		File.write((char*)Text.data(), Text.size());
		File.close();
	}

	template<typename T>
	static void LoadFile(const char* FilePath, T* Ptr)	//�Ǎ�����
	{
		//�t�@�C�����J��
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::in);

		//��O����
		if (!File) {
			std::ostringstream oss;
			oss << "�t�@�C���i" << FilePath << "�j���J���܂���ł����B";
			throw ERROR_EX2(oss.str().c_str());
		}

		//�f�[�^�Ǎ�
		File.read((char*)Ptr, sizeof(T));
		File.close();
	}

	template<typename T>
	static void LoadFile(const char* FilePath, std::vector<T>& Array)	//�Ǎ������i�z��j
	{
		//�t�@�C�����J��
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::in);

		//��O����
		if (!File) {
			std::ostringstream oss;
			oss << "�t�@�C���i" << FilePath << "�j���J���܂���ł����B";
			throw ERROR_EX2(oss.str().c_str());
		}

		//�f�[�^�Ǎ�
		File.read((char*)Array.data(), sizeof(T) * Array.size());
		File.close();
	}

	static void LoadFile(const char* FilePath, std::string& Text)	//�Ǎ������i������j
	{
		//�t�@�C�����J��
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::in);

		//��O����
		if (!File) {
			std::ostringstream oss;
			oss << "�t�@�C���i" << FilePath << "�j���J���܂���ł����B";
			throw ERROR_EX2(oss.str().c_str());
		}

		//�f�[�^�Ǎ�
		File.read((char*)Text.data(), Text.size());
		File.close();
	}

private:

	//�v���g�^�C�v�錾
	explicit FILE_IO() noexcept {}
	~FILE_IO() noexcept {}
};
