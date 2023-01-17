/**
 * @file Viewer.h
 * @brief ���f���r���[��
 * @author ���J�C�A��
 * @date 2022/07/19
 * @���� 2022/07/19�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <EditorApp/Editor.h>
#include <EditorApp/ModelViewer/Viewer_Camera.h>
#include <EditorApp/ModelViewer/ModelLoader.h>

//===== �O���錾 =====
class DRAWER;
class BONE;

//===== �N���X��` =====

//***** ���f���r���[�� *****
class VIEWER
{
public:

	//�v���g�^�C�v�錾
	explicit VIEWER(APP& App);
	~VIEWER() noexcept;
	void Update() noexcept;							//�X�V����
	void Draw() const;								//�`�揈��
	int GetPolygonNum() const noexcept;				//�|���S�����擾
	void LoadModel(bool bAnimOnly = false);			//���f���Ǎ�

	int GetDrawerNum() const noexcept				//�h�����[�L�����擾
	{
		return static_cast<int>(m_aDrawer.size());
	}
	VIEWER_CAM& GetCamera() noexcept				//�J�����Q��
	{
		return m_Camera;
	}
	FBX_LOADER& GetLoader() const noexcept			//FBX���[�_�Q��
	{
		return *m_pLoader;
	}
	float& GetModelScale() noexcept					//���f���X�P�[���Q��
	{
		return m_Scale;
	}
	float& GetModelRotation() noexcept				//���f����](Y��)�Q��
	{
		return m_RotY;
	}
	bool& GetFlag_DrawNormal() noexcept				//�@���\���t���O�Q��
	{
		return m_bDrawNormal;
	}
	bool& GetFlag_DrawSurface() noexcept			//�|���S���\���ؑփt���O�Q��
	{
		return m_bDrawSurface;
	}
	bool& GetFlag_DrawAnimation() noexcept			//�A�j���[�V�����Đ��t���O�Q��
	{
		return m_bDrawAnimation;
	}
	int& GetAnimationID() noexcept					//�A�j���[�V�����ԍ��Q��
	{
		return m_AnimationID;
	}
	int& GetAnimationFrame() noexcept				//�Đ��t���[�����Q��
	{
		return m_AnimFrame;
	}
	bool& GetFlag_AnimPause() noexcept				//�A�j���[�V�����ꎞ��~�t���O�Q��
	{
		return m_bAnimPause;
	}
	bool& GetFlag_DrawBone() noexcept				//���`��t���O�Q��
	{
		return m_bDrawBone;
	}
	BONE& GetBone() const noexcept;					//�����b�V���Q��

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;									//�`��f�[�^�Q��
	INPUT_MGR& m_Input;									//���͎Q��
	std::vector<std::unique_ptr<DRAWER>> m_aDrawer;		//�`��z��
	VIEWER_CAM m_Camera;								//�J����

	std::unique_ptr<FBX_LOADER> m_pLoader;				//FBX���[�_
	float m_Scale;										//���f���X�P�[��
	float m_RotY;										//���f����](Y��)
	bool m_bDrawNormal;									//�@���\��
	bool m_bDrawSurface;								//�|���S���\���ؑ�

	bool m_bDrawAnimation;								//�A�j���[�V�����Đ�
	int m_AnimationID;									//�A�j���[�V�����ԍ�
	int m_AnimFrame;									//�Đ�����t���[����
	bool m_bAnimPause;									//�A�j���[�V�����ꎞ��~

	std::unique_ptr<DRAWER> m_Drawer_Bone;				//���`��p
	std::unique_ptr<DRAWER> m_Drawer_BoneLine;			//�����C���`��p
	bool m_bDrawBone;									//���`��t���O
};
