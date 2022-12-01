/**
 * @file BoneLine.h
 * @brief ���f���`��
 * @author ���J�C�A��
 * @date 2022/08/10
 * @���� 2022/08/10�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <Geometry/ShaderMgr.h>
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== �O���錾 =====
class VIEWER;
class FBX_LOADER;

//===== �N���X��` =====

//***** ���f���`�� *****
class BONE_LINE : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	BONE_LINE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, DRAWER& BoneIn);
	~BONE_LINE() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(int InstanceNum = -1) const noexcept override;		//�����ݏ���

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//�|���S�����擾
	{
		return GetIndexNum() / 3;
	}

private:

	//�ϐ��錾
	GRAPHIC& m_Gfx;							//�O���t�B�b�N�Q��
	SHADER_MGR& m_ShaderMgr;				//�V�F�[�_Mgr�Q��
	FBX_LOADER& m_Loader;					//���[�_�Q��
	DirectX::XMFLOAT4X4 m_mtxWorld;			//���[���h�s��

	DRAWER& m_Bone;							//���Q��
	bool& m_bDrawAnimation;					//�A�j���[�V�����Đ�
	int& m_AnimationID;						//�A�j���[�V�����ԍ�
	int& m_AnimFrame;						//�Đ�����t���[����

	//�v���g�^�C�v�錾
	VS_DATA<VERTEX> MakeData_VS() const;	//���_���쐬
};
