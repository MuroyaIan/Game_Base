
//===== インクルード部 =====
#include <EditorApp/ModelViewer/ModelLoader.h>
#include <Tool/imguiMgr.h>
#include <Tool/FileIO.h>
#include <Tool/TextIO.h>
#include <Tool/gMath.h>

//===== 追加ライブラリ =====
#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")
#pragma comment(lib, "zlib-mt.lib")

namespace dx = DirectX;

//===== クラス実装 =====
FBX_LOADER::FBX_LOADER() noexcept :
	m_pManager(nullptr), m_pioSettings(nullptr), m_pImporter(nullptr), m_pScene(nullptr), m_pEvaluator(nullptr), m_TimeMode(FbxTime::eDefaultMode), m_Period(), AnimLoadBaseIndex(0),
	m_FilePath(""), m_aData(0), m_aMesh(0), m_aAnimation(0), m_aBone(0), m_aSkin(0)
{
	//マネージャ作成
	m_pManager = FbxManager::Create();

	//入出力設定作成
	m_pioSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(m_pioSettings);
}

FBX_LOADER::~FBX_LOADER() noexcept
{
	//シーン解放
	if (m_pScene != nullptr)
		m_pScene->Destroy();

	//マネージャ解放
	m_pManager->Destroy();
}

//描画処理
void FBX_LOADER::Draw() const noexcept
{
	//ローダ用UI
	if (ImGui::Begin("FBX Loader")){

		//ノード情報
		if (ImGui::TreeNode("Nodes")) {

			//ノード一覧
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar);

			//ノード名と属性名
			for (auto& d : m_aData) {
				ImGui::Text("%s (", d.NodeName.c_str()); ImGui::SameLine();
				for (size_t i = 0, Cnt = d.m_pAttributesType.size(); i < Cnt; i++) {
					ImGui::Text("%s", d.m_pAttributesType[i].c_str()); ImGui::SameLine();
					if (i < d.m_pAttributesType.size() - 1) {
						ImGui::Text(", "); ImGui::SameLine();
					}
				}
				ImGui::Text(")");
			}

			ImGui::EndChild();
			ImGui::TreePop();
		}

		//メッシュ情報
		if (ImGui::TreeNode("Meshes")) {

			//メッシュ一覧
			if (m_aMesh.size() > 0) {
				ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "MeshNum"); ImGui::SameLine(); ImGui::Text(": %d", m_aMesh.size());
			}
			for (size_t i = 0, Cnt = m_aMesh.size(); i < Cnt; i++) {

				//情報出力
				ImGui::Text(U8(u8"　")); ImGui::SameLine(); ImGui::Text("Mesh : %s", m_aMesh[i].NodeName.c_str());

				//ポリゴン数、頂点数、インデックス数
				ImGui::Text("Polygons : %d", m_aMesh[i].vsData.m_Vertices.size() / 3);
				ImGui::Text("Vertices : %d", m_aMesh[i].vsData.m_Vertices.size());
				ImGui::Text("Indices : %d", m_aMesh[i].vsData.m_Indices.size());
				ImGui::Text("Material : %s", m_aMesh[i].MaterialName.c_str()); ImGui::SameLine();
				if (m_aMesh[i].IsPhong)
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.5f, 1.0f }, "(Phong)");
				else
					ImGui::TextColored(ImVec4{ 1.0f, 0.5f, 0.5f, 1.0f }, "(NotPhong)");

				//テクスチャ情報
				std::string TexName = "";
				TexName += m_aMesh[i].MaterialName;
				TexName += "'s Textures";
				if (ImGui::TreeNode(TexName.c_str())) {

					//環境光
					if (m_aMesh[i].aTex_Ambient.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aTex_Ambient.size(); j < jCnt; j++) {
							ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "Ambient"); ImGui::SameLine();
							ImGui::Text("%d : %s", j + 1, m_aMesh[i].aTex_Ambient[j].c_str());
						}
					}
					else if (m_aMesh[i].aLayerTex_Ambient.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aLayerTex_Ambient.size(); j < jCnt; j++) {
							ImGui::Text("Layer %d", j + 1);
							for (size_t k = 0, kCnt = m_aMesh[i].aLayerTex_Ambient[j].aTex.size(); k < kCnt; k++) {
								ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "Ambient"); ImGui::SameLine();
								ImGui::Text("%d : %s", k + 1, m_aMesh[i].aLayerTex_Ambient[j].aTex[k].c_str());
							}
						}
					}

					//拡散反射光
					if (m_aMesh[i].aTex_Diffuse.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aTex_Diffuse.size(); j < jCnt; j++) {
							ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "Diffuse"); ImGui::SameLine();
							ImGui::Text("%d : %s", j + 1, m_aMesh[i].aTex_Diffuse[j].c_str());
						}
					}
					else if (m_aMesh[i].aLayerTex_Diffuse.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aLayerTex_Diffuse.size(); j < jCnt; j++) {
							ImGui::Text("Layer %d", j + 1);
							for (size_t k = 0, kCnt = m_aMesh[i].aLayerTex_Diffuse[j].aTex.size(); k < kCnt; k++) {
								ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "Diffuse"); ImGui::SameLine();
								ImGui::Text("%d : %s", k + 1, m_aMesh[i].aLayerTex_Diffuse[j].aTex[k].c_str());
							}
						}
					}

					//発射光
					if (m_aMesh[i].aTex_Emissive.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aTex_Emissive.size(); j < jCnt; j++) {
							ImGui::TextColored(ImVec4{ 0.0f, 0.0f, 1.0f, 1.0f }, "Emissive"); ImGui::SameLine();
							ImGui::Text("%d : %s", j + 1, m_aMesh[i].aTex_Emissive[j].c_str());
						}
					}
					else if (m_aMesh[i].aLayerTex_Emissive.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aLayerTex_Emissive.size(); j < jCnt; j++) {
							ImGui::Text("Layer %d", j + 1);
							for (size_t k = 0, kCnt = m_aMesh[i].aLayerTex_Emissive[j].aTex.size(); k < kCnt; k++) {
								ImGui::TextColored(ImVec4{ 0.0f, 0.0f, 1.0f, 1.0f }, "Emissive"); ImGui::SameLine();
								ImGui::Text("%d : %s", k + 1, m_aMesh[i].aLayerTex_Emissive[j].aTex[k].c_str());
							}
						}
					}

					//透過度
					if (m_aMesh[i].aTex_Transparent.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aTex_Transparent.size(); j < jCnt; j++) {
							ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "Transparent"); ImGui::SameLine();
							ImGui::Text("%d : %s", j + 1, m_aMesh[i].aTex_Transparent[j].c_str());
						}
					}
					else if (m_aMesh[i].aLayerTex_Transparent.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aLayerTex_Transparent.size(); j < jCnt; j++) {
							ImGui::Text("Layer %d", j + 1);
							for (size_t k = 0, kCnt = m_aMesh[i].aLayerTex_Transparent[j].aTex.size(); k < kCnt; k++) {
								ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "Transparent"); ImGui::SameLine();
								ImGui::Text("%d : %s", k + 1, m_aMesh[i].aLayerTex_Transparent[j].aTex[k].c_str());
							}
						}
					}

					//鏡面反射光
					if (m_aMesh[i].aTex_Specular.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aTex_Specular.size(); j < jCnt; j++) {
							ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 1.0f, 1.0f }, "Specular"); ImGui::SameLine();
							ImGui::Text("%d : %s", j + 1, m_aMesh[i].aTex_Specular[j].c_str());
						}
					}
					else if (m_aMesh[i].aLayerTex_Specular.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aLayerTex_Specular.size(); j < jCnt; j++) {
							ImGui::Text("Layer %d", j + 1);
							for (size_t k = 0, kCnt = m_aMesh[i].aLayerTex_Specular[j].aTex.size(); k < kCnt; k++) {
								ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 1.0f, 1.0f }, "Specular"); ImGui::SameLine();
								ImGui::Text("%d : %s", k + 1, m_aMesh[i].aLayerTex_Specular[j].aTex[k].c_str());
							}
						}
					}

					//光沢
					if (m_aMesh[i].aTex_Shininess.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aTex_Shininess.size(); j < jCnt; j++) {
							ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 1.0f, 1.0f }, "Shininess"); ImGui::SameLine();
							ImGui::Text("%d : %s", j + 1, m_aMesh[i].aTex_Shininess[j].c_str());
						}
					}
					else if (m_aMesh[i].aLayerTex_Shininess.size() > 0) {
						for (size_t j = 0, jCnt = m_aMesh[i].aLayerTex_Shininess.size(); j < jCnt; j++) {
							ImGui::Text("Layer %d", j + 1);
							for (size_t k = 0, kCnt = m_aMesh[i].aLayerTex_Shininess[j].aTex.size(); k < kCnt; k++) {
								ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 1.0f, 1.0f }, "Shininess"); ImGui::SameLine();
								ImGui::Text("%d : %s", k + 1, m_aMesh[i].aLayerTex_Shininess[j].aTex[k].c_str());
							}
						}
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		//アニメーション情報
		if (ImGui::TreeNode("Animations")) {

			//アニメーション一覧
			for (size_t i = 0, Cnt = m_aAnimation.size(); i < Cnt; i++) {

				//情報出力
				ImGui::Text(U8(u8"　")); ImGui::SameLine(); ImGui::Text("Animation %d : %s", i + 1, m_aAnimation[i].TakeName.c_str());
				ImGui::Text("FrameNum : %d", m_aAnimation[i].StopFrame - m_aAnimation[i].StartFrame);
			}
			ImGui::TreePop();
		}

		//骨情報
		if (ImGui::TreeNode("Bones")) {

			//骨一覧
			for (size_t i = 0, Cnt = m_aSkin.size(); i < Cnt; i++) {

				//スキンごとに表示
				std::ostringstream oss;
				oss << "Skin " << (i + 1);
				if (ImGui::TreeNode(oss.str().c_str())) {

					//情報出力
					auto& aBones = m_aBone;
					int BoneNum = static_cast<int>(aBones.size());
					ImGui::Text(U8(u8"　")); ImGui::SameLine(); ImGui::Text("BoneNum : %d", BoneNum);

					for (size_t j = 0; j < BoneNum; j++)
						ImGui::Text("Bone[%d] : %s", j, aBones[j].BoneName.c_str());
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
 }
	ImGui::End();
}

//ファイル読込
void FBX_LOADER::Load(const char* FilePath, bool bAnimOnly)
{
	//データ初期化
	if (!bAnimOnly) {
		m_aData.clear();
		m_aMesh.clear();
		m_aBone.clear();
		m_aAnimation.clear();
		m_aSkin.clear();
	}

	//ファイルパス保存
	m_FilePath = FilePath;
	char Sep1 = '\\';
	char Sep2 = 47;		//⇒'/'
	char Sep3 = '.';
	std::vector<std::string> aStr1 = gText::Split(m_FilePath, Sep1);
	std::vector<std::string> aStr2 = gText::Split(aStr1[aStr1.size() - 1], Sep2);
	std::vector<std::string> aStr3 = gText::Split(aStr2[aStr2.size() - 1], Sep3);	//パス分解⇒ファイル名取得
	m_FilePath = "Asset/Model/Input/";
	m_FilePath += aStr3[0];
	m_FilePath += "/";																//ファイルパス作成

	//インポータ作成
	m_pImporter = FbxImporter::Create(m_pManager, "");
	if (!m_pImporter->Initialize(FilePath, -1, m_pManager->GetIOSettings())) {		//モデル読込
		m_pImporter->Destroy();
		throw ERROR_EX2("インポートエラー");
	}

	//シーン作成
	if (m_pScene != nullptr) {
		m_pEvaluator->Destroy();
		m_pScene->Destroy();
	}
	m_pScene = FbxScene::Create(m_pManager, "Scene");
	if (!m_pImporter->Import(m_pScene)) {	//モデル情報格納
		m_pScene->Destroy();
		m_pImporter->Destroy();
		throw ERROR_EX2("インポートエラー");
	}
	m_pImporter->Destroy();

	//全般設定
	FbxSystemUnit SceneSystemUnit = m_pScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0f)
		FbxSystemUnit::cm.ConvertScene(m_pScene);		//シーンの単位をcmに設定する
	FbxGeometryConverter Converter(m_pManager);
	//Converter.Triangulate(m_pScene, true);				//ポリゴン三角化
	Converter.SplitMeshesPerMaterial(m_pScene, true);	//全Mesh分割（マテリアルごと）
	GetAnimationData();									//アニメーション取得

	//ノード取得
	if (!bAnimOnly) {
		GetBoneData();
		FbxNode* Root = m_pScene->GetRootNode();
		if (Root != nullptr)
			GetNodes(Root, 0);
	}
	else
		LoadAnimation();
}

