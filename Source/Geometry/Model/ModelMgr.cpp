
//===== �C���N���[�h�� =====
#include <Geometry/Model/ModelMgr.h>
#include <Geometry/Model/ModelRef.h>
#include <Tool/FileIO.h>

namespace dx = DirectX;

//===== �ÓI�����o =====
std::string MODEL_MGR::aModelName[static_cast<int>(MODEL_ID::ID_Max)] = {
	"Tarantella",
	"UnityChan"
};

std::vector<std::string> MODEL_MGR::aAnimName[static_cast<int>(MODEL_ID::ID_Max)] = {
	{"Idle", "Walk", "Run"},
	{"Jump", "Run"}
};

std::vector<bool> MODEL_MGR::aAnimFPS_30[static_cast<int>(MODEL_ID::ID_Max)] = {
	{true, false, false},
	{true, true}
};

//===== �N���X���� =====
MODEL_MGR::MODEL_MGR() : m_aModelPackPtr(static_cast<int>(MODEL_ID::ID_Max))
{
	//���f���p�b�N�z�񏉊���
	for (auto& m : m_aModelPackPtr)
		m = std::make_unique<ModelRef::MODEL_PACK>();

	//�t�@�C���Ǎ�
	for (int i = 0, iCnt = static_cast<int>(MODEL_ID::ID_Max); i < iCnt; i++) {

		//���f���Ǎ�
		LoadModel(static_cast<MODEL_ID>(i));

		//�e�N�X�`���Ǎ�
		std::vector<ModelRef::TEX_PACK>& TexPack = m_aModelPackPtr[i]->m_aTexPack;
		std::string Path = "Asset/Model/";
		Path += aModelName[i];
		Path += "/Tex/";
		std::ostringstream oss;
		if (TexPack.size() > 0) {
			for (size_t j = 0, jCnt = TexPack.size(); j < jCnt; j++) {
				oss << Path << TexPack[j].Name;
				TexPack[j].TexData = TEX_LOADER::LoadTexture(oss.str().c_str());
				oss.str("");
			}
		}
	}

	//���[���h�s�񏉊���
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
}

MODEL_MGR::~MODEL_MGR() noexcept
{
	//�e�N�X�`�����
	for (int i = 0, iCnt = static_cast<int>(MODEL_ID::ID_Max); i < iCnt; i++) {
		std::vector<ModelRef::TEX_PACK>& TexPack = m_aModelPackPtr[i]->m_aTexPack;
		for (size_t j = 0, jCnt = TexPack.size(); j < jCnt; j++)
			TEX_LOADER::ReleaseTexture(TexPack[j].TexData.pImageData);
	}
}

