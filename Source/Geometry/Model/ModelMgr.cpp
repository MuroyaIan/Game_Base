
//===== �C���N���[�h�� =====
#include <Geometry/Model/ModelMgr.h>
#include <Geometry/Model/ModelRef.h>
#include <Tool/FileIO.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== �ÓI�����o =====
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

//===== �\���̎��� =====
MODEL_MGR::TEX_PACK::TEX_PACK() noexcept : aName(0), aTexData(0), aTexBuffPtr(0), AnimBuffPtr(), aUsedCount(0)
{}

MODEL_MGR::TEX_PACK::~TEX_PACK() noexcept
{}

//===== �N���X���� =====
MODEL_MGR::MODEL_MGR(CT_GRAPHIC& Gfx) :
	m_aModelPackPtr(static_cast<int>(MODEL_ID::ID_Max)), m_aTexPack(static_cast<int>(MODEL_ID::ID_Max)), m_DX(Gfx)
{
	//���f���p�b�N�z�񏉊���
	for (auto& m : m_aModelPackPtr)
		m = std::make_unique<ModelRef::MODEL_PACK>();

	//�t�@�C���Ǎ�
	if (m_aTexPack.size() > 0) {

		//�Ǎ����[�v
		auto pModelName = &aModelName[0];
		for (int i = 0, iCnt = static_cast<int>(MODEL_ID::ID_Max); i < iCnt; i++) {

			//���f���Ǎ�
			LoadModel(static_cast<MODEL_ID>(i));

			//�e�N�X�`���Ǎ�
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

			//�|�C���^���Z
			pModelName++;
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
	dx::XMStoreFloat4x4(&m_aModelPackPtr[static_cast<int>(MODEL_ID::Cube)]->InitMtxWorld,
		dx::XMMatrixScaling(0.5f, 0.5f, 0.5f)
		* dx::XMMatrixRotationRollPitchYaw(0.0f, gMath::GetRad(180), 0.0f)
		* dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
	);
}

MODEL_MGR::~MODEL_MGR() noexcept
{
	//�e�N�X�`�����
	if (m_aTexPack.size() > 0) {

		//������[�v
		TEX_PACK* pPack = &m_aTexPack[0];
		for (int i = 0, iCnt = static_cast<int>(MODEL_ID::ID_Max); i < iCnt; i++) {

			//��O����
			if (pPack->aTexData.size() == 0)
				continue;

			//���f�����Ƃ�
			TEX_LOADER::TEX_DATA* pTex = &pPack->aTexData[0];
			for (auto& p : pPack->aTexBuffPtr) {

				//�o�b�t�@���
				if (p != nullptr)
					p.reset();

				//���������
				if (pTex != nullptr) {
					TEX_LOADER::ReleaseTexture(pTex->pImageData);
					pTex->pImageData = nullptr;
				}

				//�|�C���^���Z
				pTex++;
			}

			//�|�C���^���Z
			pPack++;
		}
	}
}

//�e�N�X�`���o�b�t�@���p�J�n�E�I��
ID3D11ShaderResourceView* MODEL_MGR::SetTextureOn(MODEL_ID id, std::string TexName)
{
	TEX_PACK& TexPack = m_aTexPack[static_cast<int>(id)];
	for (size_t i = 0, Cnt = TexPack.aName.size(); i < Cnt; i++) {
		if (TexPack.aName[i] == TexName) {

			//�Q�Ɛ��X�V
			TexPack.aUsedCount[i]++;

			//�o�b�t�@���쐬�̏ꍇ�ˍ쐬
			if (TexPack.aTexBuffPtr[i] == nullptr)
				TexPack.aTexBuffPtr[i] = std::make_unique<TEXTURE>(m_DX, TexPack.aTexData[i]);

			//�Y������SRV�|�C���^��Ԃ�
			return TexPack.aTexBuffPtr[i]->GetSrvPtr();
		}
	}

	//��O����
	std::ostringstream oss;
	oss << "�Y������e�N�X�`��������܂��� : " << aModelName[static_cast<int>(id)] << "->" << TexName;
	throw ERROR_EX2(oss.str().c_str());
}

void MODEL_MGR::SetTextureOff(MODEL_ID id, std::string TexName)
{
	TEX_PACK& TexPack = m_aTexPack[static_cast<int>(id)];
	for (size_t i = 0, Cnt = TexPack.aName.size(); i < Cnt; i++) {
		if (TexPack.aName[i] == TexName) {

			//�Q�Ɛ��X�V
			TexPack.aUsedCount[i]--;
			if (TexPack.aUsedCount[i] < 0)
				TexPack.aUsedCount[i] = 0;

			//�Q�Ɛ������˃o�b�t�@���
			if (TexPack.aUsedCount[i] == 0 &&
				TexPack.aTexBuffPtr[i] != nullptr)
				TexPack.aTexBuffPtr[i].reset();

			return;
		}
	}

	//��O����
	std::ostringstream oss;
	oss << "�Y������e�N�X�`��������܂��� : " << aModelName[static_cast<int>(id)] << "->" << TexName;
	throw ERROR_EX2(oss.str().c_str());
}

//�A�j���[�V�����o�b�t�@���p�J�n�E�I��
ID3D11ShaderResourceView* MODEL_MGR::SetAnimTexOn(MODEL_ID id)
{
	if (m_aTexPack[static_cast<int>(id)].AnimBuffPtr == nullptr) {

		//�Q�ƃf�[�^���擾
		int ModelID = static_cast<int>(id);
		ModelRef::MODEL_PACK& Model = *m_aModelPackPtr[ModelID];

		//������Ǎ�
		std::vector<dx::XMFLOAT4X4> aMtxBone(0);
		for (auto& bd : Model.aBone) {
			for (auto& ad : bd.aSkin) {
				for (auto& md : ad.aMatrix)
					aMtxBone.push_back(md);
			}
		}

		//�e�N�X�`���쐬�p�f�[�^
		TEX_DATA_ANIM AnimData;
		AnimData.pAnimData = &aMtxBone.data()->_11;
		for (auto& f : Model.aAnimFrame)
			AnimData.nWidth += f;
		AnimData.nHeight = static_cast<int>(Model.aBone.size());

		//�o�b�t�@�쐬
		m_aTexPack[static_cast<int>(id)].AnimBuffPtr = std::make_unique<TEXTURE_ANIM>(m_DX, AnimData);
	}

	//�Y������SRV�|�C���^��Ԃ�
	return m_aTexPack[static_cast<int>(id)].AnimBuffPtr->GetSrvPtr();
}

void MODEL_MGR::SetAnimTexOff(MODEL_ID id) noexcept
{
	//�o�b�t�@���
	if (m_aTexPack[static_cast<int>(id)].AnimBuffPtr != nullptr)
		m_aTexPack[static_cast<int>(id)].AnimBuffPtr.reset();
}

//���f���Ǎ�
void MODEL_MGR::LoadModel(MODEL_ID id)
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
	if (ModelBin.MeshNum == 0) {
		std::ostringstream ossError;
		ossError << Name << "�F���f���̃��b�V������0�ł��B";
		throw ERROR_EX2(ossError.str().c_str());
	}
	Model.aMesh.resize(ModelBin.MeshNum);
	Model.aBone.resize(ModelBin.BoneNum);
	Model.aNoSkinIndex.resize(ModelBin.NoSkinNum);

	//���b�V�����i���X�g�j�Ǎ�
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

		//�|�C���^���Z
		pMeshBin++;
	}

	//���b�V�����i�ڍׁj�Ǎ�
	ModelRef::MESH_PACK* pMesh = &Model.aMesh[0];
	for (int i = 0; i < ModelBin.MeshNum; i++) {	//���b�V������

		//�t�@�C���Ǎ��i���b�V�����j
		oss << Path << "_Mesh" << i << "_Name.bin";
		FILE_IO::LoadFile(oss.str().c_str(), pMesh->Name);
		oss.str("");

		//�t�@�C���Ǎ��i�C���f�b�N�X�j
		oss << Path << "_Mesh" << i << "_Idx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), pMesh->vsData.m_Indices);
		oss.str("");

		//�t�@�C���Ǎ��i���_���j
		oss << Path << "_Mesh" << i << "_Vtx.bin";
		FILE_IO::LoadFile(oss.str().c_str(), pMesh->vsData.m_Vertices);
		oss.str("");

		//�t�@�C���Ǎ��i�}�e���A���j
		oss << Path << "_Mesh" << i << "_Material.bin";
		FILE_IO::LoadFile(oss.str().c_str(), &pMesh->MaterialData);
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��D�j
		oss << Path << "_Mesh" << i << "_TexD.bin";
		if (pMesh->Tex_D.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_D);
			LoadTextureName(pMesh->Tex_D, m_aTexPack[ModelID]);		//�e�N�X�`���o�^
		}
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��S�j
		oss << Path << "_Mesh" << i << "_TexS.bin";
		if (pMesh->Tex_S.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_S);
			LoadTextureName(pMesh->Tex_S, m_aTexPack[ModelID]);		//�e�N�X�`���o�^
		}
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��N�j
		oss << Path << "_Mesh" << i << "_TexN.bin";
		if (pMesh->Tex_N.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_N);
			LoadTextureName(pMesh->Tex_N, m_aTexPack[ModelID]);		//�e�N�X�`���o�^
		}
		oss.str("");

		//�t�@�C���Ǎ��i�e�N�X�`��Disp�j
		oss << Path << "_Mesh" << i << "_TexDisp.bin";
		if (pMesh->Tex_Disp.size() > 0) {
			FILE_IO::LoadFile(oss.str().c_str(), pMesh->Tex_Disp);
			LoadTextureName(pMesh->Tex_Disp, m_aTexPack[ModelID]);	//�e�N�X�`���o�^
		}
		oss.str("");

		//�|�C���^���Z
		pMesh++;
	}

	//�����i���X�g�j�Ǎ�
	std::vector<BONE_BIN> aBoneBin(ModelBin.BoneNum);
	oss << Path << "_Bone.bin";
	FILE_IO::LoadFile(oss.str().c_str(), aBoneBin);
	oss.str("");
	if (ModelBin.BoneNum > 0) {
		BONE_BIN* pBoneBin = &aBoneBin[0];
		for (auto& b : Model.aBone) {				//������
			b.BoneName.resize(pBoneBin->NameSize);
			pBoneBin++;
		}
	}

	//�����i�ڍׁj�Ǎ�
	if (ModelBin.BoneNum > 0) {
		ModelRef::BONE_PACK* pBone = &Model.aBone[0];
		for (int i = 0; i < ModelBin.BoneNum; i++) {

			//�t�@�C���Ǎ��i�����j
			oss << Path << "_Bone" << i << "_Name.bin";
			FILE_IO::LoadFile(oss.str().c_str(), pBone->BoneName);
			oss.str("");

			//�t�@�C���Ǎ��i�����p���j
			oss << Path << "_Bone" << i << "_Mtx.bin";
			FILE_IO::LoadFile(oss.str().c_str(), &pBone->InitMatrix);
			oss.str("");

			//�|�C���^���Z
			pBone++;
		}
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
		Model.aAnimName.push_back(aAnimName[ModelID][i]);

		//���Ǎ��i���X�g�j
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

		//���Ǎ��i�ڍׁj
		if (ModelBin.BoneNum > 0) {
			auto pBonePack = &Model.aBone[0];
			for (int j = 0; j < ModelBin.BoneNum; j++) {	//������

				//���p��
				oss << Path << "_BoneMtx" << j << ".bin";
				FILE_IO::LoadFile(oss.str().c_str(), pBonePack->aBoneSkin[i].aMatrix);
				oss.str("");

				//�t���[���p��
				oss << Path << "_FrameMtx" << j << ".bin";
				FILE_IO::LoadFile(oss.str().c_str(), pBonePack->aSkin[i].aMatrix);
				oss.str("");

				//�|�C���^���Z
				pBonePack++;
			}
		}

		//���Ǎ��i���Ȃ����b�V���j
		if (ModelBin.NoSkinNum > 0) {
			auto pIdx = &Model.aNoSkinIndex[0];
			for (int j = 0; j < ModelBin.NoSkinNum; j++) {
				auto pMeshPack = &Model.aMesh[*pIdx];
				pMeshPack->aNoSkin.push_back(AnimPack);

				//�t���[���p��
				oss << Path << "_NoSkinMtx" << j << ".bin";
				FILE_IO::LoadFile(oss.str().c_str(), pMeshPack->aNoSkin[i].aMatrix);
				oss.str("");

				//�|�C���^���Z
				pIdx++;
			}
		}
	}
}

//�e�N�X�`�����Ǎ�
void MODEL_MGR::LoadTextureName(std::string TexName, TEX_PACK& DataRef) const noexcept
{
	//���������e�N�X�`���z��Ƀf�[�^���Ȃ�
	if (DataRef.aName.size() < 1)
		DataRef.aName.push_back(TexName);
	else {
		auto pName = &DataRef.aName[0];
		for (size_t j = 0, Cnt = DataRef.aName.size(); j < Cnt; j++) {

			//�����e�N�X�`�������݂���ꍇ
			if (*pName == TexName)
				break;

			//�e�N�X�`�������݂��Ȃ��ꍇ
			if (j == Cnt - 1)
				DataRef.aName.push_back(TexName);

			//�|�C���^���Z
			pName++;
		}
	}
}