void FBX_LOADER::Load(const wchar_t* FilePath, bool bAnimOnly)
{
	//文字列変換
	size_t i{};
	std::string Path;
	Path.resize(MAX_PATH);
	wcstombs_s(&i, const_cast<char*>(Path.c_str()), static_cast<size_t>(MAX_PATH), FilePath, static_cast<size_t>(MAX_PATH - 1));

	//ファイル読込
	Load(Path.c_str(), bAnimOnly);
}

//ノード取得
void FBX_LOADER::GetNodes(FbxNode* Node, int SpaceCnt, BONE_DATA* ParentBone)
{
	//属性タイプ名の配列
	const char* AttrTypeNames[] = {
		"eUnknown",				//不明
		"eNull",				//NULL
		"eMarker",				//マーカ
		"eSkeleton",			//ボーン
		"eMesh",				//メッシュ
		"eNurbs",				//
		"ePatch",				//
		"eCamera",				//
		"eCameraStereo",		//
		"eCameraSwitcher",		//
		"eLight",				//
		"eOpticalReference",	//
		"eOpticalMarker",		//
		"eNurbsCurve",			//NURBS曲線
		"eTrimNurbsSurface",	//NURBSトリム
		"eBoundary",			//境界ボリューム配列
		"eNurbsSurface",		//NURBS曲面
		"eShape",				//
		"eLODGroup",			//
		"eSubDiv",				//
		"eCachedEffect",		//
		"eLine"					//
	};

	//ノード
	FBX_DATA Data;
	Data.Node = Node;
	BONE_DATA* pBone = nullptr;	//ボーンのポインタ

	//ノード名
	for (size_t i = 0, Cnt = SpaceCnt; i < Cnt; i++)
		Data.NodeName += "->";
	Data.NodeName += Node->GetName();

	//属性情報
	size_t AttrCnt = Node->GetNodeAttributeCount();
	for (size_t i = 0; i < AttrCnt; ++i) {

		//属性
		FbxNodeAttribute* Attr = Node->GetNodeAttributeByIndex(static_cast<int>(i));
		Data.m_pAttributes.emplace_back(Attr);

		//属性名
		FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		Data.m_pAttributesType.push_back(AttrTypeNames[AttrType]);

		//情報読込
		switch (AttrType) {
			case FbxNodeAttribute::EType::eMesh:			//メッシュ情報取得
				GetMesh(Attr, Node->GetName());
				break;
			case FbxNodeAttribute::EType::eSkeleton:		//ボーン情報更新
				for (auto& b : m_aBone) {
					if (b.BoneName == Node->GetName()) {
						if (ParentBone == nullptr)			//ルートボーン確認
							b.IsRoot = true;
						else
							ParentBone->aChildBone.emplace_back(&b);
						pBone = &b;
						break;
					}
				}
				break;
			default:
				break;
		}
	}

	//データ格納
	m_aData.emplace_back(std::move(Data));

	//子ノードも同様に取得
	size_t ChildCnt = Node->GetChildCount();
	for (size_t i = 0; i < ChildCnt; ++i)
		GetNodes(Node->GetChild(static_cast<int>(i)), SpaceCnt + 1, pBone);
}

