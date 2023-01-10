/**
 * @file vsdRef.h
 * @brief 頂点データまとめ
 * @author 室谷イアン
 * @date 2022/08/13
 * @履歴 2022/08/13：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/vsdBox.h>
#include <GraphicApp/Drawer/vsdCone.h>
#include <GraphicApp/Drawer/vsdPrism.h>
#include <GraphicApp/Drawer/vsdSphere.h>
#include <GraphicApp/Drawer/vsdPlane.h>

//===== 構造体宣言 =====
struct INSTANCE_DATA					//インスタンス情報
{
	DirectX::XMFLOAT4X4 MtxWorld;		//ワールド行列

	INSTANCE_DATA() noexcept : MtxWorld()
	{
		//ワールド行列初期化
		DirectX::XMStoreFloat4x4(&MtxWorld, DirectX::XMMatrixIdentity());
	}
	~INSTANCE_DATA() noexcept {}
};

//===== クラス定義 =====

//***** 頂点データ作成 *****
class VSD_MAKER
{
public:

	//形状タイプ
	enum class SHAPE
	{
		BOX,
		PYRAMID,
		CONE,
		PRISM,
		CYLINDER,
		SPHERE,
		PLANE,

		MAX_NUM
	};

	//プロトタイプ宣言
	static VS_DATA<VERTEX> MakeData_Default(SHAPE Type)		//デフォルトデータ
	{
		switch (Type)
		{
			case VSD_MAKER::SHAPE::BOX:
				return VSD_BOX::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::PYRAMID:
				return VSD_CONE::MakeData<VERTEX>(4);
			case VSD_MAKER::SHAPE::CONE:
				return VSD_CONE::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::PRISM:
				return VSD_PRISM::MakeData<VERTEX>(3);
			case VSD_MAKER::SHAPE::CYLINDER:
				return VSD_PRISM::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::SPHERE:
				return VSD_SPHERE::MakeData<VERTEX>();
			case VSD_MAKER::SHAPE::PLANE:
				return VSD_PLANE::MakeData<VERTEX>();
			default:
				throw ERROR_EX2("頂点データのタイプエラー。");
		}
	}

	static VS_DATA<VERTEX_T> MakeData_Tex(SHAPE Type)		//UV情報あり
	{
		switch (Type)
		{
			case VSD_MAKER::SHAPE::BOX:
				return VSD_BOX::MakeData_Tex<VERTEX_T>();
			case VSD_MAKER::SHAPE::PLANE:
				return VSD_PLANE::MakeData_Tex<VERTEX_T>();
			default:
				throw ERROR_EX2("頂点データのタイプエラー。");
		}
	}

	static VS_DATA<VERTEX_M> MakeData_Model(SHAPE Type)		//モデル用(データ作成)
	{
		switch (Type)
		{
			case VSD_MAKER::SHAPE::BOX:
				return VSD_BOX::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::PYRAMID:
				return VSD_CONE::MakeData_Model<VERTEX_M>(4);
			case VSD_MAKER::SHAPE::CONE:
				return VSD_CONE::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::PRISM:
				return VSD_PRISM::MakeData_Model<VERTEX_M>(3);
			case VSD_MAKER::SHAPE::CYLINDER:
				return VSD_PRISM::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::SPHERE:
				return VSD_SPHERE::MakeData_Model<VERTEX_M>();
			case VSD_MAKER::SHAPE::PLANE:
				return VSD_PLANE::MakeData_Model<VERTEX_M>();
			default:
				throw ERROR_EX2("頂点データのタイプエラー。");
		}
	}

private:

	//プロトタイプ宣言
	explicit VSD_MAKER() noexcept {}
	~VSD_MAKER() noexcept {}
};
