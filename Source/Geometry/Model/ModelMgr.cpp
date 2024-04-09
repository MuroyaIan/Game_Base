
//===== インクルード部 =====
#include <Geometry/Model/ModelMgr.h>
#include <Geometry/Model/ModelRef.h>
#include <Tool/FileIO.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== 静的メンバ =====
std::string MODEL_MGR::aModelName[static_cast<int>(MODEL_ID::ID_Max)] = {
	"Tarantella",
	"UnityChan",
	"Cube"
};

std::vector<std::string> MODEL_MGR::aAnimName[static_cast<int>(MODEL_ID::ID_Max)] = {
	{"Idle", "Walk", "Run"},
	{"Jump", "Run"},
	{}
};

std::vector<bool> MODEL_MGR::aAnimFPS_30[static_cast<int>(MODEL_ID::ID_Max)] = {
	{false, false, false},
	{true, true},
	{}
};

//===== 構造体実装 =====
MODEL_MGR::TEX_PACK::TEX_PACK() noexcept : aName(0), aTexData(0), aTexBuffPtr(0), AnimBuffPtr(), aUsedCount(0)
{}

MODEL_MGR::TEX_PACK::~TEX_PACK() noexcept
{}

//===== クラス実装 =====
MODEL_MGR::MODEL_MGR(CT_GRAPHIC& Gfx) :
	m_aModelPackPtr(static_cast<int>(MODEL_ID::ID_Max)), m_aTexPack(static_cast<int>(MODEL_ID::ID_Max)), m_DX(Gfx)
{
	//モデルパック配列初期化
	for (auto& m : m_aModelPackPtr)
		m = std::make_unique<ModelRef::MODEL_PACK>();

	//ファイル読込
	if (m_aTexPack.size() > 0) {

		//読込ループ
		auto pModelName = &aModelName[0];
		for (int i = 0, iCnt = static_cast<int>(MODEL_ID::ID_Max); i < iCnt; i++) {

			//モデル読込
			LoadModel(static_cast<MODEL_ID>(i));

			//テクスチャ読込
			TEX_PACK& TexPack = m_aTexPack[i];
			std::string Path = "Asset/Model/";
			Path += *pModelName;
			Path += "/Tex/";
			std::ostringstream oss;
			for (auto& n : TexPack.aName) {
				oss << Path << n;
				TexPack.aTexData.push_back(TEX_LOADER::LoadTexture(oss.str().c_str()));
				TexPack.aTexBuffPtr.emplace_back(nullptr);
				TexPack.aUsedCount.emplace_back(0);
				oss.str("");
			}

			//ポインタ加算
			pModelName++;
		}
	}

	//ワールド行列初期化
	dx::XMStoreFloat4x4(&m_aModelPackPtr[static_cast<int>(MODEL_ID::Tarantella)]->InitMtxWorld,
		dx::XMMatrixScaling(0.1f, 0.1f, 0.1f)
		* dx::XMMatrixRotationRollPitchYaw(0.0f, gMath::GetRad(180), 0.0f)
		* dx::XMMatrixTranslation(0.0f, -10.0f, 0.0f)
	);
	dx::XMStoreFloat4x4(&m_aModelPackPtr[static_cast<int>(MODEL_ID::Unity)]->InitMtxWorld,
		dx::XMMatrixScaling(0.1f, 0.1f, 0.1f)
		* dx::XMMatrixRotationRollPitchYaw(0.0f, gMath::GetRad(180), 0.0f)
		* dx::XMMatrixTranslation(0.0f, -10.0f, 0.0f)
	);
	dx::XMStoreFloat4x4(&m_aModelPackPtr[static_cast<int>(MODEL_ID::Cube)]->InitMtxWorld,
		dx::XMMatrixScaling(0.5f, 0.5f, 0.5f)
		* dx::XMMatrixRotationRollPitchYaw(0.0f, gMath::GetRad(180), 0.0f)
		* dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
	);
}