//メッシュ取得
void FBX_LOADER::GetMesh(FbxNodeAttribute* MeshIn, std::string NodeName)
{
	//メッシュ作成
	FbxMesh* pMesh = (FbxMesh*)MeshIn;
	MESH_DATA Mesh;
	Mesh.NodeName = NodeName;

	//インデックス取得
	for (size_t i = 0, Cnt = pMesh->GetPolygonCount(); i < Cnt; i++) {
		int iCnt = static_cast<int>(i);

		//左手系対策
		Mesh.vsData.m_Indices.emplace_back(static_cast<UINT>(iCnt * 3 + 2));
		Mesh.vsData.m_Indices.emplace_back(static_cast<UINT>(iCnt * 3 + 1));
		Mesh.vsData.m_Indices.emplace_back(static_cast<UINT>(iCnt * 3));
	}

	//頂点取得
	std::vector<dx::XMFLOAT3> aPos(0);
	FbxVector4* Vertices = pMesh->GetControlPoints();			//頂点バッファ取得
	Mesh.aIndexBuffer.resize(pMesh->GetControlPointsCount());	//頂点バッファ配列作成
	int* Indices = pMesh->GetPolygonVertices();					//インデックスバッファ取得
	size_t vtxCnt = pMesh->GetPolygonVertexCount();				//頂点座標の数取得
	for (size_t i = 0; i < vtxCnt; i++) {
		int iCnt = static_cast<int>(i);

		//インデックスバッファから頂点番号を取得
		int Index = Indices[iCnt];
		auto pVtx = &Vertices[Index][0];

		//頂点バッファから座標を取得
		dx::XMFLOAT3 Pos{};
		Pos.x = static_cast<float>(*pVtx);
		pVtx++;
		Pos.y = static_cast<float>(*pVtx);
		pVtx++;
		Pos.z = static_cast<float>(*pVtx);
		aPos.emplace_back(Pos);

		//頂点インデックス登録
		Mesh.aIndexBuffer[Index].aIndex.emplace_back(iCnt);
	}

	//法線取得
	std::vector<dx::XMFLOAT3> aNormal(0);
	FbxArray<FbxVector4> Normals;
	pMesh->GetPolygonVertexNormals(Normals);					//法線リスト取得
	for (size_t i = 0, Cnt = Normals.Size(); i < Cnt; i++) {
		auto pNor = &Normals[static_cast<int>(i)][0];

		VECTOR3 Normal{};
		Normal.x = static_cast<float>(*pNor);
		pNor++;
		Normal.y = static_cast<float>(*pNor);
		pNor++;
		Normal.z = static_cast<float>(*pNor);
		Normal = gMath::VecNormalize(Normal);
		aNormal.emplace_back(dx::XMFLOAT3{ Normal.x, Normal.y, Normal.z });
	}

	//UV座標取得
	std::vector<dx::XMFLOAT2> aUV;
	FbxStringList uvSetNames;
	pMesh->GetUVSetNames(uvSetNames);								//uvSetの名前リスト取得
	FbxArray<FbxVector2> uvBuff;
	pMesh->GetPolygonVertexUVs(uvSetNames.GetStringAt(0), uvBuff);	//uvSet取得
	for (size_t i = 0, Cnt = uvBuff.Size(); i < Cnt; i++) {
		auto puv = &uvBuff[static_cast<int>(i)][0];

		dx::XMFLOAT2 UV{};
		UV.x = static_cast<float>(*puv);
		puv++;
		UV.y = 1.0f - static_cast<float>(*puv);
		aUV.emplace_back(UV);
	}
	if (uvBuff.Size() == 0) {										//UV情報がない場合
		for (size_t i = 0, Cnt = aNormal.size(); i < Cnt; i++)
			aUV.emplace_back(dx::XMFLOAT2{ 0.0f, 0.0f });
	}

	//従法線取得
	std::vector<dx::XMFLOAT3> aBinormal(0);
	if (pMesh->GetElementBinormalCount() > 0) {
		FbxGeometryElementBinormal* pB = pMesh->GetElementBinormal();
		FbxGeometryElement::EMappingMode mapping = pB->GetMappingMode();
		FbxGeometryElement::EReferenceMode reference = pB->GetReferenceMode();
		if (mapping == FbxGeometryElement::EMappingMode::eByControlPoint &&
			reference == FbxGeometryElement::EReferenceMode::eDirect) {			//マッピングモードとリファレンスモードの確認

			//取得処理
			FbxLayerElementArrayTemplate<FbxVector4> paB = pB->GetDirectArray();
			for (int i = 0, Cnt = paB.GetCount(); i < Cnt; i++) {
				auto TempBinormal = paB.GetAt(i);
				dx::XMFLOAT3 vBinormal{};
				vBinormal.x = static_cast<float>(TempBinormal.mData[0]);
				vBinormal.y = static_cast<float>(TempBinormal.mData[1]);
				vBinormal.z = static_cast<float>(TempBinormal.mData[2]);
				aBinormal.emplace_back(vBinormal);
			}
		}
	}

	//接線取得
	std::vector<dx::XMFLOAT3> aTangent(0);
	if (pMesh->GetElementTangentCount() > 0) {
		FbxGeometryElementTangent* pT = pMesh->GetElementTangent();
		FbxGeometryElement::EMappingMode mapping = pT->GetMappingMode();
		FbxGeometryElement::EReferenceMode reference = pT->GetReferenceMode();
		if (mapping == FbxGeometryElement::EMappingMode::eByControlPoint &&
			reference == FbxGeometryElement::EReferenceMode::eDirect) {			//マッピングモードとリファレンスモードの確認

			//取得処理
			FbxLayerElementArrayTemplate<FbxVector4> paT = pT->GetDirectArray();
			for (int i = 0, Cnt = paT.GetCount(); i < Cnt; i++) {
				auto TempTangent = paT.GetAt(i);
				dx::XMFLOAT3 vTangent{};
				vTangent.x = static_cast<float>(TempTangent.mData[0]);
				vTangent.y = static_cast<float>(TempTangent.mData[1]);
				vTangent.z = static_cast<float>(TempTangent.mData[2]);
				aTangent.emplace_back(vTangent);
			}
		}
	}

	//取得失敗の場合、従法線と接線を計算で求める
	if (aBinormal.size() == 0 || aTangent.size() == 0) {
		aBinormal.resize(0);
		aTangent.resize(0);

		//すべてのポリゴンを計算
		for (size_t i = 0, Cnt = aPos.size() / 3; i < Cnt; i++) {

			//頂点座標取得
			dx::XMFLOAT3* pPos = &aPos[i * 3];
			VECTOR3 Pos[3];
			Pos[0] = VECTOR3{ pPos->x, pPos->y, pPos->z };
			pPos++;
			Pos[1] = VECTOR3{ pPos->x, pPos->y, pPos->z };
			pPos++;
			Pos[2] = VECTOR3{ pPos->x, pPos->y, pPos->z };

			//法線取得
			dx::XMFLOAT3* pNormal = &aNormal[i * 3];
			VECTOR3 Normal[3];
			Normal[0] = VECTOR3{ pNormal->x, pNormal->y, pNormal->z };
			pNormal++;
			Normal[1] = VECTOR3{ pNormal->x, pNormal->y, pNormal->z };
			pNormal++;
			Normal[2] = VECTOR3{ pNormal->x, pNormal->y, pNormal->z };

			//UV座標取得
			dx::XMFLOAT2* pTex = &aUV[i * 3];
			VECTOR2 Tex[3];
			Tex[0] = VECTOR2{ pTex->x, 1.0f - pTex->y };
			pTex++;
			Tex[1] = VECTOR2{ pTex->x, 1.0f - pTex->y };
			pTex++;
			Tex[2] = VECTOR2{ pTex->x, 1.0f - pTex->y };

			//接線を求める
			VECTOR3 Tangent[3];

			//Tangentのxを求める
			VECTOR3 vEdge1{
				Pos[1].x - Pos[0].x,
				Tex[1].x - Tex[0].x,
				Tex[1].y - Tex[0].y
			};
			VECTOR3 vEdge2{
				Pos[2].x - Pos[0].x,
				Tex[2].x - Tex[0].x,
				Tex[2].y - Tex[0].y
			};
			VECTOR3 CrossP = gMath::VecNormalize(gMath::VecCross(vEdge1, vEdge2), false);
			if (CrossP.x == 0.0f)
				CrossP.x = 1.0f;
			float Result = -(CrossP.y / CrossP.x);
			for (size_t j = 0; j < 3; j++)
				Tangent[j].x = Result;

			//Tangentのyを求める
			vEdge1.x = Pos[1].y - Pos[0].y;
			vEdge2.x = Pos[2].y - Pos[0].y;
			CrossP = gMath::VecNormalize(gMath::VecCross(vEdge1, vEdge2), false);
			if (CrossP.x == 0.0f)
				CrossP.x = 1.0f;
			Result = -(CrossP.y / CrossP.x);
			for (size_t j = 0; j < 3; j++)
				Tangent[j].y = Result;

			//Tangentのzを求める
			vEdge1.x = Pos[1].z - Pos[0].z;
			vEdge2.x = Pos[2].z - Pos[0].z;
			CrossP = gMath::VecNormalize(gMath::VecCross(vEdge1, vEdge2), false);
			if (CrossP.x == 0.0f)
				CrossP.x = 1.0f;
			Result = -(CrossP.y / CrossP.x);
			for (size_t j = 0; j < 3; j++)
				Tangent[j].z = Result;

			//法線と直行させる
			for (size_t j = 0; j < 3; j++) {
				Tangent[j] -= Normal[j] * gMath::VecDot(Tangent[j], Normal[j]);
				Tangent[j] = gMath::VecNormalize(Tangent[j], false);
				aTangent.push_back({ Tangent[j].x, Tangent[j].y, Tangent[j].z });
			}

			//従法線を求める
			VECTOR3 Bitangent[3];
			for (size_t j = 0; j < 3; j++) {
				Bitangent[j] = gMath::VecNormalize(gMath::VecCross(Normal[j], Tangent[j]), false);
				aBinormal.push_back({ Bitangent[j].x, Bitangent[j].y, Bitangent[j].z });
			}

			////TBN行列を修正
			//if (gMath::VecLength(Tangent[0]) == 0.0f || gMath::VecLength(Bitangent[0]) == 0.0f) {

			//	for (size_t k = 0; k < 3; k++)
			//	{
			//		aTangent[i * 3 + k]  = { 1.0f, 0.0f, 0.0f };
			//		aBinormal[i * 3 + k] = { 0.0f, 1.0f, 0.0f };
			//		aNormal[i * 3 + k]   = { 0.0f, 0.0f, 1.0f };
			//	}
			//}
		}
	}

	//頂点情報まとめ
	std::vector<VERTEX_M> aVertex;
	auto pPos = &aPos[0];
	auto pNormal = &aNormal[0];
	auto pUV = &aUV[0];
	auto pBinormal = &aBinormal[0];
	auto pTangent = &aTangent[0];
	for (size_t i = 0, Cnt = aPos.size(); i < Cnt; i++) {
		VERTEX_M vtx;
		vtx.m_Pos = *pPos;
		vtx.m_Normal = *pNormal;
		vtx.m_UV = *pUV;
		vtx.m_Binormal = *pBinormal;
		vtx.m_Tangent = *pTangent;
		aVertex.emplace_back(vtx);
		pPos++;
		pNormal++;
		pUV++;
		pBinormal++;
		pTangent++;
	}
	Mesh.vsData.m_Vertices = std::move(aVertex);

	//マテリアル取得
	int MaterialNum = pMesh->GetElementMaterialCount();		//マテリアル数取得
	if (MaterialNum == 0) {
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}
	FbxLayerElementMaterial* pMaterial = pMesh->GetElementMaterial(0);	//メッシュ側のマテリアル情報取得
	if (pMaterial == nullptr) {
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}
	int MaterialIndex = pMaterial->GetIndexArray().GetAt(0);			//マテリアルのインデックス取得
	FbxSurfaceMaterial* SurfaceMaterial = pMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(MaterialIndex);
	if (SurfaceMaterial == nullptr) {
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}
	Mesh.MaterialName += SurfaceMaterial->GetName();		//マテリアル名取得

	//LambertとPhongを識別
	FbxSurfacePhong* Phong{ nullptr };
	FbxSurfaceLambert* Lambert{ nullptr };
	if (SurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))				//Phongにダウンキャスト
		Phong = (FbxSurfacePhong*)SurfaceMaterial;
	else if (SurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {	//Lambertにダウンキャスト
		Lambert = (FbxSurfaceLambert*)SurfaceMaterial;
		Mesh.IsPhong = false;
	}
	else {
		Mesh.IsPhong = false;
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}

	//それぞれの場合の情報を取得
	if (Phong != nullptr) {

		//環境光
		Mesh.MaterialData.Ambient.x = static_cast<float>(Phong->Ambient.Get().mData[0]);
		Mesh.MaterialData.Ambient.y = static_cast<float>(Phong->Ambient.Get().mData[1]);
		Mesh.MaterialData.Ambient.z = static_cast<float>(Phong->Ambient.Get().mData[2]);
		Mesh.MaterialData.Ambient.w = static_cast<float>(Phong->AmbientFactor.Get());

		//拡散反射光
		Mesh.MaterialData.Diffuse.x = static_cast<float>(Phong->Diffuse.Get().mData[0]);
		Mesh.MaterialData.Diffuse.y = static_cast<float>(Phong->Diffuse.Get().mData[1]);
		Mesh.MaterialData.Diffuse.z = static_cast<float>(Phong->Diffuse.Get().mData[2]);
		Mesh.MaterialData.Diffuse.w = static_cast<float>(Phong->DiffuseFactor.Get());

		//発射光
		Mesh.MaterialData.Emissive.x = static_cast<float>(Phong->Emissive.Get().mData[0]);
		Mesh.MaterialData.Emissive.y = static_cast<float>(Phong->Emissive.Get().mData[1]);
		Mesh.MaterialData.Emissive.z = static_cast<float>(Phong->Emissive.Get().mData[2]);
		Mesh.MaterialData.Emissive.w = static_cast<float>(Phong->EmissiveFactor.Get());

		//透過度
		Mesh.MaterialData.Transparent.x = static_cast<float>(Phong->TransparentColor.Get().mData[0]);
		Mesh.MaterialData.Transparent.y = static_cast<float>(Phong->TransparentColor.Get().mData[1]);
		Mesh.MaterialData.Transparent.z = static_cast<float>(Phong->TransparentColor.Get().mData[2]);
		Mesh.MaterialData.Transparent.w = static_cast<float>(Phong->TransparencyFactor.Get());

		//鏡面反射光
		Mesh.MaterialData.Specular.x = static_cast<float>(Phong->Specular.Get().mData[0]);
		Mesh.MaterialData.Specular.y = static_cast<float>(Phong->Specular.Get().mData[1]);
		Mesh.MaterialData.Specular.z = static_cast<float>(Phong->Specular.Get().mData[2]);
		Mesh.MaterialData.Specular.w = static_cast<float>(Phong->SpecularFactor.Get());

		//光沢
		Mesh.MaterialData.Shininess = static_cast<float>(Phong->Shininess.Get());
	}
	else if (Lambert != nullptr) {

		//環境光
		Mesh.MaterialData.Ambient.x = static_cast<float>(Lambert->Ambient.Get().mData[0]);
		Mesh.MaterialData.Ambient.y = static_cast<float>(Lambert->Ambient.Get().mData[1]);
		Mesh.MaterialData.Ambient.z = static_cast<float>(Lambert->Ambient.Get().mData[2]);
		Mesh.MaterialData.Ambient.w = static_cast<float>(Lambert->AmbientFactor.Get());

		//拡散反射光
		Mesh.MaterialData.Diffuse.x = static_cast<float>(Lambert->Diffuse.Get().mData[0]);
		Mesh.MaterialData.Diffuse.y = static_cast<float>(Lambert->Diffuse.Get().mData[1]);
		Mesh.MaterialData.Diffuse.z = static_cast<float>(Lambert->Diffuse.Get().mData[2]);
		Mesh.MaterialData.Diffuse.w = static_cast<float>(Lambert->DiffuseFactor.Get());

		//発射光
		Mesh.MaterialData.Emissive.x = static_cast<float>(Lambert->Emissive.Get().mData[0]);
		Mesh.MaterialData.Emissive.y = static_cast<float>(Lambert->Emissive.Get().mData[1]);
		Mesh.MaterialData.Emissive.z = static_cast<float>(Lambert->Emissive.Get().mData[2]);
		Mesh.MaterialData.Emissive.w = static_cast<float>(Lambert->EmissiveFactor.Get());

		//透過度
		Mesh.MaterialData.Transparent.x = static_cast<float>(Lambert->TransparentColor.Get().mData[0]);
		Mesh.MaterialData.Transparent.y = static_cast<float>(Lambert->TransparentColor.Get().mData[1]);
		Mesh.MaterialData.Transparent.z = static_cast<float>(Lambert->TransparentColor.Get().mData[2]);
		Mesh.MaterialData.Transparent.w = static_cast<float>(Lambert->TransparencyFactor.Get());
	}

	//テクスチャ取得
	if (Phong != nullptr)
		GetTexturePath(Mesh, SurfaceMaterial);
	else if (Lambert != nullptr)
		GetTexturePath(Mesh, SurfaceMaterial, false);

	//テクスチャ確認
	if (Mesh.aTex_Diffuse.size() > 0) {
		Mesh.MaterialData.Diffuse.x = 1.0f;
		Mesh.MaterialData.Diffuse.y = 1.0f;
		Mesh.MaterialData.Diffuse.z = 1.0f;
	}
	if (Mesh.aTex_Specular.size() > 0) {
		Mesh.MaterialData.Specular.x = 1.0f;
		Mesh.MaterialData.Specular.y = 1.0f;
		Mesh.MaterialData.Specular.z = 1.0f;
	}

	//骨情報取得
	GetSkinData(Mesh, pMesh);

	//データ格納
	m_aMesh.emplace_back(std::move(Mesh));

	//[Lambert]
	//Bump_f4
	//NormalMap_f3
	//DisplacementColor_f4
	//VectorDisplacementColor_f4

	//[Phong]
	//Reflection_f4
}

