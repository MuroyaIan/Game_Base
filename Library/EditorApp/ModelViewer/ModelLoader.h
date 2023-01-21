/**
 * @file ModelLoader.h
 * @brief FBX���[�_
 * @author ���J�C�A��
 * @date 2022/08/21
 * @���� 2022/08/21�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <3rd_Party/FBXSDK/include/fbxsdk.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== �N���X�錾 =====

//***** FBX���[�_ *****
class FBX_LOADER
{
public:

	//FBX�f�[�^
	struct FBX_DATA
	{
		FbxNode* Node;									//�m�[�h
		std::string NodeName;							//�m�[�h��
		std::vector<FbxNodeAttribute*> m_pAttributes;	//����
		std::vector<std::string> m_pAttributesType;		//�����^�C�v��

		FBX_DATA() noexcept : Node(nullptr), NodeName(""), m_pAttributes(0), m_pAttributesType(0)
		{}
		~FBX_DATA() noexcept {}
	};

	//���_�C���f�b�N�X
	struct VERTEX_INDEX
	{
		std::vector<int> aIndex;			//�C���f�b�N�X�z��
		std::vector<int> aBoneID;			//���ԍ��̔z��
		std::vector<float> aWeight;			//����d�̔z��

		VERTEX_INDEX() noexcept : aIndex(0), aBoneID(0), aWeight(0)
		{}
		~VERTEX_INDEX() noexcept {}
	};

	//���C���e�N�X�`�����
	struct LAYER_TEX_DATA
	{
		std::vector<std::string> aTex;	//�e�N�X�`����

		LAYER_TEX_DATA() noexcept : aTex(0)
		{}
		~LAYER_TEX_DATA() noexcept {}
	};

	//MESH�f�[�^
	struct MESH_DATA
	{
		std::string NodeName;									//�m�[�h��
		std::vector<VERTEX_INDEX> aIndexBuffer;					//���_�o�b�t�@�C���f�b�N�X
		VS_DATA<VERTEX_M> vsData;								//���_���
		std::string MaterialName;								//�}�e���A����
		CBD_MATERIAL MaterialData;								//�}�e���A�����
		bool IsPhong;											//Phong���f���t���O
		std::vector<std::string> aTex_Ambient;					//�e�N�X�`���F����
		std::vector<std::string> aTex_Diffuse;					//�e�N�X�`���F�g�U���ˌ�
		std::vector<std::string> aTex_Emissive;					//�e�N�X�`���F���ˌ�
		std::vector<std::string> aTex_Transparent;				//�e�N�X�`���F���ߓx
		std::vector<std::string> aTex_Specular;					//�e�N�X�`���F���ʔ��ˌ�
		std::vector<std::string> aTex_Shininess;				//�e�N�X�`���F����
		std::vector<std::string> aTex_Normal;					//�e�N�X�`���F�m�[�}��
		std::vector<LAYER_TEX_DATA> aLayerTex_Ambient;
		std::vector<LAYER_TEX_DATA> aLayerTex_Diffuse;
		std::vector<LAYER_TEX_DATA> aLayerTex_Emissive;
		std::vector<LAYER_TEX_DATA> aLayerTex_Transparent;
		std::vector<LAYER_TEX_DATA> aLayerTex_Specular;
		std::vector<LAYER_TEX_DATA> aLayerTex_Shininess;
		std::vector<LAYER_TEX_DATA> aLayerTex_Normal;			//���C���e�N�X�`��
		std::vector<int> aBoneID;								//���b�V���ɉe����^���鍜�̔ԍ�
		std::vector<FbxAMatrix> aReferenceGlobalInitPosition;	//�p���v�Z�p�����s��
		std::vector<FbxMatrix> aClusterRelativeInitPosition;	//�p���v�Z�p�����s��

		//���Ȃ��p���s��
		struct NO_SKIN_MTX
		{
			std::vector<DirectX::XMFLOAT4X4> aMatrix;		//�t���[���p��

			NO_SKIN_MTX() noexcept : aMatrix(0)
			{}
			~NO_SKIN_MTX() noexcept {}
		};
		std::vector<NO_SKIN_MTX> aNoSkinData;				//�t���[�����i���Ȃ��j

		MESH_DATA() noexcept :
			NodeName(""), aIndexBuffer(0), vsData(), MaterialName(""), MaterialData(), IsPhong(true),
			aTex_Ambient(0), aTex_Diffuse(0), aTex_Emissive(0), aTex_Transparent(0), aTex_Specular(0), aTex_Shininess(0), aTex_Normal(0),
			aLayerTex_Ambient(0), aLayerTex_Diffuse(0), aLayerTex_Emissive(0), aLayerTex_Transparent(0), aLayerTex_Specular(0), aLayerTex_Shininess(0), aLayerTex_Normal(0),
			aBoneID(0), aReferenceGlobalInitPosition(0), aClusterRelativeInitPosition(0), aNoSkinData(0)
		{}
		~MESH_DATA() noexcept {}
	};

	//�A�j���[�V�������
	struct ANIM_DATA
	{
		std::string TakeName;	//�e�C�N��
		FbxTime Start;			//�J�n����
		FbxTime Stop;			//�I������
		int StartFrame;			//�J�n�t���[��
		int StopFrame;			//�I���t���[��
		bool bIsFPS_30;			//FPS��30

		ANIM_DATA() noexcept : TakeName(""), Start(), Stop(), StartFrame(0), StopFrame(0), bIsFPS_30(false)
		{}
		~ANIM_DATA() noexcept {}
	};

	//�����
	struct BONE_DATA
	{
		int Index;								//�z��C���f�b�N�X
		std::string BoneName;					//�{�[����
		DirectX::XMFLOAT4X4 InitMatrix;			//�����p��
		bool bMtxIsLoad;						//�����p���擾�ς�

		bool IsRoot;							//���[�g�{�[�����ǂ���
		std::vector<BONE_DATA*> aChildBone;		//�q�{�[���̎Q�Ɛ�

		BONE_DATA() noexcept : Index(0), BoneName(""), bMtxIsLoad(false), IsRoot(false), aChildBone(0)
		{
			DirectX::XMStoreFloat4x4(&InitMatrix, DirectX::XMMatrixIdentity());
		}
		~BONE_DATA() noexcept {}
	};

	//�t���[�����
	struct FRAME_DATA
	{
		std::vector<DirectX::XMFLOAT4X4> aMatrix;		//�t���[���p��
		std::vector<DirectX::XMFLOAT4X4> aBoneMatrix;	//�t���[���p���i���j

		FRAME_DATA() noexcept : aMatrix(0), aBoneMatrix(0)
		{}
		~FRAME_DATA() noexcept {}
	};

	//�X�L�����
	struct SKIN_DATA
	{
		std::vector<FRAME_DATA> aFrameData;		//�t���[�����

		SKIN_DATA() noexcept : aFrameData(0)
		{}
		~SKIN_DATA() noexcept {}
	};

	//�v���g�^�C�v�錾
	FBX_LOADER() noexcept;
	~FBX_LOADER() noexcept;
	void Draw() const noexcept;										//�`�揈��
	void Load(const char* FilePath, bool bAnimOnly = false);
	void Load(const wchar_t* FilePath, bool bAnimOnly = false);		//�t�@�C���Ǎ�
	void SaveModelData(const char* FileName) noexcept;				//�t�@�C�����o���i���f���j
	void SaveAnimData(const char* FileName, int AnimID) noexcept;	//�t�@�C�����o���i�A�j���[�V�����j

	std::string& GetFilePath() noexcept					//�t�@�C���p�X�Q��
	{
		return m_FilePath;
	}
	MESH_DATA& GetMesh(int Index) noexcept				//���b�V���Q��
	{
		return m_aMesh[Index];
	}
	int GetMeshCount() const noexcept					//���b�V���̐��擾
	{
		return static_cast<int>(m_aMesh.size());
	}
	std::vector<BONE_DATA>& GetSkeleton() noexcept		//���Q��
	{
		return m_aBone;
	}
	std::vector<ANIM_DATA>& GetAnimation() noexcept		//�A�j���[�V�����Q��
	{
		return m_aAnimation;
	}
	std::vector<SKIN_DATA>& GetSkin() noexcept			//�X�L���Q��
	{
		return m_aSkin;
	}

private:

	//�ϐ��錾
	FbxManager* m_pManager;								//�}�l�[�W��
	FbxIOSettings* m_pioSettings;						//���o�͐ݒ�
	FbxImporter* m_pImporter;							//�C���|�[�^
	FbxScene* m_pScene;									//�V�[��
	FbxAnimEvaluator* m_pEvaluator;						//�A�j���[�V�����]��
	FbxTime::EMode m_TimeMode;							//�A�j���[�V�������Ԑݒ�
	FbxTime m_Period;									//�t���[������
	int AnimLoadBaseIndex;								//�A�j���[�V�����Ǎ��J�n�ԍ�

	std::string m_FilePath;								//�t�@�C���p�X
	std::vector<FBX_DATA> m_aData;						//FBX���
	std::vector<MESH_DATA> m_aMesh;						//���b�V�����
	std::vector<ANIM_DATA> m_aAnimation;				//�A�j���[�V�������
	std::vector<BONE_DATA> m_aBone;						//�����
	std::vector<SKIN_DATA> m_aSkin;						//�X�L�����

	//�v���g�^�C�v�錾
	void GetNodes(FbxNode* Node, int SpaceCnt, BONE_DATA* ParentBone = nullptr) noexcept;				//�m�[�h�擾
	void GetMesh(FbxNodeAttribute* MeshIn, std::string NodeName) noexcept;								//���b�V���擾
	void GetTexturePath(MESH_DATA& Mesh, FbxSurfaceMaterial* pMaterial, bool IsPhong = true) noexcept;	//�e�N�X�`���p�X�擾
	void GetTextureName(FbxSurfaceMaterial* pMaterial, const char* Type, std::vector<std::string>& Tex, std::vector<LAYER_TEX_DATA>& LayerTex) noexcept;	//�e�N�X�`�����擾
	void GetSkinData(MESH_DATA& Mesh, FbxMesh* MeshIn) noexcept;										//�t���[�����Ǎ�
	void GetAnimationFromSkin(FbxMesh* MeshIn) noexcept;												//�t���[�����Ǎ��i�A�j���[�V�����P�̗p�j
	void GetBoneData() noexcept;																		//�����擾
	void GetAnimationData() noexcept;																	//�A�j���[�V�������擾
	void LoadAnimation();																				//�A�j���[�V�����Ǎ�
};