MODEL_MGR::~MODEL_MGR() noexcept
{
	//テクスチャ解放
	if (m_aTexPack.size() > 0) {

		//解放ループ
		TEX_PACK* pPack = &m_aTexPack[0];
		for (int i = 0, iCnt = static_cast<int>(MODEL_ID::ID_Max); i < iCnt; i++) {

			//例外処理
			if (pPack->aTexData.size() == 0)
				continue;

			//モデルごとに
			TEX_LOADER::TEX_DATA* pTex = &pPack->aTexData[0];
			for (auto& p : pPack->aTexBuffPtr) {

				//バッファ解放
				if (p != nullptr)
					p.reset();

				//メモリ解放
				if (pTex != nullptr) {
					TEX_LOADER::ReleaseTexture(pTex->pImageData);
					pTex->pImageData = nullptr;
				}

				//ポインタ加算
				pTex++;
			}

			//ポインタ加算
			pPack++;
		}
	}
}

//テクスチャバッファ利用開始・終了
ID3D11ShaderResourceView* MODEL_MGR::SetTextureOn(MODEL_ID id, std::string TexName)
{
	TEX_PACK& TexPack = m_aTexPack[static_cast<int>(id)];
	for (size_t i = 0, Cnt = TexPack.aName.size(); i < Cnt; i++) {
		if (TexPack.aName[i] == TexName) {

			//参照数更新
			TexPack.aUsedCount[i]++;

			//バッファ未作成の場合⇒作成
			if (TexPack.aTexBuffPtr[i] == nullptr)
				TexPack.aTexBuffPtr[i] = std::make_unique<TEXTURE>(m_DX, TexPack.aTexData[i]);

			//該当するSRVポインタを返す
			return TexPack.aTexBuffPtr[i]->GetSrvPtr();
		}
	}

	//例外処理
	std::ostringstream oss;
	oss << "該当するテクスチャがありません : " << aModelName[static_cast<int>(id)] << "->" << TexName;
	throw ERROR_EX2(oss.str().c_str());
}

void MODEL_MGR::SetTextureOff(MODEL_ID id, std::string TexName)
{
	TEX_PACK& TexPack = m_aTexPack[static_cast<int>(id)];
	for (size_t i = 0, Cnt = TexPack.aName.size(); i < Cnt; i++) {
		if (TexPack.aName[i] == TexName) {

			//参照数更新
			TexPack.aUsedCount[i]--;
			if (TexPack.aUsedCount[i] < 0)
				TexPack.aUsedCount[i] = 0;

			//参照数無し⇒バッファ解放
			if (TexPack.aUsedCount[i] == 0 &&
				TexPack.aTexBuffPtr[i] != nullptr)
				TexPack.aTexBuffPtr[i].reset();

			return;
		}
	}

	//例外処理
	std::ostringstream oss;
	oss << "該当するテクスチャがありません : " << aModelName[static_cast<int>(id)] << "->" << TexName;
	throw ERROR_EX2(oss.str().c_str());
}

//アニメーションバッファ利用開始・終了
ID3D11ShaderResourceView* MODEL_MGR::SetAnimTexOn(MODEL_ID id)
{
	if (m_aTexPack[static_cast<int>(id)].AnimBuffPtr == nullptr) {

		//参照データを取得
		int ModelID = static_cast<int>(id);
		ModelRef::MODEL_PACK& Model = *m_aModelPackPtr[ModelID];

		//骨情報を読込
		std::vector<dx::XMFLOAT4X4> aMtxBone(0);
		for (auto& bd : Model.aBone) {
			for (auto& ad : bd.aSkin) {
				for (auto& md : ad.aMatrix)
					aMtxBone.push_back(md);
			}
		}

		//テクスチャ作成用データ
		TEX_DATA_ANIM AnimData;
		AnimData.pAnimData = &aMtxBone.data()->_11;
		for (auto& f : Model.aAnimFrame)
			AnimData.nWidth += f;
		AnimData.nHeight = static_cast<int>(Model.aBone.size());

		//バッファ作成
		m_aTexPack[static_cast<int>(id)].AnimBuffPtr = std::make_unique<TEXTURE_ANIM>(m_DX, AnimData);
	}

	//該当するSRVポインタを返す
	return m_aTexPack[static_cast<int>(id)].AnimBuffPtr->GetSrvPtr();
}

void MODEL_MGR::SetAnimTexOff(MODEL_ID id) noexcept
{
	//バッファ解放
	if (m_aTexPack[static_cast<int>(id)].AnimBuffPtr != nullptr)
		m_aTexPack[static_cast<int>(id)].AnimBuffPtr.reset();
}