//テクスチャパス取得
void FBX_LOADER::GetTexturePath(MESH_DATA& Mesh, FbxSurfaceMaterial* pMaterial, bool IsPhong) noexcept
{
	//それぞれのテクスチャを取得
	if (IsPhong) {

		//環境光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sAmbient, Mesh.aTex_Ambient, Mesh.aLayerTex_Ambient);

		//拡散反射光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDiffuse, Mesh.aTex_Diffuse, Mesh.aLayerTex_Diffuse);

		//発射光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sEmissive, Mesh.aTex_Emissive, Mesh.aLayerTex_Emissive);

		//透過度
		GetTextureName(pMaterial, FbxSurfaceMaterial::sTransparentColor, Mesh.aTex_Transparent, Mesh.aLayerTex_Transparent);

		//ノーマルマップ
		GetTextureName(pMaterial, FbxSurfaceMaterial::sNormalMap, Mesh.aTex_Normal, Mesh.aLayerTex_Normal);

		//視差マップ
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDisplacementColor, Mesh.aTex_Displacement, Mesh.aLayerTex_Displacement);

		//鏡面反射光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sSpecular, Mesh.aTex_Specular, Mesh.aLayerTex_Specular);

		//光沢
		GetTextureName(pMaterial, FbxSurfaceMaterial::sShininess, Mesh.aTex_Shininess, Mesh.aLayerTex_Shininess);
	}
	else {

		//環境光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sAmbient, Mesh.aTex_Ambient, Mesh.aLayerTex_Ambient);

		//拡散反射光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDiffuse, Mesh.aTex_Diffuse, Mesh.aLayerTex_Diffuse);

		//発射光
		GetTextureName(pMaterial, FbxSurfaceMaterial::sEmissive, Mesh.aTex_Emissive, Mesh.aLayerTex_Emissive);

		//透過度
		GetTextureName(pMaterial, FbxSurfaceMaterial::sTransparentColor, Mesh.aTex_Transparent, Mesh.aLayerTex_Transparent);

		//ノーマルマップ
		GetTextureName(pMaterial, FbxSurfaceMaterial::sNormalMap, Mesh.aTex_Normal, Mesh.aLayerTex_Normal);

		//視差マップ
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDisplacementColor, Mesh.aTex_Displacement, Mesh.aLayerTex_Displacement);
	}
}

