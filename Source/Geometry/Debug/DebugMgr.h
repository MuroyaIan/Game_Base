/**
 * @file DebugMgr.h
 * @brief �f�o�b�O�}�l�[�W��
 * @author ���J�C�A��
 * @date 2022/08/12
 * @���� 2022/08/12�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Geometry/Debug/Mesh_Light.h>

//===== �N���X��` =====

//***** �f�o�b�O�}�l�[�W�� *****
class DEBUG_MGR
{
public:

	//�v���g�^�C�v�錾
	DEBUG_MGR(APP& App) noexcept;
	~DEBUG_MGR() noexcept;
	void Update() noexcept;					//�X�V����
	void Draw() const;						//�`�揈��
	UINT GetPolygonNum() const noexcept;	//�|���S�����擾

	MESH_LIGHT& GetLightMesh() noexcept		//���C�g���b�V���擾
	{
		return m_Light;
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;		//�`��f�[�^�Q��
	MESH_LIGHT m_Light;		//���C�g�A�C�R���p���b�V��
};