//モデル読込
void MODEL_MGR::LoadModel(MODEL_ID id)
{
	int ModelID = static_cast<int>(id);
	ModelRef::MODEL_PACK& Model = *m_aModelPackPtr[ModelID];

	//ファイルパス（モデル）
	std::string Name = aModelName[ModelID];
	std::string Path = "Asset/Model/";
	Path += Name;
	Path += "/";
	Path += Name;
	std::ostringstream oss;

	//モデル情報読込
	MODEL_BIN ModelBin;
	oss << Path << ".bin";
	FILE_IO::LoadFile(oss.str().c_str(), &ModelBin);
	oss.str("");
	if (ModelBin.MeshNum == 0) {
		std::ostringstream ossError;
		ossError << Name << "：モデルのメッシュ数が0です。";
		throw ERROR_EX2(ossError.str().c_str());
	}
	Model.aMesh.resize(ModelBin.MeshNum);
	Model.aBone.resize(ModelBin.BoneNum);
	Model.aNoSkinIndex.resize(ModelBin.NoSkinNum);

	//メッシュ情報（リスト）読込
	std::vector<MESH_BIN> aMeshBin(ModelBin.MeshNum);
	oss << Path << "_Mesh.bin";
	FILE_IO::LoadFile(oss.str().c_str(), aMeshBin);
	oss.str("");
	MESH_BIN* pMeshBin = &aMeshBin[0];
	for (auto& m : Model.aMesh) {
		m.Name.resize(pMeshBin->NameSize);
		m.vsData.m_Indices.resize(pMeshBin->VertexNum);
		m.vsData.m_Vertices.resize(pMeshBin->VertexNum);
		m.Tex_D.resize(pMeshBin->NameSize_Diffuse);
		m.Tex_S.resize(pMeshBin->NameSize_Specular);
		m.Tex_N.resize(pMeshBin->NameSize_Normal);
		m.Tex_Disp.resize(pMeshBin->NameSize_Displacement);

		//ポインタ加算
		pMeshBin++;
	}

	//メッシュ情報（詳細）読込
	ModelRef::MESH_PACK* pMesh = &Model.aMesh[0];
	for (int i = 0; i < ModelBin.MeshNum; i++) {	//メッシュごと

		//ファイル読込（メッシュ名）
		oss << Path << "_Mesh" << i << "_Name.bin";
		FILE_IO::LoadFile(oss.str().c_str(), pMesh->Name);
		oss.str("");

		//ファイル読込（インデックス）
		oss << Path << "_Mesh" << i << "_Idx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), pMesh->vsData.m_Indices);
		oss.str("");

		//ファイル読込（頂点情報）
		oss << Path << "_Mesh" << i << "_Vtx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), pMesh->vsData.m_Vertices);
		oss.str("");

		//ファイル読込（マテリアル）
		oss << Path << "_Mesh" << i << "_Material.bin";
		FILE_IO::LoadFile(oss.str().c_str(), &pMesh->MaterialData);
		oss.str("");

		//ファイル読込（テクスチャD）
		oss << Path << "_Mesh" << i << "_TexD.bin";
		if (pMesh->Tex_D.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_D);
			LoadTextureName(pMesh->Tex_D, m_aTexPack[ModelID]);		//テクスチャ登録
		}
		oss.str("");

		//ファイル読込（テクスチャS）
		oss << Path << "_Mesh" << i << "_TexS.bin";
		if (pMesh->Tex_S.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_S);
			LoadTextureName(pMesh->Tex_S, m_aTexPack[ModelID]);		//テクスチャ登録
		}
		oss.str("");

		//ファイル読込（テクスチャN）
		oss << Path << "_Mesh" << i << "_TexN.bin";
		if (pMesh->Tex_N.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_N);
			LoadTextureName(pMesh->Tex_N, m_aTexPack[ModelID]);		//テクスチャ登録
		}
		oss.str("");

		//ファイル読込（テクスチャDisp）
		oss << Path << "_Mesh" << i << "_TexDisp.bin";
		if (pMesh->Tex_Disp.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_Disp);
			LoadTextureName(pMesh->Tex_Disp, m_aTexPack[ModelID]);	//テクスチャ登録
		}
		oss.str("");

		//ポインタ加算
		pMesh++;
	}

	//骨情報（リスト）読込
	std::vector<BONE_BIN> aBoneBin(ModelBin.BoneNum);
	oss << Path << "_Bone.bin";
	FILE_IO::LoadFile(oss.str().c_str(), aBoneBin);
	oss.str("");
	if (ModelBin.BoneNum > 0) {
		BONE_BIN* pBoneBin = &aBoneBin[0];
		for (auto& b : Model.aBone) {				//骨ごと
			b.BoneName.resize(pBoneBin->NameSize);
			pBoneBin++;
		}
	}

	//骨情報（詳細）読込
	if (ModelBin.BoneNum > 0) {
		ModelRef::BONE_PACK* pBone = &Model.aBone[0];
		for (int i = 0; i < ModelBin.BoneNum; i++) {

			//ファイル読込（骨名）
			oss << Path << "_Bone" << i << "_Name.bin";
			FILE_IO::LoadFile(oss.str().c_str(), pBone->BoneName);
			oss.str("");

			//ファイル読込（初期姿勢）
			oss << Path << "_Bone" << i << "_Mtx.bin";
			FILE_IO::LoadFile(oss.str().c_str(), &pBone->InitMatrix);
			oss.str("");

			//ポインタ加算
			pBone++;
		}
	}

	//骨なしメッシュ番号読込
	oss << Path << "_NoSkin.bin";
	FILE_IO::LoadFile(oss.str().c_str(), Model.aNoSkinIndex);
	oss.str("");

	//アニメーション読込
	for (size_t i = 0, Cnt = aAnimName[ModelID].size(); i < Cnt; i++) {

		//FPS情報書込み
		Model.aIsFPS_30.emplace_back(aAnimFPS_30[ModelID][i]);

		//ファイルパス
		Path = "Asset/Model/";
		Path += Name;
		Path += "/Anim/Anim_";
		Path += aAnimName[ModelID][i];
		Model.aAnimName.push_back(aAnimName[ModelID][i]);

		//情報読込（リスト）
		ANIM_BIN AnimBin;
		oss << Path << ".bin";
		FILE_IO::LoadFile(oss.str().c_str(), &AnimBin);
		oss.str("");
		Model.aAnimFrame.emplace_back(AnimBin.FrameNum);
		ModelRef::ANIM_PACK AnimPack;
		AnimPack.aMatrix.resize(AnimBin.FrameNum);
		for (auto& b : Model.aBone) {
			b.aBoneSkin.push_back(AnimPack);
			b.aSkin.push_back(AnimPack);
		}

		//情報読込（詳細）
		if (ModelBin.BoneNum > 0) {
			auto pBonePack = &Model.aBone[0];
			for (int j = 0; j < ModelBin.BoneNum; j++) {	//骨ごと

				//骨姿勢
				oss << Path << "_BoneMtx" << j << ".bin";
				FILE_IO::LoadFile(oss.str().c_str(), pBonePack->aBoneSkin[i].aMatrix);
				oss.str("");

				//フレーム姿勢
				oss << Path << "_FrameMtx" << j << ".bin";
				FILE_IO::LoadFile(oss.str().c_str(), pBonePack->aSkin[i].aMatrix);
				oss.str("");

				//ポインタ加算
				pBonePack++;
			}
		}

		//情報読込（骨なしメッシュ）
		if (ModelBin.NoSkinNum > 0) {
			auto pIdx = &Model.aNoSkinIndex[0];
			for (int j = 0; j < ModelBin.NoSkinNum; j++) {
				auto pMeshPack = &Model.aMesh[*pIdx];
				pMeshPack->aNoSkin.push_back(AnimPack);

				//フレーム姿勢
				oss << Path << "_NoSkinMtx" << j << ".bin";
				FILE_IO::LoadFile(oss.str().c_str(), pMeshPack->aNoSkin[i].aMatrix);
				oss.str("");

				//ポインタ加算
				pIdx++;
			}
		}
	}
}

//テクスチャ名読込
void MODEL_MGR::LoadTextureName(std::string TexName, TEX_PACK& DataRef) const noexcept
{
	//そもそもテクスチャ配列にデータがない
	if (DataRef.aName.size() < 1)
		DataRef.aName.push_back(TexName);
	else {
		auto pName = &DataRef.aName[0];
		for (size_t j = 0, Cnt = DataRef.aName.size(); j < Cnt; j++) {

			//同じテクスチャが存在する場合
			if (*pName == TexName)
				break;

			//テクスチャが存在しない場合
			if (j == Cnt - 1)
				DataRef.aName.push_back(TexName);

			//ポインタ加算
			pName++;
		}
	}
}
