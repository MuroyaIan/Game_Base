/**
 * @file App.h
 * @brief �v���b�g�t�H�[�������iWin64�j
 * @author ���J�C�A��
 * @date 2022/06/09
 * @���� 2022/06/09�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/BaseWindow.h>
#include <Tool/TimeProc.h>
#include <GraphicApp/Graphic.h>
#include <Geometry/ShaderMgr.h>
#include <Geometry/TextureMgr.h>
#include <Geometry/Model/ModelMgr.h>
#include <Tool/Input/InputMgr.h>

#ifdef IMGUI
#
#	include <Tool/imguiMgr.h>
#
#endif // IMGUI

//===== �\���̒�` =====
struct GFX_PACK					//�`��p�f�[�^
{
	GRAPHIC& m_DX;				//DX�I�u�W�F�N�g�Q��
	SHADER_MGR& m_ShaderMgr;	//�V�F�[�_�}�l�[�W���Q��
	TEXTURE_MGR& m_TextureMgr;	//�e�N�X�`���}�l�[�W���Q��
	MODEL_MGR& m_ModelMgr;		//���f���}�l�[�W���Q��
};

//===== �O���錾 =====
class EDITOR;
class CAMERA_MGR;
class LIGHT_MGR;

class DRAWER;
class MODEL;
class DIRECTIONAL_LIGHT;
class POINT_LIGHT;
class PLAYER;

#ifdef IMGUI

	class DEBUG_MGR;

#endif // IMGUI

//===== �N���X��` =====

//***** �Q�[���A�v���iWin64�j *****
class APP
{
public:

	//�v���g�^�C�v�錾
	APP();
	~APP();
	int Run();											//�A�v���P�[�V�������s
	void Update();										//�X�V����
	void Draw();										//�`�揈��

	IF_WINDOW& GetWindowProc() const noexcept			//�E�B���h�E�����Q��
	{
		return *m_pWindow.get();
	}
	GAME_TIME& GetTimeProc() noexcept					//���ԏ����Q��
	{
		return m_Time;
	}
	GFX_PACK& GetGfxPack() const noexcept				//�`��f�[�^�Q��
	{
		return *m_pGfx;
	}
	INPUT_MGR& GetInputMgr() const noexcept				//���̓}�l�[�W���Q��
	{
		return *m_pInputMgr;
	}

#ifdef IMGUI

	DEBUG_MGR& GetDebugMgr() const noexcept				//�f�o�b�O�}�l�[�W���Q��
	{
		return *m_pDebugMgr;
	}

#endif // IMGUI

	CAMERA_MGR& GetCameraMgr() const noexcept			//�J�����}�l�[�W���Q��
	{
		return *m_pCameraMgr;
	}

	LIGHT_MGR& GetLightMgr() const noexcept				//���C�g�}�l�[�W���Q��
	{
		return *m_pLightMgr;
	}

private:

	//�ϐ��錾

#ifdef IMGUI

	IMGUI_MGR m_ImGui;									//ImGui�}�l�[�W��
	std::unique_ptr<DEBUG_MGR> m_pDebugMgr;				//�f�o�b�O�}�l�[�W���p�|�C���^

#endif // IMGUI

	std::unique_ptr<IF_WINDOW> m_pWindow;				//�Q�[���E�B���h�E
	MSG m_Message;										//���b�Z�[�W�\����
	GAME_TIME m_Time;									//���ԏ���
	std::unique_ptr<GRAPHIC> m_pDX;						//DirectX�p�|�C���^
	std::unique_ptr<SHADER_MGR> m_pShaderMgr;			//�V�F�[�_�}�l�[�W���p�|�C���^
	std::unique_ptr<TEXTURE_MGR> m_pTextureMgr;			//�e�N�X�`���}�l�[�W���p�|�C���^
	std::unique_ptr<MODEL_MGR> m_pModelMgr;				//���f���}�l�[�W���p�|�C���^
	std::unique_ptr<GFX_PACK> m_pGfx;					//�`��f�[�^�p�|�C���^
	std::unique_ptr<INPUT_MGR> m_pInputMgr;				//���̓}�l�[�W���p�|�C���^
	std::unique_ptr<EDITOR> m_pEditor;					//�G�f�B�^�p�|�C���^
	std::unique_ptr<CAMERA_MGR> m_pCameraMgr;			//�J�����}�l�[�W��
	std::unique_ptr<LIGHT_MGR> m_pLightMgr;				//���C�g�}�l�[�W��



	std::vector<std::unique_ptr<DRAWER>> m_aDrawer;		//�y�`��e�X�g�z
	static constexpr int nDrawNum = 120;				//�`�搔
	std::vector<std::unique_ptr<MODEL>> m_aModel;		//���f��
	std::unique_ptr<DIRECTIONAL_LIGHT> m_pSunLight;		//���z��
	std::vector<std::unique_ptr<POINT_LIGHT>> m_aLight;	//�_�����z��
	std::unique_ptr<PLAYER> m_pPlayer;					//�ۑ�p�v���C���[

	//�����w��
	friend class EDITOR;
};
