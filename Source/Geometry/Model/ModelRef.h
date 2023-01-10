/**
 * @file ModelRef.h
 * @brief モデル参照まとめ
 * @author 室谷イアン
 * @date 2022/09/15
 * @履歴 2022/09/15：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/vsdRef.h>
#include <Tool/TextureLoader.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== 構造体宣言 =====
namespace ModelRef {

	//アニメーションパック
	struct ANIM_PACK
	{
		std::vector<DirectX::XMFLOAT4X4> aMatrix;	//フレーム姿勢配列

		ANIM_PACK() noexcept : aMatrix(0)
		{}
		~ANIM_PACK() noexcept {}
	};

	//メッシュパック
	struct MESH_PACK
	{
		std::string Name;					//メッシュ名
		VS_DATA<VERTEX_MB> vsData;			//頂点情報
		CBD_MATERIAL MaterialData;			//マテリアル情報
		std::string Tex_D;					//テクスチャ：拡散反射光
		std::vector<ANIM_PACK> aNoSkin;		//骨なしメッシュ用アニメーション配列

		MESH_PACK() noexcept : Name(""), vsData(), MaterialData(), Tex_D(""), aNoSkin(0)
		{}
		~MESH_PACK() noexcept {}
	};

	//骨パック
	struct BONE_PACK
	{
		std::string BoneName;				//ボーン名
		DirectX::XMFLOAT4X4 InitMatrix;		//初期姿勢
		std::vector<ANIM_PACK> aBoneSkin;	//骨用アニメーション配列
		std::vector<ANIM_PACK> aSkin;		//アニメーション配列

		BONE_PACK() noexcept : BoneName(""), aBoneSkin(0), aSkin(0)
		{
			DirectX::XMStoreFloat4x4(&InitMatrix, DirectX::XMMatrixIdentity());
		}
		~BONE_PACK() noexcept {}
	};

	//テクスチャパック
	struct TEX_PACK
	{
		std::string Name;				//テクスチャ名
		TEX_LOADER::TEX_DATA TexData;	//テクスチャ情報

		TEX_PACK() noexcept : Name(""), TexData()
		{}
		~TEX_PACK() noexcept {}
	};

	//モデルパック
	struct MODEL_PACK
	{
		std::vector<MESH_PACK> aMesh;			//メッシュ配列
		std::vector<BONE_PACK> aBone;			//骨配列
		std::vector<int> aNoSkinIndex;			//骨なしメッシュ番号
		std::vector<std::string> aAnimName;		//アニメーション名
		std::vector<int> aAnimFrame;			//アニメーションフレーム数
		std::vector<bool> aIsFPS_30;			//アニメーションFPS数
		std::vector<TEX_PACK> m_aTexPack;		//テクスチャパック配列
		DirectX::XMFLOAT4X4 InitMtxWorld;		//初期ワールド行列

		MODEL_PACK() noexcept : aMesh(0), aBone(0), aNoSkinIndex(0), aAnimName(0), aAnimFrame(0), aIsFPS_30(false), m_aTexPack(0)
		{
			DirectX::XMStoreFloat4x4(&InitMtxWorld, DirectX::XMMatrixIdentity());
		}
		~MODEL_PACK() noexcept {}
	};
}
