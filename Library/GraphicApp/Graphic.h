/**
 * @file Graphic.h
 * @brief DirectX����
 * @author ���J�C�A��
 * @date 2022/06/12
 * @���� 2022/06/12�F�t�@�C���쐬
 *		 2022/11/21�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/WinWindow.h>
#include <wrl.h>					//COM�|�C���^
#include <dxgi1_6.h>
#include <d3d11.h>
#include <d3dcompiler.h>			//�V�F�[�_�R���p�C��
#include <DirectXMath.h>
#include <Startup.h>				//�f�o�b�O����

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>				//DXGI�f�o�b�O

#endif // _DEBUG

//===== �萔�E�}�N����` =====
#define ERROR_DX(hr) if((hr) != S_OK) throw ERROR_EX(hr)	//�G���[�o��(DirectX�p)

//===== �N���X��` =====

//***** �O���t�B�b�N *****
class CT_GRAPHIC
{
public:

	//�`�惂�[�h
	enum class ET_DRAW_MODE
	{
		me_Draw_2D,	//2D
		me_Draw_3D	//3D
	};

	//�R�s�[�����[�u
	CT_GRAPHIC(const CT_GRAPHIC&) = delete;
	CT_GRAPHIC& operator =(const CT_GRAPHIC&) = delete;
	CT_GRAPHIC(CT_GRAPHIC&&) noexcept = default;
	CT_GRAPHIC& operator=(CT_GRAPHIC&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_GRAPHIC(HWND hWindow, float fWidth, float fHeight);
	~CT_GRAPHIC() noexcept(!gc_IS_DEBUG);
	void BeginFrame(float r, float g, float b) const noexcept;				//�t���[���J�n
	void DrawIndexed(UINT indexNum) const noexcept;							//�C���f�b�N�X�`��
	void DrawInstanced(UINT indexNum, UINT instanceNum) const noexcept;		//�C���X�^���V���O�`��
	void EndFrame() const;													//�t���[���I���˕`��J�n
	void SetDrawMode(ET_DRAW_MODE mode) const noexcept;						//�`�惂�[�h�ݒ�

	void SetViewMtx(const DirectX::XMFLOAT4X4& mtxView) noexcept			//�r���[�s��ւ̃A�N�Z�X
	{
		m_MtxView = mtxView;
	}
	[[nodiscard]] DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
	{
		return m_MtxView;
	}
	void SetProjectionMtx(const DirectX::XMFLOAT4X4& mtxProj) noexcept		//���e�s��ւ̃A�N�Z�X
	{
		m_MtxProjection = mtxProj;
	}
	[[nodiscard]] DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
	{
		return m_MtxProjection;
	}

#ifdef IMGUI

	void SetImGuiMode(const bool bEnable) noexcept							//ImGui�`��ON/OFF
	{
		m_bDrawImGui = bEnable;
	}

	[[nodiscard]] bool IsImGuiEnabled() const noexcept						//ImGui�`���Ԋm�F
	{
		return m_bDrawImGui;
	}

#endif // IMGUI

private:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter;						//�A�_�v�^�[
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;							//�f�o�C�X
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;					//�X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;					//�R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pView_RenderTarget;	//�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pView_DepthStencil;	//�[�x�E�X�e���V���r���[

	DirectX::XMFLOAT4X4 m_MtxView;			//�r���[�s��i�J�����j
	DirectX::XMFLOAT4X4 m_MtxProjection;	//���e�s��

#ifdef IMGUI

	bool m_bDrawImGui;						//ImGUI�`�搧��

#endif // IMGUI

	//�v���g�^�C�v�錾
	void InitDxgi();						//DXGI������

	//�����w��
	friend class BINDER;
};
