/**
 * @file Grid.h
 * @brief �O���b�h�`��
 * @author ���J�C�A��
 * @date 2022/07/19
 * @���� 2022/07/19�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <Geometry/ShaderMgr.h>

//===== �N���X��` =====

//***** �O���b�h�`�� *****
class GRID : public DRAWER
{
public:

	//�v���g�^�C�v�錾
	GRID(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr);
	~GRID() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(int InstanceNum = -1) const noexcept override;		//�����ݏ���

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//�|���S�����擾
	{
		return GetIndexNum() / 2;
	}

private:

	//�ϐ��錾
	SHADER_MGR& m_ShaderMgr;			//�V�F�[�_Mgr�Q��
	DirectX::XMFLOAT4X4 m_mtxWorld;		//���[���h�s��
	DirectX::XMFLOAT2 m_Size;			//�T�C�Y�iXY�j
};
