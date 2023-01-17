/**
 * @file Viewer_Model.h
 * @brief ���f���`��
 * @author ���J�C�A��
 * @date 2022/07/19
 * @���� 2022/07/19�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <App.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== �O���錾 =====
class VIEWER;
class FBX_LOADER;
struct CBD_BONE;

//===== �N���X��` =====

//***** ���f���`�� *****
class VIEWER_MODEL : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	explicit VIEWER_MODEL(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex);
	~VIEWER_MODEL() noexcept override;
	void Update() noexcept override;													//�X�V����
	void Draw(int InstanceNum = 0) const noexcept override;								//�����ݏ���

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override	//�ό`�s��擾
	{
		(void)InstanceIndex;
		return m_MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override										//�|���S�����擾
	{
		return GetIndexNum() / 3;
	}

private:

	//�ϐ��錾
	SHADER_MGR& m_ShaderMgr;				//�V�F�[�_Mgr�Q��
	VIEWER& m_Viewer;						//�r���[���Q��
	FBX_LOADER& m_Loader;					//���[�_�Q��
	int m_MeshIndex;						//���b�V���ԍ�
	DirectX::XMFLOAT4X4 m_MtxLocal;			//���[�J���s��
	DirectX::XMFLOAT4X4 m_MtxWorld;			//���[���h�s��
	CBD_MATERIAL m_Material;				//�}�e���A�����
	bool m_bNoBone;							//���Ȃ����b�V��

	std::unique_ptr<CBD_BONE> m_pMtxBone;	//�����
	bool& m_bDrawAnimation;					//�A�j���[�V�����Đ�
	int& m_AnimationID;						//�A�j���[�V�����ԍ�
	int m_AnimFrame;						//�Đ�����t���[����
	int m_FrameCnt;							//�Đ��t���[���v�Z�p
	bool& m_AnimPause;						//�A�j���[�V�����ꎞ��~

	float& m_Scale;							//���f���X�P�[��
	float& m_RotY;							//��](Y��)

	//�v���g�^�C�v�錾
	VS_DATA<VERTEX_MB> MakeData_VS() const noexcept;	//���_���쐬
	void UpdateBoneData(int AnimID = 0) noexcept;		//�����X�V
};
