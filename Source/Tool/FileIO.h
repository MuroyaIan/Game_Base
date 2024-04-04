/**
 * @file FileIO.h
 * @brief ファイル入出力
 * @author 室谷イアン
 * @date 2022/09/01
 * @履歴 2022/09/01：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <fstream>						//ファイル入出力
#include <sstream>
#include <WinApp/ErrorOutput_Win.h>

//===== 構造体宣言 =====
struct MODEL_BIN			//モデルバイナリ(xxx.bin)
{
	int MeshNum;			//メッシュ数⇒(xxx_Mesh.bin)
	int BoneNum;			//骨数⇒(xxxAnim.bin)
	int NoSkinNum;			//骨なしメッシュ数⇒(xxx_NoSkin.bin)

	explicit MODEL_BIN() noexcept : MeshNum(0), BoneNum(0), NoSkinNum(0)
	{}
	~MODEL_BIN() noexcept {}
};

struct MESH_BIN					//メッシュバイナリ(xxx_Mesh.bin)
{
	int NameSize;				//名前文字列のサイズ
	int VertexNum;				//頂点数⇒(xxx_Mesh0_Data.bin)
	int NameSize_Diffuse;
	int NameSize_Specular;
	int NameSize_Normal;
	int NameSize_Displacement;	//テクスチャ名のサイズ

	explicit MESH_BIN() noexcept :
		NameSize(0), VertexNum(0),
		NameSize_Diffuse(0), NameSize_Specular(0), NameSize_Normal(0), NameSize_Displacement(0)
	{}
	~MESH_BIN() noexcept {}
};

struct BONE_BIN					//骨バイナリ(xxx_Bone.bin)
{
	int NameSize;				//名前文字列のサイズ⇒(xxx_Bone0_Data.bin)

	explicit BONE_BIN() noexcept : NameSize(0)
	{}
	~BONE_BIN() noexcept {}
};

struct ANIM_BIN					//アニメーションバイナリ(Anim_xxx.bin)
{
	int FrameNum;				//フレーム数⇒(Anim_xxx_Data.bin)

	explicit ANIM_BIN() noexcept : FrameNum(0)
	{}
	~ANIM_BIN() noexcept {}
};

//===== クラス定義 =====
class FILE_IO
{
public:

	//プロトタイプ宣言
	template<typename T>
	static void SaveFile(const char* FilePath, T* Ptr)	//書出処理
	{
		//ファイルを開く
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::out);

		//例外処理
		if (!File) {
			std::ostringstream oss;
			oss << "ファイル（" << FilePath << "）が開けませんでした。";
			throw ERROR_EX2(oss.str().c_str());
		}

		//データ書出
		File.write((char*)Ptr, sizeof(T));
		File.close();
	}

	template<typename T>
	static void SaveFile(const char* FilePath, std::vector<T>& Array)	//書出処理（配列）
	{
		//ファイルを開く
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::out);

		//例外処理
		if (!File) {
			std::ostringstream oss;
			oss << "ファイル（" << FilePath << "）が開けませんでした。";
			throw ERROR_EX2(oss.str().c_str());
		}

		//データ書出
		File.write((char*)Array.data(), sizeof(T) * Array.size());
		File.close();
	}

	static void SaveFile(const char* FilePath, std::string& Text)	//書出処理（文字列）
	{
		//ファイルを開く
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::out);

		//例外処理
		if (!File) {
			std::ostringstream oss;
			oss << "ファイル（" << FilePath << "）が開けませんでした。";
			throw ERROR_EX2(oss.str().c_str());
		}

		//データ書出
		File.write((char*)Text.data(), Text.size());
		File.close();
	}

	template<typename T>
	static void LoadFile(const char* FilePath, T* Ptr)	//読込処理
	{
		//ファイルを開く
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::in);

		//例外処理
		if (!File) {
			std::ostringstream oss;
			oss << "ファイル（" << FilePath << "）が開けませんでした。";
			throw ERROR_EX2(oss.str().c_str());
		}

		//データ読込
		File.read((char*)Ptr, sizeof(T));
		File.close();
	}

	template<typename T>
	static void LoadFile(const char* FilePath, std::vector<T>& Array)	//読込処理（配列）
	{
		//ファイルを開く
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::in);

		//例外処理
		if (!File) {
			std::ostringstream oss;
			oss << "ファイル（" << FilePath << "）が開けませんでした。";
			throw ERROR_EX2(oss.str().c_str());
		}

		//データ読込
		File.read((char*)Array.data(), sizeof(T) * Array.size());
		File.close();
	}

	static void LoadFile(const char* FilePath, std::string& Text)	//読込処理（文字列）
	{
		//ファイルを開く
		std::fstream File;
		File.open(FilePath, std::ios::binary | std::ios::in);

		//例外処理
		if (!File) {
			std::ostringstream oss;
			oss << "ファイル（" << FilePath << "）が開けませんでした。";
			throw ERROR_EX2(oss.str().c_str());
		}

		//データ読込
		File.read((char*)Text.data(), Text.size());
		File.close();
	}

private:

	//プロトタイプ宣言
	explicit FILE_IO() noexcept {}
	~FILE_IO() noexcept {}
};