//テクスチャ名取得
void FBX_LOADER::GetTextureName(FbxSurfaceMaterial* pMaterial, const char* Type, std::vector<std::string>& Tex, std::vector<LAYER_TEX_DATA>& LayerTex) noexcept
{
	FbxProperty Property = pMaterial->FindProperty(Type);
	size_t LayerNum = Property.GetSrcObjectCount<FbxLayeredTexture>();		//LayeredTextureの枚数取得
	if (LayerNum == 0) {													//LayeredTexture無し⇒通常テクスチャ

		//各テクスチャの情報を取得
		size_t TextureNum = Property.GetSrcObjectCount<FbxFileTexture>();	//通常テクスチャの枚数確認
		for (size_t i = 0; i < TextureNum; ++i) {
			FbxFileTexture* Texture = Property.GetSrcObject<FbxFileTexture>(static_cast<int>(i));	//テクスチャオブジェクト取得
			std::string FileName = "";
			if (Texture != nullptr)
				FileName += Texture->GetRelativeFileName();					//ファイルパス取得
			else
				FileName += "Texture Not Found!";

			//パス分解⇒ファイル名格納
			char Sep1 = '\\';
			char Sep2 = 47;		//⇒'/'
			std::vector<std::string> aStr1 = gText::Split(FileName, Sep1);
			std::vector<std::string> aStr2 = gText::Split(aStr1[aStr1.size() - 1], Sep2);
			Tex.emplace_back(aStr2[aStr2.size() - 1]);
		}
	}
	else {

		//LayeredTextureからテクスチャを取得
		for (size_t j = 0; j < LayerNum; ++j) {
			FbxLayeredTexture* LayeredTexture = Property.GetSrcObject<FbxLayeredTexture>(static_cast<int>(j));
			LayerTex.emplace_back(LAYER_TEX_DATA());

			//レイヤごとのテクスチャ情報を取得
			size_t TextureNum = LayeredTexture->GetSrcObjectCount<FbxFileTexture>();	//テクスチャの枚数確認
			for (size_t i = 0; i < TextureNum; ++i) {
				FbxFileTexture* Texture = Property.GetSrcObject<FbxFileTexture>(static_cast<int>(i));	//テクスチャオブジェクト取得
				std::string FileName = "";
				if (Texture != nullptr)
					FileName += Texture->GetRelativeFileName();							//ファイルパス取得
				else
					FileName += "Texture Not Found!";

				//パス分解⇒ファイル名格納
				char Sep1 = '\\';
				char Sep2 = 47;		//⇒'/'
				std::vector<std::string> aStr1 = gText::Split(FileName, Sep1);
				std::vector<std::string> aStr2 = gText::Split(aStr1[aStr1.size() - 1], Sep2);
				LayerTex[j].aTex.emplace_back(aStr2[aStr2.size() - 1]);
			}
		}
	}
}

//フレーム情報読込
void FBX_LOADER::GetSkinData(MESH_DATA& Mesh, FbxMesh* MeshIn) noexcept
{
	//例外処理
	if (m_aSkin.size() == 0)	//アニメーション情報がない
		return;

	//前処理
	FbxMesh* pMesh = MeshIn;
	FbxNode* pNode = pMesh->GetNode();

	//移動、回転、拡大の行列
	FbxVector4 vecT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 vecR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 vecS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix mtxGeometryOffset = FbxAMatrix{ vecT, vecR, vecS };

	//ボーン情報取得
	size_t SkinCnt = pMesh->GetDeformerCount(FbxDeformer::eSkin);	//スキンの数取得
	for (size_t i = 0; i < SkinCnt; ++i) {
		int AnimID = static_cast<int>(i + AnimLoadBaseIndex);		//アニメーションID
		size_t MaxFrame = m_aAnimation[AnimID].StopFrame;			//再生フレーム数

		//スキン取得
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(static_cast<int>(i), FbxDeformer::eSkin);
		size_t ClusterNum = pSkin->GetClusterCount();				//骨の数取得
		for (size_t j = 0; j < ClusterNum; ++j) {

			//骨ID取得
			FbxCluster* pCluster = pSkin->GetCluster(static_cast<int>(j));
			int BoneID{ 0 };
			std::string BoneName = pCluster->GetLink()->GetName();
			for (auto& b : m_aBone) {
				if (b.BoneName == BoneName) {
					BoneID = b.Index;
					Mesh.aBoneID.emplace_back(BoneID);	//対応する骨を記憶
					break;
				}
			}

			//初期姿勢
			FbxAMatrix clusterGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
			FbxAMatrix referenceGlobalInitPosition;
			pCluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= mtxGeometryOffset;
			FbxMatrix clusterRelativeInitPosition;
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
			Mesh.aReferenceGlobalInitPosition.emplace_back(referenceGlobalInitPosition);
			Mesh.aClusterRelativeInitPosition.emplace_back(clusterRelativeInitPosition);	//初期姿勢行列を記憶
			if (!m_aBone[BoneID].bMtxIsLoad) {

				//行列初期化
				float* pInitMtx = &m_aBone[BoneID].InitMatrix.m[0][0];
				auto pRefMtx = &clusterGlobalInitPosition[0][0];
				for (size_t x = 0; x < 4; x++) {
					for (size_t y = 0; y < 4; y++) {
						*pInitMtx = static_cast<float>(*pRefMtx);
						pInitMtx++;
						pRefMtx++;
					}
				}
				m_aBone[BoneID].bMtxIsLoad = true;
			}

			//フレーム姿勢
			if (m_aSkin[AnimID].aFrameData[BoneID].aMatrix.size() < MaxFrame) {		//フレーム姿勢未登録の場合⇒情報格納
				FRAME_DATA FrameData;
				for (size_t k = 0; k < MaxFrame; ++k) {
					FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
					FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
					FbxMatrix clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
					FbxMatrix clusterRelativeCurrentPositionInverse;
					clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

					FbxMatrix vertexTransformMatrix;
					vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

					//スキン
					dx::XMFLOAT4X4 Matrix{};
					float* pInitMtx = &Matrix.m[0][0];
					auto pRefMtx = &vertexTransformMatrix[0][0];
					for (size_t x = 0; x < 4; x++) {
						for (size_t y = 0; y < 4; y++) {
							*pInitMtx = static_cast<float>(*pRefMtx);
							pInitMtx++;
							pRefMtx++;
						}
					}
					FrameData.aMatrix.emplace_back(Matrix);

					//骨
					FbxMatrix boneTransformMatrix;
					boneTransformMatrix = clusterGlobalCurrentPosition * referenceGlobalInitPosition;
					pInitMtx = &Matrix.m[0][0];
					pRefMtx = &boneTransformMatrix[0][0];
					for (size_t x = 0; x < 4; x++) {
						for (size_t y = 0; y < 4; y++) {
							*pInitMtx = static_cast<float>(*pRefMtx);
							pInitMtx++;
							pRefMtx++;
						}
					}
					FrameData.aBoneMatrix.emplace_back(Matrix);
				}
				m_aSkin[AnimID].aFrameData[BoneID] = std::move(FrameData);
			}

			//その他情報取得
			size_t IndexNum = pCluster->GetControlPointIndicesCount();	//骨が影響するインデックスの数
			int* pIndices = pCluster->GetControlPointIndices();			//頂点インデックス配列
			double* pWeights = pCluster->GetControlPointWeights();		//比重配列
			for (size_t k = 0; k < IndexNum; ++k) {

				//頂点インデックス取得
				int vtxIndex = pIndices[k];

				//骨番号、比重を頂点バッファ情報に登録
				Mesh.aIndexBuffer[vtxIndex].aBoneID.emplace_back(BoneID);
				Mesh.aIndexBuffer[vtxIndex].aWeight.emplace_back(static_cast<float>(pWeights[k]));
			}
		}
	}

	//骨なしメッシュの場合
	if (SkinCnt == 0) {
		for (size_t i = 0, Cnt = m_aSkin.size() - AnimLoadBaseIndex; i < Cnt; ++i) {
			Mesh.aNoSkinData.emplace_back(MESH_DATA::NO_SKIN_MTX{});	//行列配列確保
			int AnimID = static_cast<int>(i + AnimLoadBaseIndex);		//アニメーションID
			size_t MaxFrame = m_aAnimation[AnimID].StopFrame;			//再生するフレーム数

			//フレーム姿勢
			for (size_t k = 0; k < MaxFrame; ++k) {
				FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
				FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
				//FbxMatrix clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
				//FbxMatrix clusterRelativeCurrentPositionInverse;
				//clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

				//FbxMatrix vertexTransformMatrix;
				//vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

				//スキン
				dx::XMFLOAT4X4 Matrix{};
				float* pInitMtx = &Matrix.m[0][0];
				auto pRefMtx = &globalPosition[0][0];
				for (size_t x = 0; x < 4; x++) {
					for (size_t y = 0; y < 4; y++) {
						*pInitMtx = static_cast<float>(*pRefMtx);
						pInitMtx++;
						pRefMtx++;
					}
				}
				Mesh.aNoSkinData[AnimID].aMatrix.emplace_back(Matrix);
			}
		}
	}
}

