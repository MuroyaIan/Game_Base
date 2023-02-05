
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/ModelLoader.h>
#include <Tool/imguiMgr.h>
#include <Tool/FileIO.h>
#include <Tool/TextIO.h>
#include <Tool/gMath.h>

//===== �ǉ����C�u���� =====
#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")
#pragma comment(lib, "zlib-mt.lib")

namespace dx = DirectX;

//===== �N���X���� =====
FBX_LOADER::FBX_LOADER() noexcept :
	m_pManager(nullptr), m_pioSettings(nullptr), m_pImporter(nullptr), m_pScene(nullptr), m_pEvaluator(nullptr), m_TimeMode(FbxTime::eDefaultMode), m_Period(), AnimLoadBaseIndex(0),
	m_FilePath(""), m_aData(0), m_aMesh(0), m_aAnimation(0), m_aBone(0), m_aSkin(0)
{
	//�}�l�[�W���쐬
	m_pManager = FbxManager::Create();

	//���o�͐ݒ�쐬
	m_pioSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(m_pioSettings);
}

FBX_LOADER::~FBX_LOADER() noexcept
{
	//�V�[�����
	if (m_pScene != nullptr)
		m_pScene->Destroy();

	//�}�l�[�W�����
	m_pManager->Destroy();
}

//�`�揈��
void FBX_LOADER::Draw() const noexcept
{
	//���[�_�pUI
	if (ImGui::Begin("FBX Loader")){

		//�m�[�h���
		if (ImGui::TreeNode("Nodes")) {

			//�m�[�h�ꗗ
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar);

			//�m�[�h���Ƒ�����
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

		//���b�V�����
		if (ImGui::TreeNode("Meshes")) {

			//���b�V���ꗗ
			if (m_aMesh.size() > 0) {
				ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "MeshNum"); ImGui::SameLine(); ImGui::Text(": %d", m_aMesh.size());
			}
			for (size_t i = 0, Cnt = m_aMesh.size(); i < Cnt; i++) {

				//���o��
				ImGui::Text(U8(u8"�@")); ImGui::SameLine(); ImGui::Text("Mesh : %s", m_aMesh[i].NodeName.c_str());

				//�|���S�����A���_���A�C���f�b�N�X��
				ImGui::Text("Polygons : %d", m_aMesh[i].vsData.m_Vertices.size() / 3);
				ImGui::Text("Vertices : %d", m_aMesh[i].vsData.m_Vertices.size());
				ImGui::Text("Indices : %d", m_aMesh[i].vsData.m_Indices.size());
				ImGui::Text("Material : %s", m_aMesh[i].MaterialName.c_str()); ImGui::SameLine();
				if (m_aMesh[i].IsPhong)
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.5f, 1.0f }, "(Phong)");
				else
					ImGui::TextColored(ImVec4{ 1.0f, 0.5f, 0.5f, 1.0f }, "(NotPhong)");

				//�e�N�X�`�����
				std::string TexName = "";
				TexName += m_aMesh[i].MaterialName;
				TexName += "'s Textures";
				if (ImGui::TreeNode(TexName.c_str())) {

					//����
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

					//�g�U���ˌ�
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

					//���ˌ�
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

					//���ߓx
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

					//���ʔ��ˌ�
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

					//����
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

		//�A�j���[�V�������
		if (ImGui::TreeNode("Animations")) {

			//�A�j���[�V�����ꗗ
			for (size_t i = 0, Cnt = m_aAnimation.size(); i < Cnt; i++) {

				//���o��
				ImGui::Text(U8(u8"�@")); ImGui::SameLine(); ImGui::Text("Animation %d : %s", i + 1, m_aAnimation[i].TakeName.c_str());
				ImGui::Text("FrameNum : %d", m_aAnimation[i].StopFrame - m_aAnimation[i].StartFrame);
			}
			ImGui::TreePop();
		}

		//�����
		if (ImGui::TreeNode("Bones")) {

			//���ꗗ
			for (size_t i = 0, Cnt = m_aSkin.size(); i < Cnt; i++) {

				//�X�L�����Ƃɕ\��
				std::ostringstream oss;
				oss << "Skin " << (i + 1);
				if (ImGui::TreeNode(oss.str().c_str())) {

					//���o��
					auto& aBones = m_aBone;
					int BoneNum = static_cast<int>(aBones.size());
					ImGui::Text(U8(u8"�@")); ImGui::SameLine(); ImGui::Text("BoneNum : %d", BoneNum);

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

//�t�@�C���Ǎ�
void FBX_LOADER::Load(const char* FilePath, bool bAnimOnly)
{
	//�f�[�^������
	if (!bAnimOnly) {
		m_aData.clear();
		m_aMesh.clear();
		m_aBone.clear();
		m_aAnimation.clear();
		m_aSkin.clear();
	}

	//�t�@�C���p�X�ۑ�
	m_FilePath = FilePath;
	char Sep1 = '\\';
	char Sep2 = 47;		//��'/'
	char Sep3 = '.';
	std::vector<std::string> aStr1 = gText::Split(m_FilePath, Sep1);
	std::vector<std::string> aStr2 = gText::Split(aStr1[aStr1.size() - 1], Sep2);
	std::vector<std::string> aStr3 = gText::Split(aStr2[aStr2.size() - 1], Sep3);	//�p�X�����˃t�@�C�����擾
	m_FilePath = "Asset/Model/Input/";
	m_FilePath += aStr3[0];
	m_FilePath += "/";																//�t�@�C���p�X�쐬

	//�C���|�[�^�쐬
	m_pImporter = FbxImporter::Create(m_pManager, "");
	if (!m_pImporter->Initialize(FilePath, -1, m_pManager->GetIOSettings())) {		//���f���Ǎ�
		m_pImporter->Destroy();
		throw ERROR_EX2("�C���|�[�g�G���[");
	}

	//�V�[���쐬
	if (m_pScene != nullptr) {
		m_pEvaluator->Destroy();
		m_pScene->Destroy();
	}
	m_pScene = FbxScene::Create(m_pManager, "Scene");
	if (!m_pImporter->Import(m_pScene)) {	//���f�����i�[
		m_pScene->Destroy();
		m_pImporter->Destroy();
		throw ERROR_EX2("�C���|�[�g�G���[");
	}
	m_pImporter->Destroy();

	//�S�ʐݒ�
	FbxSystemUnit SceneSystemUnit = m_pScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0f)
		FbxSystemUnit::cm.ConvertScene(m_pScene);		//�V�[���̒P�ʂ�cm�ɐݒ肷��
	FbxGeometryConverter Converter(m_pManager);
	//Converter.Triangulate(m_pScene, true);				//�|���S���O�p��
	Converter.SplitMeshesPerMaterial(m_pScene, true);	//�SMesh�����i�}�e���A�����Ɓj
	GetAnimationData();									//�A�j���[�V�����擾

	//�m�[�h�擾
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
	//������ϊ�
	size_t i{};
	std::string Path;
	Path.resize(MAX_PATH);
	wcstombs_s(&i, const_cast<char*>(Path.c_str()), static_cast<size_t>(MAX_PATH), FilePath, static_cast<size_t>(MAX_PATH - 1));

	//�t�@�C���Ǎ�
	Load(Path.c_str(), bAnimOnly);
}

//�m�[�h�擾
void FBX_LOADER::GetNodes(FbxNode* Node, int SpaceCnt, BONE_DATA* ParentBone)
{
	//�����^�C�v���̔z��
	const char* AttrTypeNames[] = {
		"eUnknown",				//�s��
		"eNull",				//NULL
		"eMarker",				//�}�[�J
		"eSkeleton",			//�{�[��
		"eMesh",				//���b�V��
		"eNurbs",				//
		"ePatch",				//
		"eCamera",				//
		"eCameraStereo",		//
		"eCameraSwitcher",		//
		"eLight",				//
		"eOpticalReference",	//
		"eOpticalMarker",		//
		"eNurbsCurve",			//NURBS�Ȑ�
		"eTrimNurbsSurface",	//NURBS�g����
		"eBoundary",			//���E�{�����[���z��
		"eNurbsSurface",		//NURBS�Ȗ�
		"eShape",				//
		"eLODGroup",			//
		"eSubDiv",				//
		"eCachedEffect",		//
		"eLine"					//
	};

	//�m�[�h
	FBX_DATA Data;
	Data.Node = Node;
	BONE_DATA* pBone = nullptr;	//�{�[���̃|�C���^

	//�m�[�h��
	for (size_t i = 0, Cnt = SpaceCnt; i < Cnt; i++)
		Data.NodeName += "->";
	Data.NodeName += Node->GetName();

	//�������
	size_t AttrCnt = Node->GetNodeAttributeCount();
	for (size_t i = 0; i < AttrCnt; ++i) {

		//����
		FbxNodeAttribute* Attr = Node->GetNodeAttributeByIndex(static_cast<int>(i));
		Data.m_pAttributes.emplace_back(Attr);

		//������
		FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		Data.m_pAttributesType.push_back(AttrTypeNames[AttrType]);

		//���Ǎ�
		switch (AttrType) {
			case FbxNodeAttribute::EType::eMesh:			//���b�V�����擾
				GetMesh(Attr, Node->GetName());
				break;
			case FbxNodeAttribute::EType::eSkeleton:		//�{�[�����X�V
				for (auto& b : m_aBone) {
					if (b.BoneName == Node->GetName()) {
						if (ParentBone == nullptr)			//���[�g�{�[���m�F
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

	//�f�[�^�i�[
	m_aData.emplace_back(std::move(Data));

	//�q�m�[�h�����l�Ɏ擾
	size_t ChildCnt = Node->GetChildCount();
	for (size_t i = 0; i < ChildCnt; ++i)
		GetNodes(Node->GetChild(static_cast<int>(i)), SpaceCnt + 1, pBone);
}

//���b�V���擾
void FBX_LOADER::GetMesh(FbxNodeAttribute* MeshIn, std::string NodeName)
{
	//���b�V���쐬
	FbxMesh* pMesh = (FbxMesh*)MeshIn;
	MESH_DATA Mesh;
	Mesh.NodeName = NodeName;

	//�C���f�b�N�X�擾
	for (size_t i = 0, Cnt = pMesh->GetPolygonCount(); i < Cnt; i++) {
		int iCnt = static_cast<int>(i);

		//����n�΍�
		Mesh.vsData.m_Indices.emplace_back(static_cast<UINT>(iCnt * 3 + 2));
		Mesh.vsData.m_Indices.emplace_back(static_cast<UINT>(iCnt * 3 + 1));
		Mesh.vsData.m_Indices.emplace_back(static_cast<UINT>(iCnt * 3));
	}

	//���_�擾
	std::vector<dx::XMFLOAT3> aPos(0);
	FbxVector4* Vertices = pMesh->GetControlPoints();			//���_�o�b�t�@�擾
	Mesh.aIndexBuffer.resize(pMesh->GetControlPointsCount());	//���_�o�b�t�@�z��쐬
	int* Indices = pMesh->GetPolygonVertices();					//�C���f�b�N�X�o�b�t�@�擾
	size_t vtxCnt = pMesh->GetPolygonVertexCount();				//���_���W�̐��擾
	for (size_t i = 0; i < vtxCnt; i++) {
		int iCnt = static_cast<int>(i);

		//�C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int Index = Indices[iCnt];
		auto pVtx = &Vertices[Index][0];

		//���_�o�b�t�@������W���擾
		dx::XMFLOAT3 Pos{};
		Pos.x = static_cast<float>(*pVtx);
		pVtx++;
		Pos.y = static_cast<float>(*pVtx);
		pVtx++;
		Pos.z = static_cast<float>(*pVtx);
		aPos.emplace_back(Pos);

		//���_�C���f�b�N�X�o�^
		Mesh.aIndexBuffer[Index].aIndex.emplace_back(iCnt);
	}

	//�@���擾
	std::vector<dx::XMFLOAT3> aNormal(0);
	FbxArray<FbxVector4> Normals;
	pMesh->GetPolygonVertexNormals(Normals);					//�@�����X�g�擾
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

	//UV���W�擾
	std::vector<dx::XMFLOAT2> aUV;
	FbxStringList uvSetNames;
	pMesh->GetUVSetNames(uvSetNames);								//uvSet�̖��O���X�g�擾
	FbxArray<FbxVector2> uvBuff;
	pMesh->GetPolygonVertexUVs(uvSetNames.GetStringAt(0), uvBuff);	//uvSet�擾
	for (size_t i = 0, Cnt = uvBuff.Size(); i < Cnt; i++) {
		auto puv = &uvBuff[static_cast<int>(i)][0];

		dx::XMFLOAT2 UV{};
		UV.x = static_cast<float>(*puv);
		puv++;
		UV.y = 1.0f - static_cast<float>(*puv);
		aUV.emplace_back(UV);
	}
	if (uvBuff.Size() == 0) {										//UV��񂪂Ȃ��ꍇ
		for (size_t i = 0, Cnt = aNormal.size(); i < Cnt; i++)
			aUV.emplace_back(dx::XMFLOAT2{ 0.0f, 0.0f });
	}

	//�]�@���擾
	std::vector<dx::XMFLOAT3> aBinormal(0);
	if (pMesh->GetElementBinormalCount() > 0) {
		FbxGeometryElementBinormal* pB = pMesh->GetElementBinormal();
		FbxGeometryElement::EMappingMode mapping = pB->GetMappingMode();
		FbxGeometryElement::EReferenceMode reference = pB->GetReferenceMode();
		if (mapping == FbxGeometryElement::EMappingMode::eByControlPoint &&
			reference == FbxGeometryElement::EReferenceMode::eDirect) {			//�}�b�s���O���[�h�ƃ��t�@�����X���[�h�̊m�F

			//�擾����
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

	//�ڐ��擾
	std::vector<dx::XMFLOAT3> aTangent(0);
	if (pMesh->GetElementTangentCount() > 0) {
		FbxGeometryElementTangent* pT = pMesh->GetElementTangent();
		FbxGeometryElement::EMappingMode mapping = pT->GetMappingMode();
		FbxGeometryElement::EReferenceMode reference = pT->GetReferenceMode();
		if (mapping == FbxGeometryElement::EMappingMode::eByControlPoint &&
			reference == FbxGeometryElement::EReferenceMode::eDirect) {			//�}�b�s���O���[�h�ƃ��t�@�����X���[�h�̊m�F

			//�擾����
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

	//�擾���s�̏ꍇ�A�]�@���Ɛڐ����v�Z�ŋ��߂�
	if (aBinormal.size() == 0 || aTangent.size() == 0) {
		aBinormal.resize(0);
		aTangent.resize(0);

		//���ׂẴ|���S�����v�Z
		for (size_t i = 0, Cnt = aPos.size() / 3; i < Cnt; i++) {

			//���_���W�擾
			dx::XMFLOAT3* pPos = &aPos[i * 3];
			VECTOR3 Pos[3];
			Pos[0] = VECTOR3{ pPos->x, pPos->y, pPos->z };
			pPos++;
			Pos[1] = VECTOR3{ pPos->x, pPos->y, pPos->z };
			pPos++;
			Pos[2] = VECTOR3{ pPos->x, pPos->y, pPos->z };

			//�@���擾
			dx::XMFLOAT3* pNormal = &aNormal[i * 3];
			VECTOR3 Normal[3];
			Normal[0] = VECTOR3{ pNormal->x, pNormal->y, pNormal->z };
			pNormal++;
			Normal[1] = VECTOR3{ pNormal->x, pNormal->y, pNormal->z };
			pNormal++;
			Normal[2] = VECTOR3{ pNormal->x, pNormal->y, pNormal->z };

			//UV���W�擾
			dx::XMFLOAT2* pTex = &aUV[i * 3];
			VECTOR2 Tex[3];
			Tex[0] = VECTOR2{ pTex->x, 1.0f - pTex->y };
			pTex++;
			Tex[1] = VECTOR2{ pTex->x, 1.0f - pTex->y };
			pTex++;
			Tex[2] = VECTOR2{ pTex->x, 1.0f - pTex->y };

			//�ڐ������߂�
			VECTOR3 Tangent[3];

			//Tangent��x�����߂�
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

			//Tangent��y�����߂�
			vEdge1.x = Pos[1].y - Pos[0].y;
			vEdge2.x = Pos[2].y - Pos[0].y;
			CrossP = gMath::VecNormalize(gMath::VecCross(vEdge1, vEdge2), false);
			if (CrossP.x == 0.0f)
				CrossP.x = 1.0f;
			Result = -(CrossP.y / CrossP.x);
			for (size_t j = 0; j < 3; j++)
				Tangent[j].y = Result;

			//Tangent��z�����߂�
			vEdge1.x = Pos[1].z - Pos[0].z;
			vEdge2.x = Pos[2].z - Pos[0].z;
			CrossP = gMath::VecNormalize(gMath::VecCross(vEdge1, vEdge2), false);
			if (CrossP.x == 0.0f)
				CrossP.x = 1.0f;
			Result = -(CrossP.y / CrossP.x);
			for (size_t j = 0; j < 3; j++)
				Tangent[j].z = Result;

			//�@���ƒ��s������
			for (size_t j = 0; j < 3; j++) {
				Tangent[j] -= Normal[j] * gMath::VecDot(Tangent[j], Normal[j]);
				Tangent[j] = gMath::VecNormalize(Tangent[j], false);
				aTangent.push_back({ Tangent[j].x, Tangent[j].y, Tangent[j].z });
			}

			//�]�@�������߂�
			VECTOR3 Bitangent[3];
			for (size_t j = 0; j < 3; j++) {
				Bitangent[j] = gMath::VecNormalize(gMath::VecCross(Normal[j], Tangent[j]), false);
				aBinormal.push_back({ Bitangent[j].x, Bitangent[j].y, Bitangent[j].z });
			}

			////TBN�s����C��
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

	//���_���܂Ƃ�
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

	//�}�e���A���擾
	int MaterialNum = pMesh->GetElementMaterialCount();		//�}�e���A�����擾
	if (MaterialNum == 0) {
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}
	FbxLayerElementMaterial* pMaterial = pMesh->GetElementMaterial(0);	//���b�V�����̃}�e���A�����擾
	if (pMaterial == nullptr) {
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}
	int MaterialIndex = pMaterial->GetIndexArray().GetAt(0);			//�}�e���A���̃C���f�b�N�X�擾
	FbxSurfaceMaterial* SurfaceMaterial = pMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(MaterialIndex);
	if (SurfaceMaterial == nullptr) {
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}
	Mesh.MaterialName += SurfaceMaterial->GetName();		//�}�e���A�����擾

	//Lambert��Phong������
	FbxSurfacePhong* Phong{ nullptr };
	FbxSurfaceLambert* Lambert{ nullptr };
	if (SurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))				//Phong�Ƀ_�E���L���X�g
		Phong = (FbxSurfacePhong*)SurfaceMaterial;
	else if (SurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {	//Lambert�Ƀ_�E���L���X�g
		Lambert = (FbxSurfaceLambert*)SurfaceMaterial;
		Mesh.IsPhong = false;
	}
	else {
		Mesh.IsPhong = false;
		m_aMesh.emplace_back(std::move(Mesh));
		return;
	}

	//���ꂼ��̏ꍇ�̏����擾
	if (Phong != nullptr) {

		//����
		Mesh.MaterialData.Ambient.x = static_cast<float>(Phong->Ambient.Get().mData[0]);
		Mesh.MaterialData.Ambient.y = static_cast<float>(Phong->Ambient.Get().mData[1]);
		Mesh.MaterialData.Ambient.z = static_cast<float>(Phong->Ambient.Get().mData[2]);
		Mesh.MaterialData.Ambient.w = static_cast<float>(Phong->AmbientFactor.Get());

		//�g�U���ˌ�
		Mesh.MaterialData.Diffuse.x = static_cast<float>(Phong->Diffuse.Get().mData[0]);
		Mesh.MaterialData.Diffuse.y = static_cast<float>(Phong->Diffuse.Get().mData[1]);
		Mesh.MaterialData.Diffuse.z = static_cast<float>(Phong->Diffuse.Get().mData[2]);
		Mesh.MaterialData.Diffuse.w = static_cast<float>(Phong->DiffuseFactor.Get());

		//���ˌ�
		Mesh.MaterialData.Emissive.x = static_cast<float>(Phong->Emissive.Get().mData[0]);
		Mesh.MaterialData.Emissive.y = static_cast<float>(Phong->Emissive.Get().mData[1]);
		Mesh.MaterialData.Emissive.z = static_cast<float>(Phong->Emissive.Get().mData[2]);
		Mesh.MaterialData.Emissive.w = static_cast<float>(Phong->EmissiveFactor.Get());

		//���ߓx
		Mesh.MaterialData.Transparent.x = static_cast<float>(Phong->TransparentColor.Get().mData[0]);
		Mesh.MaterialData.Transparent.y = static_cast<float>(Phong->TransparentColor.Get().mData[1]);
		Mesh.MaterialData.Transparent.z = static_cast<float>(Phong->TransparentColor.Get().mData[2]);
		Mesh.MaterialData.Transparent.w = static_cast<float>(Phong->TransparencyFactor.Get());

		//���ʔ��ˌ�
		Mesh.MaterialData.Specular.x = static_cast<float>(Phong->Specular.Get().mData[0]);
		Mesh.MaterialData.Specular.y = static_cast<float>(Phong->Specular.Get().mData[1]);
		Mesh.MaterialData.Specular.z = static_cast<float>(Phong->Specular.Get().mData[2]);
		Mesh.MaterialData.Specular.w = static_cast<float>(Phong->SpecularFactor.Get());

		//����
		Mesh.MaterialData.Shininess = static_cast<float>(Phong->Shininess.Get());
	}
	else if (Lambert != nullptr) {

		//����
		Mesh.MaterialData.Ambient.x = static_cast<float>(Lambert->Ambient.Get().mData[0]);
		Mesh.MaterialData.Ambient.y = static_cast<float>(Lambert->Ambient.Get().mData[1]);
		Mesh.MaterialData.Ambient.z = static_cast<float>(Lambert->Ambient.Get().mData[2]);
		Mesh.MaterialData.Ambient.w = static_cast<float>(Lambert->AmbientFactor.Get());

		//�g�U���ˌ�
		Mesh.MaterialData.Diffuse.x = static_cast<float>(Lambert->Diffuse.Get().mData[0]);
		Mesh.MaterialData.Diffuse.y = static_cast<float>(Lambert->Diffuse.Get().mData[1]);
		Mesh.MaterialData.Diffuse.z = static_cast<float>(Lambert->Diffuse.Get().mData[2]);
		Mesh.MaterialData.Diffuse.w = static_cast<float>(Lambert->DiffuseFactor.Get());

		//���ˌ�
		Mesh.MaterialData.Emissive.x = static_cast<float>(Lambert->Emissive.Get().mData[0]);
		Mesh.MaterialData.Emissive.y = static_cast<float>(Lambert->Emissive.Get().mData[1]);
		Mesh.MaterialData.Emissive.z = static_cast<float>(Lambert->Emissive.Get().mData[2]);
		Mesh.MaterialData.Emissive.w = static_cast<float>(Lambert->EmissiveFactor.Get());

		//���ߓx
		Mesh.MaterialData.Transparent.x = static_cast<float>(Lambert->TransparentColor.Get().mData[0]);
		Mesh.MaterialData.Transparent.y = static_cast<float>(Lambert->TransparentColor.Get().mData[1]);
		Mesh.MaterialData.Transparent.z = static_cast<float>(Lambert->TransparentColor.Get().mData[2]);
		Mesh.MaterialData.Transparent.w = static_cast<float>(Lambert->TransparencyFactor.Get());
	}

	//�e�N�X�`���擾
	if (Phong != nullptr)
		GetTexturePath(Mesh, SurfaceMaterial);
	else if (Lambert != nullptr)
		GetTexturePath(Mesh, SurfaceMaterial, false);

	//�e�N�X�`���m�F
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

	//�����擾
	GetSkinData(Mesh, pMesh);

	//�f�[�^�i�[
	m_aMesh.emplace_back(std::move(Mesh));

	//[Lambert]
	//Bump_f4
	//NormalMap_f3
	//DisplacementColor_f4
	//VectorDisplacementColor_f4

	//[Phong]
	//Reflection_f4
}

//�e�N�X�`���p�X�擾
void FBX_LOADER::GetTexturePath(MESH_DATA& Mesh, FbxSurfaceMaterial* pMaterial, bool IsPhong) noexcept
{
	//���ꂼ��̃e�N�X�`�����擾
	if (IsPhong) {

		//����
		GetTextureName(pMaterial, FbxSurfaceMaterial::sAmbient, Mesh.aTex_Ambient, Mesh.aLayerTex_Ambient);

		//�g�U���ˌ�
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDiffuse, Mesh.aTex_Diffuse, Mesh.aLayerTex_Diffuse);

		//���ˌ�
		GetTextureName(pMaterial, FbxSurfaceMaterial::sEmissive, Mesh.aTex_Emissive, Mesh.aLayerTex_Emissive);

		//���ߓx
		GetTextureName(pMaterial, FbxSurfaceMaterial::sTransparentColor, Mesh.aTex_Transparent, Mesh.aLayerTex_Transparent);

		//�m�[�}���}�b�v
		GetTextureName(pMaterial, FbxSurfaceMaterial::sNormalMap, Mesh.aTex_Normal, Mesh.aLayerTex_Normal);

		//�����}�b�v
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDisplacementColor, Mesh.aTex_Displacement, Mesh.aLayerTex_Displacement);

		//���ʔ��ˌ�
		GetTextureName(pMaterial, FbxSurfaceMaterial::sSpecular, Mesh.aTex_Specular, Mesh.aLayerTex_Specular);

		//����
		GetTextureName(pMaterial, FbxSurfaceMaterial::sShininess, Mesh.aTex_Shininess, Mesh.aLayerTex_Shininess);
	}
	else {

		//����
		GetTextureName(pMaterial, FbxSurfaceMaterial::sAmbient, Mesh.aTex_Ambient, Mesh.aLayerTex_Ambient);

		//�g�U���ˌ�
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDiffuse, Mesh.aTex_Diffuse, Mesh.aLayerTex_Diffuse);

		//���ˌ�
		GetTextureName(pMaterial, FbxSurfaceMaterial::sEmissive, Mesh.aTex_Emissive, Mesh.aLayerTex_Emissive);

		//���ߓx
		GetTextureName(pMaterial, FbxSurfaceMaterial::sTransparentColor, Mesh.aTex_Transparent, Mesh.aLayerTex_Transparent);

		//�m�[�}���}�b�v
		GetTextureName(pMaterial, FbxSurfaceMaterial::sNormalMap, Mesh.aTex_Normal, Mesh.aLayerTex_Normal);

		//�����}�b�v
		GetTextureName(pMaterial, FbxSurfaceMaterial::sDisplacementColor, Mesh.aTex_Displacement, Mesh.aLayerTex_Displacement);
	}
}

//�e�N�X�`�����擾
void FBX_LOADER::GetTextureName(FbxSurfaceMaterial* pMaterial, const char* Type, std::vector<std::string>& Tex, std::vector<LAYER_TEX_DATA>& LayerTex) noexcept
{
	FbxProperty Property = pMaterial->FindProperty(Type);
	size_t LayerNum = Property.GetSrcObjectCount<FbxLayeredTexture>();		//LayeredTexture�̖����擾
	if (LayerNum == 0) {													//LayeredTexture�����˒ʏ�e�N�X�`��

		//�e�e�N�X�`���̏����擾
		size_t TextureNum = Property.GetSrcObjectCount<FbxFileTexture>();	//�ʏ�e�N�X�`���̖����m�F
		for (size_t i = 0; i < TextureNum; ++i) {
			FbxFileTexture* Texture = Property.GetSrcObject<FbxFileTexture>(static_cast<int>(i));	//�e�N�X�`���I�u�W�F�N�g�擾
			std::string FileName = "";
			if (Texture != nullptr)
				FileName += Texture->GetRelativeFileName();					//�t�@�C���p�X�擾
			else
				FileName += "Texture Not Found!";

			//�p�X�����˃t�@�C�����i�[
			char Sep1 = '\\';
			char Sep2 = 47;		//��'/'
			std::vector<std::string> aStr1 = gText::Split(FileName, Sep1);
			std::vector<std::string> aStr2 = gText::Split(aStr1[aStr1.size() - 1], Sep2);
			Tex.emplace_back(aStr2[aStr2.size() - 1]);
		}
	}
	else {

		//LayeredTexture����e�N�X�`�����擾
		for (size_t j = 0; j < LayerNum; ++j) {
			FbxLayeredTexture* LayeredTexture = Property.GetSrcObject<FbxLayeredTexture>(static_cast<int>(j));
			LayerTex.emplace_back(LAYER_TEX_DATA());

			//���C�����Ƃ̃e�N�X�`�������擾
			size_t TextureNum = LayeredTexture->GetSrcObjectCount<FbxFileTexture>();	//�e�N�X�`���̖����m�F
			for (size_t i = 0; i < TextureNum; ++i) {
				FbxFileTexture* Texture = Property.GetSrcObject<FbxFileTexture>(static_cast<int>(i));	//�e�N�X�`���I�u�W�F�N�g�擾
				std::string FileName = "";
				if (Texture != nullptr)
					FileName += Texture->GetRelativeFileName();							//�t�@�C���p�X�擾
				else
					FileName += "Texture Not Found!";

				//�p�X�����˃t�@�C�����i�[
				char Sep1 = '\\';
				char Sep2 = 47;		//��'/'
				std::vector<std::string> aStr1 = gText::Split(FileName, Sep1);
				std::vector<std::string> aStr2 = gText::Split(aStr1[aStr1.size() - 1], Sep2);
				LayerTex[j].aTex.emplace_back(aStr2[aStr2.size() - 1]);
			}
		}
	}
}

//�t���[�����Ǎ�
void FBX_LOADER::GetSkinData(MESH_DATA& Mesh, FbxMesh* MeshIn) noexcept
{
	//��O����
	if (m_aSkin.size() == 0)	//�A�j���[�V������񂪂Ȃ�
		return;

	//�O����
	FbxMesh* pMesh = MeshIn;
	FbxNode* pNode = pMesh->GetNode();

	//�ړ��A��]�A�g��̍s��
	FbxVector4 vecT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 vecR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 vecS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix mtxGeometryOffset = FbxAMatrix{ vecT, vecR, vecS };

	//�{�[�����擾
	size_t SkinCnt = pMesh->GetDeformerCount(FbxDeformer::eSkin);	//�X�L���̐��擾
	for (size_t i = 0; i < SkinCnt; ++i) {
		int AnimID = static_cast<int>(i + AnimLoadBaseIndex);		//�A�j���[�V����ID
		size_t MaxFrame = m_aAnimation[AnimID].StopFrame;			//�Đ��t���[����

		//�X�L���擾
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(static_cast<int>(i), FbxDeformer::eSkin);
		size_t ClusterNum = pSkin->GetClusterCount();				//���̐��擾
		for (size_t j = 0; j < ClusterNum; ++j) {

			//��ID�擾
			FbxCluster* pCluster = pSkin->GetCluster(static_cast<int>(j));
			int BoneID{ 0 };
			std::string BoneName = pCluster->GetLink()->GetName();
			for (auto& b : m_aBone) {
				if (b.BoneName == BoneName) {
					BoneID = b.Index;
					Mesh.aBoneID.emplace_back(BoneID);	//�Ή����鍜���L��
					break;
				}
			}

			//�����p��
			FbxAMatrix clusterGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
			FbxAMatrix referenceGlobalInitPosition;
			pCluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= mtxGeometryOffset;
			FbxMatrix clusterRelativeInitPosition;
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
			Mesh.aReferenceGlobalInitPosition.emplace_back(referenceGlobalInitPosition);
			Mesh.aClusterRelativeInitPosition.emplace_back(clusterRelativeInitPosition);	//�����p���s����L��
			if (!m_aBone[BoneID].bMtxIsLoad) {

				//�s�񏉊���
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

			//�t���[���p��
			if (m_aSkin[AnimID].aFrameData[BoneID].aMatrix.size() < MaxFrame) {		//�t���[���p�����o�^�̏ꍇ�ˏ��i�[
				FRAME_DATA FrameData;
				for (size_t k = 0; k < MaxFrame; ++k) {
					FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
					FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
					FbxMatrix clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
					FbxMatrix clusterRelativeCurrentPositionInverse;
					clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

					FbxMatrix vertexTransformMatrix;
					vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

					//�X�L��
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

					//��
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

			//���̑����擾
			size_t IndexNum = pCluster->GetControlPointIndicesCount();	//�����e������C���f�b�N�X�̐�
			int* pIndices = pCluster->GetControlPointIndices();			//���_�C���f�b�N�X�z��
			double* pWeights = pCluster->GetControlPointWeights();		//��d�z��
			for (size_t k = 0; k < IndexNum; ++k) {

				//���_�C���f�b�N�X�擾
				int vtxIndex = pIndices[k];

				//���ԍ��A��d�𒸓_�o�b�t�@���ɓo�^
				Mesh.aIndexBuffer[vtxIndex].aBoneID.emplace_back(BoneID);
				Mesh.aIndexBuffer[vtxIndex].aWeight.emplace_back(static_cast<float>(pWeights[k]));
			}
		}
	}

	//���Ȃ����b�V���̏ꍇ
	if (SkinCnt == 0) {
		for (size_t i = 0, Cnt = m_aSkin.size() - AnimLoadBaseIndex; i < Cnt; ++i) {
			Mesh.aNoSkinData.emplace_back(MESH_DATA::NO_SKIN_MTX{});	//�s��z��m��
			int AnimID = static_cast<int>(i + AnimLoadBaseIndex);		//�A�j���[�V����ID
			size_t MaxFrame = m_aAnimation[AnimID].StopFrame;			//�Đ�����t���[����

			//�t���[���p��
			for (size_t k = 0; k < MaxFrame; ++k) {
				FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
				FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
				//FbxMatrix clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
				//FbxMatrix clusterRelativeCurrentPositionInverse;
				//clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

				//FbxMatrix vertexTransformMatrix;
				//vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

				//�X�L��
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

//�t���[�����Ǎ��i�A�j���[�V�����P�̗p�j
void FBX_LOADER::GetAnimationFromSkin(FbxMesh* MeshIn) noexcept
{
	//�O����
	FbxMesh* pMesh = MeshIn;
	FbxNode* pNode = pMesh->GetNode();

	//�ړ��A��]�A�g��̍s��
	FbxVector4 vecT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 vecR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 vecS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix mtxGeometryOffset = FbxAMatrix{ vecT, vecR, vecS };

	//�{�[�����擾
	size_t SkinCnt = pMesh->GetDeformerCount(FbxDeformer::eSkin);	//�X�L���̐��擾
	for (size_t i = 0; i < SkinCnt; ++i) {
		int AnimID = static_cast<int>(i + AnimLoadBaseIndex);		//�A�j���[�V����ID
		size_t MaxFrame = m_aAnimation[AnimID].StopFrame;			//�Đ��t���[����

		//�X�L���擾
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(static_cast<int>(i), FbxDeformer::eSkin);
		size_t ClusterNum = pSkin->GetClusterCount();				//���̐��擾
		for (size_t j = 0; j < ClusterNum; ++j) {

			//��ID�擾
			FbxCluster* pCluster = pSkin->GetCluster(static_cast<int>(j));
			int BoneID{ 0 };
			std::string BoneName = pCluster->GetLink()->GetName();
			for (auto& b : m_aBone) {
				if (b.BoneName == BoneName) {
					BoneID = b.Index;
					break;
				}
			}

			//�����p��
			FbxAMatrix clusterGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
			FbxAMatrix referenceGlobalInitPosition;
			pCluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= mtxGeometryOffset;
			FbxMatrix clusterRelativeInitPosition;
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
			if (!m_aBone[BoneID].bMtxIsLoad) {

				//�s�񏉊���
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

			//�t���[���p��
			if (m_aSkin[AnimID].aFrameData[BoneID].aMatrix.size() < MaxFrame) {		//�t���[���p�����o�^�̏ꍇ�ˏ��i�[
				FRAME_DATA FrameData;
				for (size_t k = 0; k < MaxFrame; ++k) {
					FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
					FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
					FbxMatrix clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);
					FbxMatrix clusterRelativeCurrentPositionInverse;
					clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

					FbxMatrix vertexTransformMatrix;
					vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

					//�X�L��
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

					//��
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

	//���Ȃ����b�V���̏ꍇ
	if (SkinCnt == 0) {

		//���b�V���m�[�h�m�F
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
			MeshData->aNoSkinData.emplace_back(MESH_DATA::NO_SKIN_MTX{});	//�s��z��m��
			int AnimID = static_cast<int>(i + AnimLoadBaseIndex);			//�A�j���[�V����ID
			size_t MaxFrame = m_aAnimation[AnimID].StopFrame;				//�Đ�����t���[����

			//�t���[���p��
			for (size_t k = 0; k < MaxFrame; ++k) {
				FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
				FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);

				//�s��i�[
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

//�����擾
void FBX_LOADER::GetBoneData() noexcept
{
	//��O����
	size_t BoneCount = m_pScene->GetSrcObjectCount<FbxSkeleton>();
	if (BoneCount == 0)
		return;

	//�Ǎ�����
	for (size_t i = 0; i < BoneCount; ++i) {
		int iCnt = static_cast<int>(i);

		FbxSkeleton* pBone = m_pScene->GetSrcObject<FbxSkeleton>(iCnt);		//�|�C���^�擾
		BONE_DATA Data;
		Data.Index = iCnt;
		Data.BoneName = pBone->GetNode()->GetName();
		m_aBone.emplace_back(Data);
	}

	//�t���[�����p�z��m��
	int BoneNum = static_cast<int>(m_aBone.size());
	for (auto& bs : m_aSkin) {
		if (bs.aFrameData.size() == 0)
			bs.aFrameData.resize(BoneNum);
	}
}

//�A�j���[�V�����擾
void FBX_LOADER::GetAnimationData() noexcept
{
	m_pEvaluator = m_pScene->GetAnimationEvaluator();			//�A�j���[�V�����]���擾
	m_TimeMode = m_pScene->GetGlobalSettings().GetTimeMode();	//�A�j���[�V�������Ԑݒ�擾
	m_Period.SetTime(0, 0, 0, 1, 0, m_TimeMode);				//�t���[�����Ԏ擾

	//�e�C�N���擾
	FbxArray<FbxString*> aTakeName;					//�e�C�N�z��
	m_pScene->FillAnimStackNameArray(aTakeName);
	size_t TakeNum = aTakeName.GetCount();			//�e�C�N���擾
	for (size_t i = 0; i < TakeNum; ++i) {
		int iCnt = static_cast<int>(i);

		//�e�C�N������e�C�N�����擾
		FbxTakeInfo* CurrentTakeInfo = m_pScene->GetTakeInfo(*(aTakeName[iCnt]));
		if (CurrentTakeInfo != nullptr) {
			ANIM_DATA Data;
			Data.Start = CurrentTakeInfo->mLocalTimeSpan.GetStart();		//�J�n����
			Data.Stop = CurrentTakeInfo->mLocalTimeSpan.GetStop();			//�I������
			Data.StartFrame = (int)(Data.Start.Get() / m_Period.Get());		//�J�n�t���[��
			Data.StopFrame = (int)(Data.Stop.Get() / m_Period.Get());		//�I���t���[��
			Data.TakeName = CurrentTakeInfo->mName;							//�e�C�N��

			//�t���[�����m�F
			if (Data.StartFrame < 0) {
				Data.StopFrame -= Data.StartFrame;
				Data.StartFrame = 0;
			}
			if (m_TimeMode == fbxsdk::FbxTime::eFrames30)
				Data.bIsFPS_30 = true;
			m_aAnimation.emplace_back(Data);
		}

		//���������
		aTakeName[iCnt]->Clear();
		SAFE_DELETE(aTakeName[iCnt]);
	}
	aTakeName.Clear();	//���������

	//�����z��ǉ�
	AnimLoadBaseIndex = static_cast<int>(m_aSkin.size());	//�Ǎ��J�n�ʒu�X�V
	size_t BoneNum = m_aAnimation.size() - m_aSkin.size();
	for (size_t i = 0; i < BoneNum; i++)
		m_aSkin.emplace_back(SKIN_DATA{});					//�X�L�����p�z��m��
}

//�A�j���[�V�����Ǎ�
void FBX_LOADER::LoadAnimation()
{
	//��O����
	if (m_aSkin.size() == AnimLoadBaseIndex)
		throw ERROR_EX2("�A�j���[�V�����Ǎ��G���[�F�A�j���[�V������񂪂Ȃ��B");

	//�t���[�����p�z��m��
	int BoneNum = static_cast<int>(m_aBone.size());
	for (auto& bs : m_aSkin) {
		if (bs.aFrameData.size() == 0)
			bs.aFrameData.resize(BoneNum);
	}

	//���b�V������Ǎ��ޏꍇ
	std::vector<FbxMesh*> aMesh(0);
	size_t MeshCount = m_pScene->GetSrcObjectCount<FbxMesh>();
	for (size_t i = 0; i < MeshCount; ++i)
		aMesh.emplace_back(m_pScene->GetSrcObject<FbxMesh>(static_cast<int>(i)));  //���b�V���|�C���^�擾
	if (MeshCount > 0 && MeshCount == m_aMesh.size()) {
		for (auto& m : aMesh)
			GetAnimationFromSkin(m);
		return;
	}

	//���b�V�����Ȃ��ꍇ
	std::vector<FbxNode*> aBone(0);									//���m�[�h�̃|�C���^�z��
	size_t BoneCount = m_pScene->GetSrcObjectCount<FbxSkeleton>();
	if (BoneCount == 0)
		throw ERROR_EX2("�A�j���[�V�����Ǎ��G���[�F���m�[�h���Ȃ��B");
	for (size_t i = 0; i < BoneCount; ++i)
		aBone.push_back(m_pScene->GetSrcObject<FbxSkeleton>(static_cast<int>(i))->GetNode());

	//�A�j���[�V�����̐�����
	for (size_t i = 0, Cnt = m_aSkin.size() - AnimLoadBaseIndex; i < Cnt; ++i) {
		int AnimID = static_cast<int>(i + AnimLoadBaseIndex);	//�A�j���[�V����ID
		size_t MaxFrame = m_aAnimation[AnimID].StopFrame;		//�Đ�����t���[����
		for (auto& m : m_aMesh) {								//���b�V������

			//���Ȃ��̏ꍇ
			if (m.aNoSkinData.size() > 0) {
				m.aNoSkinData.emplace_back(MESH_DATA::NO_SKIN_MTX{});	//�s��z��m��

				//���b�V���m�[�h�m�F
				FbxNode* pNode = nullptr;
				for (auto& node : aMesh) {
					if (m.NodeName == node->GetNode()->GetName()) {
						pNode = node->GetNode();
						break;
					}
				}

				//�t���[���p��
				for (size_t k = 0; k < MaxFrame; ++k) {
					FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
					FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);

					//�s��i�[
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

			//������̏ꍇ
			for (size_t j = 0; j < m.aBoneID.size(); ++j) {		//�Ή����鍜

				//�t���[���p��
				if (m_aSkin[AnimID].aFrameData[m.aBoneID[j]].aMatrix.size() < MaxFrame) {	//�t���[���p�����o�^�̏ꍇ�ˏ��i�[
					FRAME_DATA FrameData;
					for (size_t k = 0; k < MaxFrame; ++k) {
						FbxTime time = m_aAnimation[AnimID].Start + m_Period * static_cast<int>(k);
						//FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(time);
						FbxMatrix clusterGlobalCurrentPosition = aBone[m.aBoneID[j]]->EvaluateGlobalTransform(time);
						FbxMatrix clusterRelativeCurrentPositionInverse;
						clusterRelativeCurrentPositionInverse = clusterGlobalCurrentPosition;

						FbxMatrix vertexTransformMatrix;
						vertexTransformMatrix = clusterRelativeCurrentPositionInverse * m.aClusterRelativeInitPosition[j];

						//�X�L��
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

						//��
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

//�t�@�C�����o��
void FBX_LOADER::SaveModelData(const char* FileName) noexcept
{
	//�t�@�C���p�X
	std::string Path = "Asset/Model/Output/";
	Path += FileName;
	std::ostringstream oss;

	//���f����񏑏o��
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

	//���b�V�����i���X�g�j���o��
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

	//���b�V�����i�ڍׁj���o��
	for (size_t i = 0; i < ModelBin.MeshNum; i++) {			//���b�V������

		//VS�f�[�^�쐬
		VS_DATA<VERTEX_M>& DataRef = m_aMesh[i].vsData;
		VS_DATA<VERTEX_MB> vsData;
		vsData.m_Indices = DataRef.m_Indices;				//�C���f�b�N�X
		for (auto& v : DataRef.m_Vertices) {				//���_���

			//���_���
			VERTEX_MB Vertex;
			Vertex.m_Pos = v.m_Pos;
			Vertex.m_UV = v.m_UV;
			Vertex.m_Normal = v.m_Normal;
			Vertex.m_Binormal = v.m_Binormal;
			Vertex.m_Tangent = v.m_Tangent;
			vsData.m_Vertices.emplace_back(Vertex);
		}

		//����d
		for (auto& idx : m_aMesh[i].aIndexBuffer) {			//���_�o�b�t�@�C���f�b�N�X�z��

			//�Ή����钸�_�֔��f
			size_t BoneNum = idx.aBoneID.size();			//���_���Ƃ̍��̐�
			for (auto& idxNum : idx.aIndex) {

				//�����e�����Ȃ��ꍇ
				if (BoneNum == 0) {
					vsData.m_Vertices[idxNum].m_BoneWeight[0] = 1.0f;
					continue;
				}

				//����d���i�[
				for (size_t k = 0; k < 4; k++) {			//4�ȏ�̍���j��
					if (k >= BoneNum)						//��O�����i����4�����̏ꍇ�j
						break;
					vsData.m_Vertices[idxNum].m_BoneID[k] = idx.aBoneID[k];
					vsData.m_Vertices[idxNum].m_BoneWeight[k] = idx.aWeight[k];
				}
			}
		}

		//�t�@�C�����o���i���b�V�����j
		oss << Path << "_Mesh" << i << "_Name.bin";
		FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].NodeName);
		oss.str("");

		//�t�@�C�����o���i�C���f�b�N�X�j
		oss << Path << "_Mesh" << i << "_Idx.bin";
		FILE_IO::SaveFile(oss.str().c_str(), vsData.m_Indices);
		oss.str("");

		//�t�@�C�����o���i���_���j
		oss << Path << "_Mesh" << i << "_Vtx.bin";
		FILE_IO::SaveFile(oss.str().c_str(), vsData.m_Vertices);
		oss.str("");

		//�t�@�C�����o���i�}�e���A���j
		oss << Path << "_Mesh" << i << "_Material.bin";
		FILE_IO::SaveFile(oss.str().c_str(), &m_aMesh[i].MaterialData);
		oss.str("");

		//�t�@�C�����o���i�e�N�X�`��D�j
		oss << Path << "_Mesh" << i << "_TexD.bin";
		if (m_aMesh[i].aTex_Diffuse.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Diffuse[0]);
		oss.str("");

		//�t�@�C�����o���i�e�N�X�`��S�j
		oss << Path << "_Mesh" << i << "_TexS.bin";
		if (m_aMesh[i].aTex_Specular.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Specular[0]);
		oss.str("");

		//�t�@�C�����o���i�e�N�X�`��N�j
		oss << Path << "_Mesh" << i << "_TexN.bin";
		if (m_aMesh[i].aTex_Normal.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Normal[0]);
		oss.str("");

		//�t�@�C�����o���i�e�N�X�`��Disp�j
		oss << Path << "_Mesh" << i << "_TexDisp.bin";
		if (m_aMesh[i].aTex_Displacement.size() > 0)
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aTex_Displacement[0]);
		oss.str("");
	}

	//�����i���X�g�j���o��
	std::vector<BONE_BIN> aBoneBin(ModelBin.BoneNum);
	for (size_t i = 0; i < ModelBin.BoneNum; i++)		//������
		aBoneBin[i].NameSize = static_cast<int>(m_aBone[i].BoneName.size());
	oss << Path << "_Bone.bin";
	FILE_IO::SaveFile(oss.str().c_str(), aBoneBin);
	oss.str("");

	//�����i�ڍׁj���o��
	for (size_t i = 0; i < ModelBin.BoneNum; i++) {

		//�t�@�C�����o���i�����j
		oss << Path << "_Bone" << i << "_Name.bin";
		FILE_IO::SaveFile(oss.str().c_str(), m_aBone[i].BoneName);
		oss.str("");

		//�t�@�C�����o���i�����p���j
		oss << Path << "_Bone" << i << "_Mtx.bin";
		FILE_IO::SaveFile(oss.str().c_str(), &m_aBone[i].InitMatrix);
		oss.str("");
	}

	//���Ȃ����b�V���ԍ����o��
	std::vector<int> aNoSkinIndex(0);
	for (size_t i = 0, Cnt = m_aMesh.size(); i < Cnt; i++) {
		if (m_aMesh[i].aNoSkinData.size() > 0)
			aNoSkinIndex.emplace_back(static_cast<int>(i));
	}
	oss << Path << "_NoSkin.bin";
	FILE_IO::SaveFile(oss.str().c_str(), aNoSkinIndex);
	oss.str("");
}

//�t�@�C�����o���i�A�j���[�V�����j
void FBX_LOADER::SaveAnimData(const char* FileName, int AnimID) noexcept
{
	//��O����
	if (m_aAnimation.size() <= AnimID)
		return;

	//�t�@�C���p�X
	std::string Path = "Asset/Model/Output/Animation/Anim_";
	Path += FileName;
	std::ostringstream oss;

	//��񏑏o���i���X�g�j
	ANIM_BIN AnimBin;
	AnimBin.FrameNum = m_aAnimation[AnimID].StopFrame - m_aAnimation[AnimID].StartFrame;
	oss << Path << ".bin";
	FILE_IO::SaveFile(oss.str().c_str(), &AnimBin);
	oss.str("");

	//��񏑏o���i�ڍׁj
	for (size_t i = 0, Cnt = m_aBone.size(); i < Cnt; i++) {	//������
		auto FrameData = &m_aSkin[AnimID].aFrameData[i];

		//���p��
		oss << Path << "_BoneMtx" << i << ".bin";
		FILE_IO::SaveFile(oss.str().c_str(), FrameData->aBoneMatrix);
		oss.str("");

		//�t���[���p��
		oss << Path << "_FrameMtx" << i << ".bin";
		FILE_IO::SaveFile(oss.str().c_str(), FrameData->aMatrix);
		oss.str("");
	}

	//��񏑏o���i���Ȃ��j
	for (size_t i = 0, Cnt = m_aMesh.size(); i < Cnt; i++) {	//���b�V������

		//���Ȃ����b�V���m�F
		if (m_aMesh[i].aNoSkinData.size() > 0) {

			//�t���[���p��
			oss << Path << "_NoSkinMtx" << i << ".bin";
			FILE_IO::SaveFile(oss.str().c_str(), m_aMesh[i].aNoSkinData[AnimID].aMatrix);
			oss.str("");
		}
	}
}
