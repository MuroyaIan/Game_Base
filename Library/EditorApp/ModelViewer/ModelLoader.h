/**
 * @file ModelLoader.h
 * @brief FBXローダ
 * @author 室谷イアン
 * @date 2022/08/21
 * @履歴 2022/08/21：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <3rd_Party/FBXSDK/include/fbxsdk.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== クラス宣言 =====

//***** FBXローダ *****
class FBX_LOADER
{
public:

	//FBXデータ
	struct FBX_DATA
	{
		FbxNode* Node;									//ノード
		std::string NodeName;							//ノード名
		std::vector<FbxNodeAttribute*> m_pAttributes;	//属性
		std::vector<std::string> m_pAttributesType;		//属性タイプ名

		FBX_DATA() noexcept : Node(nullptr), NodeName(""), m_pAttributes(0), m_pAttributesType(0)
		{}
		~FBX_DATA() noexcept {}
	};

	//頂点インデックス
	struct VERTEX_INDEX
	{
		std::vector<int> aIndex;			//インデックス配列
		std::vector<int> aBoneID;			//骨番号の配列
		std::vector<float> aWeight;			//骨比重の配列

		VERTEX_INDEX() noexcept : aIndex(0), aBoneID(0), aWeight(0)
		{}
		~VERTEX_INDEX() noexcept {}
	};

	//レイヤテクスチャ情報
	struct LAYER_TEX_DATA
	{
		std::vector<std::string> aTex;	//テクスチャ名

		LAYER_TEX_DATA() noexcept : aTex(0)
		{}
		~LAYER_TEX_DATA() noexcept {}
	};

	//MESHデータ
	struct MESH_DATA
	{
		std::string NodeName;									//ノード名
		std::vector<VERTEX_INDEX> aIndexBuffer;					//頂点バッファインデックス
		VS_DATA<VERTEX_M> vsData;								//頂点情報
		std::string MaterialName;								//マテリアル名
		CBD_MATERIAL MaterialData;								//マテリアル情報
		bool IsPhong;											//Phongモデルフラグ
		std::vector<std::string> aTex_Ambient;					//テクスチャ：環境光
		std::vector<std::string> aTex_Diffuse;					//テクスチャ：拡散反射光
		std::vector<std::string> aTex_Emissive;					//テクスチャ：発射光
		std::vector<std::string> aTex_Transparent;				//テクスチャ：透過度
		std::vector<std::string> aTex_Specular;					//テクスチャ：鏡面反射光
		std::vector<std::string> aTex_Shininess;				//テクスチャ：光沢
		std::vector<std::string> aTex_Normal;					//テクスチャ：ノーマル
		std::vector<LAYER_TEX_DATA> aLayerTex_Ambient;
		std::vector<LAYER_TEX_DATA> aLayerTex_Diffuse;
		std::vector<LAYER_TEX_DATA> aLayerTex_Emissive;
		std::vector<LAYER_TEX_DATA> aLayerTex_Transparent;
		std::vector<LAYER_TEX_DATA> aLayerTex_Specular;
		std::vector<LAYER_TEX_DATA> aLayerTex_Shininess;
		std::vector<LAYER_TEX_DATA> aLayerTex_Normal;			//レイヤテクスチャ
		std::vector<int> aBoneID;								//メッシュに影響を与える骨の番号
		std::vector<FbxAMatrix> aReferenceGlobalInitPosition;	//姿勢計算用初期行列
		std::vector<FbxMatrix> aClusterRelativeInitPosition;	//姿勢計算用初期行列

		//骨なし姿勢行列
		struct NO_SKIN_MTX
		{
			std::vector<DirectX::XMFLOAT4X4> aMatrix;		//フレーム姿勢

			NO_SKIN_MTX() noexcept : aMatrix(0)
			{}
			~NO_SKIN_MTX() noexcept {}
		};
		std::vector<NO_SKIN_MTX> aNoSkinData;				//フレーム情報（骨なし）

		MESH_DATA() noexcept :
			NodeName(""), aIndexBuffer(0), vsData(), MaterialName(""), MaterialData(), IsPhong(true),
			aTex_Ambient(0), aTex_Diffuse(0), aTex_Emissive(0), aTex_Transparent(0), aTex_Specular(0), aTex_Shininess(0), aTex_Normal(0),
			aLayerTex_Ambient(0), aLayerTex_Diffuse(0), aLayerTex_Emissive(0), aLayerTex_Transparent(0), aLayerTex_Specular(0), aLayerTex_Shininess(0), aLayerTex_Normal(0),
			aBoneID(0), aReferenceGlobalInitPosition(0), aClusterRelativeInitPosition(0), aNoSkinData(0)
		{}
		~MESH_DATA() noexcept {}
	};

	//アニメーション情報
	struct ANIM_DATA
	{
		std::string TakeName;	//テイク名
		FbxTime Start;			//開始時間
		FbxTime Stop;			//終了時間
		int StartFrame;			//開始フレーム
		int StopFrame;			//終了フレーム
		bool bIsFPS_30;			//FPSは30

		ANIM_DATA() noexcept : TakeName(""), Start(), Stop(), StartFrame(0), StopFrame(0), bIsFPS_30(false)
		{}
		~ANIM_DATA() noexcept {}
	};

	//骨情報
	struct BONE_DATA
	{
		int Index;								//配列インデックス
		std::string BoneName;					//ボーン名
		DirectX::XMFLOAT4X4 InitMatrix;			//初期姿勢
		bool bMtxIsLoad;						//初期姿勢取得済み

		bool IsRoot;							//ルートボーンかどうか
		std::vector<BONE_DATA*> aChildBone;		//子ボーンの参照先

		BONE_DATA() noexcept : Index(0), BoneName(""), bMtxIsLoad(false), IsRoot(false), aChildBone(0)
		{
			DirectX::XMStoreFloat4x4(&InitMatrix, DirectX::XMMatrixIdentity());
		}
		~BONE_DATA() noexcept {}
	};

	//フレーム情報
	struct FRAME_DATA
	{
		std::vector<DirectX::XMFLOAT4X4> aMatrix;		//フレーム姿勢
		std::vector<DirectX::XMFLOAT4X4> aBoneMatrix;	//フレーム姿勢（骨）

		FRAME_DATA() noexcept : aMatrix(0), aBoneMatrix(0)
		{}
		~FRAME_DATA() noexcept {}
	};

	//スキン情報
	struct SKIN_DATA
	{
		std::vector<FRAME_DATA> aFrameData;		//フレーム情報

		SKIN_DATA() noexcept : aFrameData(0)
		{}
		~SKIN_DATA() noexcept {}
	};

	//プロトタイプ宣言
	FBX_LOADER() noexcept;
	~FBX_LOADER() noexcept;
	void Draw() const noexcept;										//描画処理
	void Load(const char* FilePath, bool bAnimOnly = false);
	void Load(const wchar_t* FilePath, bool bAnimOnly = false);		//ファイル読込
	void SaveModelData(const char* FileName) noexcept;				//ファイル書出し（モデル）
	void SaveAnimData(const char* FileName, int AnimID) noexcept;	//ファイル書出し（アニメーション）

	std::string& GetFilePath() noexcept					//ファイルパス参照
	{
		return m_FilePath;
	}
	MESH_DATA& GetMesh(int Index) noexcept				//メッシュ参照
	{
		return m_aMesh[Index];
	}
	int GetMeshCount() const noexcept					//メッシュの数取得
	{
		return static_cast<int>(m_aMesh.size());
	}
	std::vector<BONE_DATA>& GetSkeleton() noexcept		//骨参照
	{
		return m_aBone;
	}
	std::vector<ANIM_DATA>& GetAnimation() noexcept		//アニメーション参照
	{
		return m_aAnimation;
	}
	std::vector<SKIN_DATA>& GetSkin() noexcept			//スキン参照
	{
		return m_aSkin;
	}

private:

	//変数宣言
	FbxManager* m_pManager;								//マネージャ
	FbxIOSettings* m_pioSettings;						//入出力設定
	FbxImporter* m_pImporter;							//インポータ
	FbxScene* m_pScene;									//シーン
	FbxAnimEvaluator* m_pEvaluator;						//アニメーション評価
	FbxTime::EMode m_TimeMode;							//アニメーション時間設定
	FbxTime m_Period;									//フレーム時間
	int AnimLoadBaseIndex;								//アニメーション読込開始番号

	std::string m_FilePath;								//ファイルパス
	std::vector<FBX_DATA> m_aData;						//FBX情報
	std::vector<MESH_DATA> m_aMesh;						//メッシュ情報
	std::vector<ANIM_DATA> m_aAnimation;				//アニメーション情報
	std::vector<BONE_DATA> m_aBone;						//骨情報
	std::vector<SKIN_DATA> m_aSkin;						//スキン情報

	//プロトタイプ宣言
	void GetNodes(FbxNode* Node, int SpaceCnt, BONE_DATA* ParentBone = nullptr) noexcept;				//ノード取得
	void GetMesh(FbxNodeAttribute* MeshIn, std::string NodeName) noexcept;								//メッシュ取得
	void GetTexturePath(MESH_DATA& Mesh, FbxSurfaceMaterial* pMaterial, bool IsPhong = true) noexcept;	//テクスチャパス取得
	void GetTextureName(FbxSurfaceMaterial* pMaterial, const char* Type, std::vector<std::string>& Tex, std::vector<LAYER_TEX_DATA>& LayerTex) noexcept;	//テクスチャ名取得
	void GetSkinData(MESH_DATA& Mesh, FbxMesh* MeshIn) noexcept;										//フレーム情報読込
	void GetAnimationFromSkin(FbxMesh* MeshIn) noexcept;												//フレーム情報読込（アニメーション単体用）
	void GetBoneData() noexcept;																		//骨情報取得
	void GetAnimationData() noexcept;																	//アニメーション情報取得
	void LoadAnimation();																				//アニメーション読込
};