//フレーム情報読込（アニメーション単体用）
void FBX_LOADER::GetAnimationFromSkin(FbxMesh* MeshIn) noexcept
{
	//前処理
	FbxMesh* pMesh = MeshIn;
	FbxNode* pNode = pMesh->GetNode();

	//移動、回転、拡大の行列
	FbxVector4 vecT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 vecR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 vecS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix mtxGeometryOffset = FbxAMatrix{ vecT, vecR, vecS };

	//ボーン情報取得
	size_t SkinCnt = pMesh->GetDeformerCount(FbxDeformer::eSkin);	//スキンの数取得
	for (size_t i = 0; i < SkinCnt; ++i) {
		int AnimID = static_cast<int>(i + AnimLoadBaseIndex);		//アニメーションID
		size_t MaxFrame = m_aAnimation[AnimID].StopFrame;			//再生フレーム数

		//スキン取得
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(static_cast<int>(i), FbxDeformer::eSkin);
		size_t ClusterNum = pSkin->GetClusterCount();				//骨の数取得
		for (size_t j = 0; j < ClusterNum; ++j) {

			//骨ID取得
			FbxCluster* pCluster = pSkin->GetCluster(static_cast<int>(j));
			int BoneID{ 0 };
			std::string BoneName = pCluster->GetLink()->GetName();
			for (auto& b : m_aBone) {
				if (b.BoneName == BoneName) {
					BoneID = b.Index;
					break;
				}
			}

			//初期姿勢
			FbxAMatrix clusterGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
			FbxAMatrix referenceGlobalInitPosition;
			pCluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= mtxGeometryOffset;
			FbxMatrix clusterRelativeInitPosition;
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
			if (!m_aBone[BoneID].bMtxIsLoad) {

				//行列初期化
				float* pInitMtx = &m_aBone[BoneID].InitMatrix.m[0][0];
				auto pRefMtx = &clusterGlobalInitPosition[0][0];
				for (size_t x = 0; x < 4; x++) {
					for (size_t y = 0; y < 4; y++) {
						*pInitMtx = static_cast<float>(*pRefMtx);
						pInitMtx++;
						pRefMtx++;
					}
				}
				m_aBone[BoneID].bMtxIsLoad = true;
			}

			//フレーム姿勢
			if (m_aSkin[AnimID].aFrameData[BoneID].aMatrix.size() < MaxFrame) {		//フレーム姿勢未登録の場合⇒情報格納
				FRAME_DATA FrameData;
				for (size_t k = 0; k < MaxFrame; ++k) {
					FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
					FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
					FbxMatrix clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
					FbxMatrix clusterRelativeCurrentPositionInverse;
					clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

					FbxMatrix vertexTransformMatrix;
					vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

					//スキン
					dx::XMFLOAT4X4 Matrix{};
					float* pInitMtx = &Matrix.m[0][0];
					auto pRefMtx = &vertexTransformMatrix[0][0];
					for (size_t x = 0; x < 4; x++) {
						for (size_t y = 0; y < 4; y++) {
							*pInitMtx = static_cast<float>(*pRefMtx);
							pInitMtx++;
							pRefMtx++;
						}
					}
					FrameData.aMatrix.emplace_back(Matrix);

					//骨
					FbxMatrix boneTransformMatrix;
					boneTransformMatrix = clusterGlobalCurrentPosition * referenceGlobalInitPosition;
					pInitMtx = &Matrix.m[0][0];
					pRefMtx = &boneTransformMatrix[0][0];
					for (size_t x = 0; x < 4; x++) {
						for (size_t y = 0; y < 4; y++) {
							*pInitMtx = static_cast<float>(*pRefMtx);
							pInitMtx++;
							pRefMtx++;
						}
					}
					FrameData.aBoneMatrix.emplace_back(Matrix);
				}
				m_aSkin[AnimID].aFrameData[BoneID] = std::move(FrameData);
			}
		}
	}

	//骨なしメッシュの場合
	if (SkinCnt == 0) {

		//メッシュノード確認
		MESH_DATA* MeshData = nullptr;
		for (auto& m : m_aMesh) {
			if (m.NodeName == pNode->GetName()) {
				MeshData = &m;
				break;
			}
		}
		if (MeshData == nullptr)
			return;

		for (size_t i = 0, Cnt = m_aSkin.size() - AnimLoadBaseIndex; i < Cnt; ++i) {
			MeshData->aNoSkinData.emplace_back(MESH_DATA::NO_SKIN_MTX{});	//行列配列確保
			int AnimID = static_cast<int>(i + AnimLoadBaseIndex);			//アニメーションID
			size_t MaxFrame = m_aAnimation[AnimID].StopFrame;				//再生するフレーム数

			//フレーム姿勢
			for (size_t k = 0; k < MaxFrame; ++k) {
				FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
				FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);

				//行列格納
				dx::XMFLOAT4X4 Matrix{};
				float* pInitMtx = &Matrix.m[0][0];
				auto pRefMtx = &globalPosition[0][0];
				for (size_t x = 0; x < 4; x++) {
					for (size_t y = 0; y < 4; y++) {
						*pInitMtx = static_cast<float>(*pRefMtx);
						pInitMtx++;
						pRefMtx++;
					}
				}
				MeshData->aNoSkinData[AnimID].aMatrix.emplace_back(Matrix);
			}
		}
	}
}

