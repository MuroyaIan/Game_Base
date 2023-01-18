/**
 * @file Normal.h
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
class FBX_LOADER;

//===== �N���X��` =====

//***** ���f���`�� *****
class NORMAL : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	explicit NORMAL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, FBX_LOADER& Loader, int MeshIndex, DRAWER& ModelIn);
	~NORMAL() noexcept override;
	void Update() noexcept override;													//�X�V����
	void Draw(int InstanceNum = 0) const noexcept override;								//�����ݏ���

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override	//�ό`�s��擾
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	UINT GetPolygonNum() const noexcept override										//�|���S�����擾
	{
		return GetIndexNum() / 3;
	}

private:

	//�ϐ��錾
	SHADER_MGR& m_ShaderMgr;				//�V�F�[�_Mgr�Q��
	FBX_LOADER& m_Loader;					//���[�_�Q��
	int m_MeshIndex;						//���b�V���ԍ�
	DirectX::XMFLOAT4X4 m_mtxWorld;			//���[���h�s��

	DRAWER& m_Model;						//���f���Q��

	//�v���g�^�C�v�錾
	VS_DATA<VERTEX_C> MakeData_VS() const;	//���_���쐬
};