//���f���Ǎ�
void MODEL_MGR::LoadModel(MODEL_ID id) const
{
	int ModelID = static_cast<int>(id);
	ModelRef::MODEL_PACK& Model = *m_aModelPackPtr[ModelID];

	//�t�@�C���p�X�i���f���j
	std::string Name = aModelName[ModelID];
	std::string Path = "Asset/Model/";
	Path += Name;
	Path += "/";
	Path += Name;
	std::ostringstream oss;

	//���f�����Ǎ�
	MODEL_BIN ModelBin;
	oss << Path << ".bin";
	FILE_IO::LoadFile(oss.str().c_str(), &ModelBin);
	oss.str("");
	Model.aMesh.resize(ModelBin.MeshNum);
	Model.aBone.resize(ModelBin.BoneNum);
	Model.aNoSkinIndex.resize(ModelBin.NoSkinNum);

	//���b�V�����i���X�g�j�Ǎ�
	std::vector<MESH_BIN> aMeshBin(ModelBin.MeshNum);
	oss << Path << "_Mesh.bin";
	FILE_IO::LoadFile(oss.str().c_str(), aMeshBin);
	oss.str("");
	for (int i = 0; i < ModelBin.MeshNum; i++) {
		Model.aMesh[i].Name.resize(aMeshBin[i].NameSize);
		Model.aMesh[i].vsData.m_Indices.resize(aMeshBin[i].VertexNum);
		Model.aMesh[i].vsData.m_Vertices.resize(aMeshBin[i].VertexNum);
		Model.aMesh[i].Tex_D.resize(aMeshBin[i].NameSize_Diffuse);
		Model.aMesh[i].Tex_S.resize(aMeshBin[i].NameSize_Specular);
		Model.aMesh[i].Tex_N.resize(aMeshBin[i].NameSize_Normal);
		Model.aMesh[i].Tex_Disp.resize(aMeshBin[i].NameSize_Displacement);
	}

	//���b�V�����i�ڍׁj�Ǎ�
	for (int i = 0; i < ModelBin.MeshNum; i++) {	//���b�V������

		//�t�@�C���Ǎ��i���b�V�����j
		oss << Path << "_Mesh" << i << "_Name.bin";
		FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].Name);
		oss.str("");

		//�t�@�C���Ǎ��i�C���f�b�N�X�j
		oss << Path << "_Mesh" << i << "_Idx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].vsData.m_Indices);
		oss.str("");

		//�t�@�C���Ǎ��i���_���j
		oss << Path << "_Mesh" << i << "_Vtx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].vsData.m_Vertices);
		oss.str("");

		//�t�@�C���Ǎ��i�}�e���A���j
		oss << Path << "_Mesh" << i << "_Material.bin";
		FILE_IO::LoadFile(oss.str().c_str(), &Model.aMesh[i].MaterialData);
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��D�j
		oss << Path << "_Mesh" << i << "_TexD.bin";
		if (Model.aMesh[i].Tex_D.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].Tex_D);
			LoadTextureName(Model.aMesh[i].Tex_D, Model.m_aTexPack);		//�e�N�X�`���o�^
		}
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��S�j
		oss << Path << "_Mesh" << i << "_TexS.bin";
		if (Model.aMesh[i].Tex_S.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].Tex_S);
			LoadTextureName(Model.aMesh[i].Tex_S, Model.m_aTexPack);		//�e�N�X�`���o�^
		}
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��N�j
		oss << Path << "_Mesh" << i << "_TexN.bin";
		if (Model.aMesh[i].Tex_N.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].Tex_N);
			LoadTextureName(Model.aMesh[i].Tex_N, Model.m_aTexPack);		//�e�N�X�`���o�^
		}
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��Disp�j
		oss << Path << "_Mesh" << i << "_TexDisp.bin";
		if (Model.aMesh[i].Tex_Disp.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), Model.aMesh[i].Tex_Disp);
			LoadTextureName(Model.aMesh[i].Tex_Disp, Model.m_aTexPack);		//�e�N�X�`���o�^
		}
		oss.str("");
	}

	//�����i���X�g�j�Ǎ�
	std::vector<BONE_BIN> aBoneBin(ModelBin.BoneNum);
	oss << Path << "_Bone.bin";
	FILE_IO::LoadFile(oss.str().c_str(), aBoneBin);
	oss.str("");
	for (int i = 0; i < ModelBin.BoneNum; i++)   //������
		Model.aBone[i].BoneName.resize(aBoneBin[i].NameSize);

	//�����i�ڍׁj�Ǎ�
	for (int i = 0; i < ModelBin.BoneNum; i++) {

		//�t�@�C���Ǎ��i�����j
		oss << Path << "_Bone" << i << "_Name.bin";
		FILE_IO::LoadFile(oss.str().c_str(), Model.aBone[i].BoneName);
		oss.str("");

		//�t�@�C���Ǎ��i�����p���j
		oss << Path << "_Bone" << i << "_Mtx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), &Model.aBone[i].InitMatrix);
		oss.str("");
	}

	//���Ȃ����b�V���ԍ��Ǎ�
	oss << Path << "_NoSkin.bin";
	FILE_IO::LoadFile(oss.str().c_str(), Model.aNoSkinIndex);
	oss.str("");

	//�A�j���[�V�����Ǎ�
	for (size_t i = 0, Cnt = aAnimName[ModelID].size(); i < Cnt; i++) {

		//FPS��񏑍���
		Model.aIsFPS_30.emplace_back(aAnimFPS_30[ModelID][i]);

		//�t�@�C���p�X
		Path = "Asset/Model/";
		Path += Name;
		Path += "/Anim/Anim_";
		Path += aAnimName[ModelID][i];
		Model.aAnimName.emplace_back(aAnimName[ModelID][i]);

		//���Ǎ��i���X�g�j
		ANIM_BIN AnimBin;
		oss << Path << ".bin";
		FILE_IO::LoadFile(oss.str().c_str(), &AnimBin);
		oss.str("");
		Model.aAnimFrame.emplace_back(AnimBin.FrameNum);
		ModelRef::ANIM_PACK AnimPack;
		AnimPack.aMatrix.resize(AnimBin.FrameNum);
		for (auto& b : Model.aBone) {
			b.aBoneSkin.emplace_back(AnimPack);
			b.aSkin.emplace_back(AnimPack);
		}

		//���Ǎ��i�ڍׁj
		for (int j = 0; j < ModelBin.BoneNum; j++) {     //������
			auto pBonePack = &Model.aBone[j];

			//���p��
			oss << Path << "_BoneMtx" << j << ".bin";
			FILE_IO::LoadFile(oss.str().c_str(), pBonePack->aBoneSkin[i].aMatrix);
			oss.str("");

			//�t���[���p��
			oss << Path << "_FrameMtx" << j << ".bin";
			FILE_IO::LoadFile(oss.str().c_str(), pBonePack->aSkin[i].aMatrix);
			oss.str("");

			//�]�u�s��v�Z
			for (auto& mtx : pBonePack->aSkin[i].aMatrix)
				gMath::MtxTranspose4x4_SSE(&mtx._11);
		}

		//���Ǎ��i���Ȃ����b�V���j
		for (size_t j = 0, jCnt = Model.aNoSkinIndex.size(); j < jCnt; j++) {
			auto pMeshPack = &Model.aMesh[Model.aNoSkinIndex[j]];
			pMeshPack->aNoSkin.emplace_back(AnimPack);

			//�t���[���p��
			oss << Path << "_NoSkinMtx" << j << ".bin";
			FILE_IO::LoadFile(oss.str().c_str(), pMeshPack->aNoSkin[i].aMatrix);
			oss.str("");

			//�]�u�s��v�Z
			for (auto& mtx : pMeshPack->aNoSkin[i].aMatrix)
				gMath::MtxTranspose4x4_SSE(&mtx._11);
		}
	}
}

//�e�N�X�`�����Ǎ�
void MODEL_MGR::LoadTextureName(std::string TexName, std::vector<ModelRef::TEX_PACK>& DataRef) const noexcept
{
	//���������e�N�X�`���z��Ƀf�[�^���Ȃ�
	if (DataRef.size() < 1) {
		ModelRef::TEX_PACK Data;
		Data.Name = TexName;
		DataRef.emplace_back(Data);
	}
	else {
		for (size_t j = 0, Cnt = DataRef.size(); j < Cnt; j++) {

			//�����e�N�X�`�������݂���ꍇ
			if (DataRef[j].Name == TexName)
				break;

			//�e�N�X�`�������݂��Ȃ��ꍇ
			if (j == Cnt - 1) {
				ModelRef::TEX_PACK Data;
				Data.Name = TexName;
				DataRef.emplace_back(Data);
			}
		}
	}
}