//骨情報取得
void FBX_LOADER::GetBoneData() noexcept
{
	//例外処理
	size_t BoneCount = m_pScene->GetSrcObjectCount<FbxSkeleton>();
	if (BoneCount == 0)
		return;

	//読込処理
	for (size_t i = 0; i < BoneCount; ++i) {
		int iCnt = static_cast<int>(i);

		FbxSkeleton* pBone = m_pScene->GetSrcObject<FbxSkeleton>(iCnt);		//ポインタ取得
		BONE_DATA Data;
		Data.Index = iCnt;
		Data.BoneName = pBone->GetNode()->GetName();
		m_aBone.emplace_back(Data);
	}

	//フレーム情報用配列確保
	int BoneNum = static_cast<int>(m_aBone.size());
	for (auto& bs : m_aSkin) {
		if (bs.aFrameData.size() == 0)
			bs.aFrameData.resize(BoneNum);
	}
}

//アニメーション取得
void FBX_LOADER::GetAnimationData() noexcept
{
	m_pEvaluator = m_pScene->GetAnimationEvaluator();			//アニメーション評価取得
	m_TimeMode = m_pScene->GetGlobalSettings().GetTimeMode();	//アニメーション時間設定取得
	m_Period.SetTime(0, 0, 0, 1, 0, m_TimeMode);				//フレーム時間取得

	//テイク情報取得
	FbxArray<FbxString*> aTakeName;					//テイク配列
	m_pScene->FillAnimStackNameArray(aTakeName);
	size_t TakeNum = aTakeName.GetCount();			//テイク数取得
	for (size_t i = 0; i < TakeNum; ++i) {
		int iCnt = static_cast<int>(i);

		//テイク名からテイク情報を取得
		FbxTakeInfo* CurrentTakeInfo = m_pScene->GetTakeInfo(*(aTakeName[iCnt]));
		if (CurrentTakeInfo != nullptr) {
			ANIM_DATA Data;
			Data.Start = CurrentTakeInfo->mLocalTimeSpan.GetStart();		//開始時刻
			Data.Stop = CurrentTakeInfo->mLocalTimeSpan.GetStop();			//終了時刻
			Data.StartFrame = (int)(Data.Start.Get() / m_Period.Get());		//開始フレーム
			Data.StopFrame = (int)(Data.Stop.Get() / m_Period.Get());		//終了フレーム
			Data.TakeName = CurrentTakeInfo->mName;							//テイク名

			//フレーム数確認
			if (Data.StartFrame < 0) {
				Data.StopFrame -= Data.StartFrame;
				Data.StartFrame = 0;
			}
			if (m_TimeMode == fbxsdk::FbxTime::eFrames30)
				Data.bIsFPS_30 = true;
			m_aAnimation.emplace_back(Data);
		}

		//メモリ解放
		aTakeName[iCnt]->Clear();
		SAFE_DELETE(aTakeName[iCnt]);
	}
	aTakeName.Clear();	//メモリ解放

	//骨情報配列追加
	AnimLoadBaseIndex = static_cast<int>(m_aSkin.size());	//読込開始位置更新
	size_t BoneNum = m_aAnimation.size() - m_aSkin.size();
	for (size_t i = 0; i < BoneNum; i++)
		m_aSkin.emplace_back(SKIN_DATA{});					//スキン情報用配列確保
}

//アニメーション読込
void FBX_LOADER::LoadAnimation()
{
	//例外処理
	if (m_aSkin.size() == AnimLoadBaseIndex)
		throw ERROR_EX2("アニメーション読込エラー：アニメーション情報がない。");

	//フレーム情報用配列確保
	int BoneNum = static_cast<int>(m_aBone.size());
	for (auto& bs : m_aSkin) {
		if (bs.aFrameData.size() == 0)
			bs.aFrameData.resize(BoneNum);
	}

	//メッシュから読込む場合
	std::vector<FbxMesh*> aMesh(0);
	size_t MeshCount = m_pScene->GetSrcObjectCount<FbxMesh>();
	for (size_t i = 0; i < MeshCount; ++i)
		aMesh.emplace_back(m_pScene->GetSrcObject<FbxMesh>(static_cast<int>(i)));  //メッシュポインタ取得
	if (MeshCount > 0 && MeshCount == m_aMesh.size()) {
		for (auto& m : aMesh)
			GetAnimationFromSkin(m);
		return;
	}

	//メッシュがない場合
	std::vector<FbxNode*> aBone(0);									//骨ノードのポインタ配列
	size_t BoneCount = m_pScene->GetSrcObjectCount<FbxSkeleton>();
	if (BoneCount == 0)
		throw ERROR_EX2("アニメーション読込エラー：骨ノードがない。");
	for (size_t i = 0; i < BoneCount; ++i)
		aBone.push_back(m_pScene->GetSrcObject<FbxSkeleton>(static_cast<int>(i))->GetNode());

	//アニメーションの数ごと
	for (size_t i = 0, Cnt = m_aSkin.size() - AnimLoadBaseIndex; i < Cnt; ++i) {
		int AnimID = static_cast<int>(i + AnimLoadBaseIndex);	//アニメーションID
		size_t MaxFrame = m_aAnimation[AnimID].StopFrame;		//再生するフレーム数
		for (auto& m : m_aMesh) {								//メッシュごと

			//骨なしの場合
			if (m.aNoSkinData.size() > 0) {
				m.aNoSkinData.emplace_back(MESH_DATA::NO_SKIN_MTX{});	//行列配列確保

				//メッシュノード確認
				FbxNode* pNode = nullptr;
				for (auto& node : aMesh) {
					if (m.NodeName == node->GetNode()->GetName()) {
						pNode = node->GetNode();
						break;
					}
				}

				//フレーム姿勢
				for (size_t k = 0; k < MaxFrame; ++k) {
					FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
					FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);

					//行列格納
					dx::XMFLOAT4X4 Matrix{};
					float* pInitMtx = &Matrix.m[0][0];
					auto pRefMtx = &globalPosition[0][0];
					for (size_t x = 0; x < 4; x++) {
						for (size_t y = 0; y < 4; y++) {
							*pInitMtx = static_cast<float>(*pRefMtx);
							pInitMtx++;
							pRefMtx++;
						}
					}
					m.aNoSkinData[AnimID].aMatrix.emplace_back(Matrix);
				}
				continue;
			}

			//骨ありの場合
			for (size_t j = 0; j < m.aBoneID.size(); ++j) {		//対応する骨

				//フレーム姿勢
				if (m_aSkin[AnimID].aFrameData[m.aBoneID[j]].aMatrix.size() < MaxFrame) {	//フレーム姿勢未登録の場合⇒情報格納
					FRAME_DATA FrameData;
					for (size_t k = 0; k < MaxFrame; ++k) {
						FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
						//FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
						FbxMatrix clusterGlobalCurrentPosition = aBone[m.aBoneID[j]]->EvaluateGlobalTransform(time);
						FbxMatrix clusterRelativeCurrentPositionInverse;
						clusterRelativeCurrentPositionInverse = clusterGlobalCurrentPosition;

						FbxMatrix vertexTransformMatrix;
						vertexTransformMatrix = clusterRelativeCurrentPositionInverse * m.aClusterRelativeInitPosition[j];

						//スキン
						dx::XMFLOAT4X4 Matrix{};
						float* pInitMtx = &Matrix.m[0][0];
						auto pRefMtx = &vertexTransformMatrix[0][0];
						for (size_t x = 0; x < 4; x++) {
							for (size_t y = 0; y < 4; y++) {
								*pInitMtx = static_cast<float>(*pRefMtx);
								pInitMtx++;
								pRefMtx++;
							}
						}
						FrameData.aMatrix.emplace_back(Matrix);

						//骨
						FbxMatrix boneTransformMatrix;
						boneTransformMatrix = clusterGlobalCurrentPosition * m.aReferenceGlobalInitPosition[j];
						pInitMtx = &Matrix.m[0][0];
						pRefMtx = &boneTransformMatrix[0][0];
						for (size_t x = 0; x < 4; x++) {
							for (size_t y = 0; y < 4; y++) {
								*pInitMtx = static_cast<float>(*pRefMtx);
								pInitMtx++;
								pRefMtx++;
							}
						}
						FrameData.aBoneMatrix.emplace_back(Matrix);
					}
					m_aSkin[AnimID].aFrameData[m.aBoneID[j]] = std::move(FrameData);
				}
			}
		}
	}
}

