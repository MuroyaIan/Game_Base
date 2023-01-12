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
struct VSD_INSTANCE						//インスタンス情報
{
	DirectX::XMFLOAT4X4 MtxWorld;		//ワールド行列

	VSD_INSTANCE() noexcept : MtxWorld()
	{
		//ワールド行列初期化
		DirectX::XMStoreFloat4x4(&MtxWorld, DirectX::XMMatrixIdentity());
	}
	~VSD_INSTANCE() noexcept {}
};

//===== クラス定義 =====

//***** 頂点データ作成 *****
class VSD_MAKER
{
public:

	//形状タイプ
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

	//プロトタイプ宣言
	static VS_DATA<VERTEX> MakeData_Default(SHAPE Type)		//デフォルトデータ
	{
		switch (Type) {

			//生成処理
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
				throw ERROR_EX2("頂点データ作成エラー：タイプが存在しません。");
		}
	}

	static VS_DATA<VERTEX_T> MakeData_Tex(SHAPE Type)		//UV情報あり
	{
		switch (Type) {

			//生成処理
			case VSD_MAKER::SHAPE::Box:
				return VSD_BOX::MakeData_Tex<VERTEX_T>();
			case VSD_MAKER::SHAPE::Plane:
				return VSD_PLANE::MakeData_Tex<VERTEX_T>();
			default:
				throw ERROR_EX2("頂点データ作成エラー：タイプが存在しません。");
		}
	}

	static VS_DATA<VERTEX_M> MakeData_Model(SHAPE Type)		//モデル用(データ作成)
	{
		switch (Type) {

			//生成処理
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
				throw ERROR_EX2("頂点データ作成エラー：タイプが存在しません。");
		}
	}

private:

	//プロトタイプ宣言
	explicit VSD_MAKER() noexcept {}
	~VSD_MAKER() noexcept {}
};