//ファイル書出し
void FBX_LOADER::SaveModelData(const char* FileName) noexcept
{
	//ファイルパス
	std::string Path = "Asset/Model/Output/";
	Path += FileName;
	std::ostringstream oss;

	//モデル情報書出し
	MODEL_BIN ModelBin;
	if (m_aMesh.size() == 0)
		return;
	ModelBin.MeshNum = static_cast<int>(m_aMesh.size());
	if (m_aBone.size() > 0)
		ModelBin.BoneNum = static_cast<int>(m_aBone.size());
	else
		ModelBin.BoneNum = 0;
	for (auto& m : m_aMesh) {
		if (m.aNoSkinData.size() > 0)
			ModelBin.NoSkinNum++;
	}
	oss << Path << ".bin";
	FILE_IO::SaveFile(oss.str().c_str(), &ModelBin);
	oss.str("");

	//メッシュ情報（リスト）書出し
	std::vector<MESH_BIN> aMeshBin(ModelBin.MeshNum);
	MESH_BIN* pMeshBin = &aMeshBin[0];
	MESH_DATA* pMeshData = &m_aMesh[0];
	for (auto& m : m_aMesh) {
		pMeshBin->NameSize = static_cast<int>(pMeshData->NodeName.size());
		pMeshBin->VertexNum = static_cast<int>(pMeshData->vsData.m_Indices.size());
		if (m.aTex_Diffuse.size() > 0)
			pMeshBin->NameSize_Diffuse = static_cast<int>(pMeshData->aTex_Diffuse[0].size());
		if (m.aTex_Specular.size() > 0)
			pMeshBin->NameSize_Specular = static_cast<int>(pMeshData->aTex_Specular[0].size());
		if (m.aTex_Normal.size() > 0)
			pMeshBin->NameSize_Normal = static_cast<int>(pMeshData->aTex_Normal[0].size());
		if (m.aTex_Displacement.size() > 0)
			pMeshBin->NameSize_Displacement = static_cast<int>(pMeshData->aTex_Displacement[0].size());
		pMeshBin++;
		pMeshData++;
	}
	oss << Path << "_Mesh.bin";
	FILE_IO::SaveFile(oss.str().c_str(), aMeshBin);
	oss.str("");

	//メッシュ情報（詳細）書出し
	for (size_t i = 0; i < ModelBin.MeshNum; i++) {			//メッシュごと

		//VSデータ作成
		VS_DATA<VERTEX_M>& DataRef = m_aMesh[i].vsData;
		VS_DATA<VERTEX_MB> vsData;
		vsData.m_Indices = DataRef.m_Indices;				//インデックス
		for (auto& v : DataRef.m_Vertices) {				//頂点情報

			//頂点情報
			VERTEX_MB Vertex;
			Vertex.m_Pos = v.m_Pos;
			Vertex.m_UV = v.m_UV;
			Vertex.m_Normal = v.m_Normal;
			Vertex.m_Binormal = v.m_Binormal;
			Vertex.m_Tangent = v.m_Tangent;
			vsData.m_Vertices.emplace_back(Vertex);
		}

		//骨比重
		for (auto& idx : m_aMesh[i].aIndexBuffer) {			//頂点バッファインデックス配列

			//対応する頂点へ反映
			size_t BoneNum = idx.aBoneID.size();			//頂点ごとの骨の数
			for (auto& idxNum : idx.aIndex) {

				//骨が影響しない場合
				if (BoneNum == 0) {
					vsData.m_Vertices[idxNum].m_BoneWeight[0] = 1.0f;
					continue;
				}

				//骨比重を格納
				for (size_t k = 0; k < 4; k++) {			//4つ以上の骨を破棄
					if (k >= BoneNum)						//例外処理（骨が4つ未満の場合）
						break;
					vsData.m_Vertices[idxNum].m_BoneID[k] = idx.aBoneID[k];
					vsData.m_Vertices[idxNum].m_BoneWeight[k] = idx.aWeight[k];
				}
			}
		}

		//ファイル書出し（メッシュ名）
		oss << Path << "_Mesh" << i << "_Name.bin";
		FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].NodeName);
		oss.str("");

		//ファイル書出し（インデックス）
		oss << Path << "_Mesh" << i << "_Idx.bin";
		FILE_IO::SaveFile(oss.str().c_str(), vsData.m_Indices);
		oss.str("");

		//ファイル書出し（頂点情報）
		oss << Path << "_Mesh" << i << "_Vtx.bin";
		FILE_IO::SaveFile(oss.str().c_str(), vsData.m_Vertices);
		oss.str("");

		//ファイル書出し（マテリアル）
		oss << Path << "_Mesh" << i << "_Material.bin";
		FILE_IO::SaveFile(oss.str().c_str(), &m_aMesh[i].MaterialData);
		oss.str("");

		//ファイル書出し（テクスチャD）
		oss << Path << "_Mesh" << i << "_TexD.bin";
		if (m_aMesh[i].aTex_Diffuse.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Diffuse[0]);
		oss.str("");

		//ファイル書出し（テクスチャS）
		oss << Path << "_Mesh" << i << "_TexS.bin";
		if (m_aMesh[i].aTex_Specular.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Specular[0]);
		oss.str("");

		//ファイル書出し（テクスチャN）
		oss << Path << "_Mesh" << i << "_TexN.bin";
		if (m_aMesh[i].aTex_Normal.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Normal[0]);
		oss.str("");

		//ファイル書出し（テクスチャDisp）
		oss << Path << "_Mesh" << i << "_TexDisp.bin";
		if (m_aMesh[i].aTex_Displacement.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Displacement[0]);
		oss.str("");
	}

	//骨情報（リスト）書出し
	std::vector<BONE_BIN> aBoneBin(ModelBin.BoneNum);
	for (size_t i = 0; i < ModelBin.BoneNum; i++)		//骨ごと
		aBoneBin[i].NameSize = static_cast<int>(m_aBone[i].BoneName.size());
	oss << Path << "_Bone.bin";
	FILE_IO::SaveFile(oss.str().c_str(), aBoneBin);
	oss.str("");

	//骨情報（詳細）書出し
	for (size_t i = 0; i < ModelBin.BoneNum; i++) {

		//ファイル書出し（骨名）
		oss << Path << "_Bone" << i << "_Name.bin";
		FILE_IO::SaveFile(oss.str().c_str(), m_aBone[i].BoneName);
		oss.str("");

		//ファイル書出し（初期姿勢）
		oss << Path << "_Bone" << i << "_Mtx.bin";
		FILE_IO::SaveFile(oss.str().c_str(), &m_aBone[i].InitMatrix);
		oss.str("");
	}

	//骨なしメッシュ番号書出し
	std::vector<int> aNoSkinIndex(0);
	for (size_t i = 0, Cnt = m_aMesh.size(); i < Cnt; i++) {
		if (m_aMesh[i].aNoSkinData.size() > 0)
			aNoSkinIndex.emplace_back(static_cast<int>(i));
	}
	oss << Path << "_NoSkin.bin";
	FILE_IO::SaveFile(oss.str().c_str(), aNoSkinIndex);
	oss.str("");
}

//ファイル書出し（アニメーション）
void FBX_LOADER::SaveAnimData(const char* FileName, int AnimID) noexcept
{
	//例外処理
	if (m_aAnimation.size() <= AnimID)
		return;

	//ファイルパス
	std::string Path = "Asset/Model/Output/Animation/Anim_";
	Path += FileName;
	std::ostringstream oss;

	//情報書出し（リスト）
	ANIM_BIN AnimBin;
	AnimBin.FrameNum = m_aAnimation[AnimID].StopFrame - m_aAnimation[AnimID].StartFrame;
	oss << Path << ".bin";
	FILE_IO::SaveFile(oss.str().c_str(), &AnimBin);
	oss.str("");

	//情報書出し（詳細）
	for (size_t i = 0, Cnt = m_aBone.size(); i < Cnt; i++) {	//骨ごと
		auto FrameData = &m_aSkin[AnimID].aFrameData[i];

		//骨姿勢
		oss << Path << "_BoneMtx" << i << ".bin";
		FILE_IO::SaveFile(oss.str().c_str(), FrameData->aBoneMatrix);
		oss.str("");

		//フレーム姿勢
		oss << Path << "_FrameMtx" << i << ".bin";
		FILE_IO::SaveFile(oss.str().c_str(), FrameData->aMatrix);
		oss.str("");
	}

	//情報書出し（骨なし）
	for (size_t i = 0, Cnt = m_aMesh.size(); i < Cnt; i++) {	//メッシュごと

		//骨なしメッシュ確認
		if (m_aMesh[i].aNoSkinData.size() > 0) {

			//フレーム姿勢
			oss << Path << "_NoSkinMtx" << i << ".bin";
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aNoSkinData[AnimID].aMatrix);
			oss.str("");
		}
	}
}
