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
#define ERROR_DX(hr) if(hr != S_OK) throw ERROR_EX(hr)	//�G���[�o��(DirectX�p)

//===== �N���X��` =====

//***** �O���t�B�b�N *****
class GRAPHIC
{
public:

	//�`�惂�[�h
	enum class DRAW_MODE
	{
		DRAW_2D,
		DRAW_3D
	};

	//�R�s�[NG
	GRAPHIC(const GRAPHIC&) = delete;
	GRAPHIC& operator=(const GRAPHIC&) = delete;

	//�v���g�^�C�v�錾
	explicit GRAPHIC(HWND hWindow, float fWidth, float fHeight);
	~GRAPHIC() noexcept(!IS_DEBUG);
	void BeginFrame(float R, float G, float B) const noexcept;				//�t���[���J�n
	void DrawIndexed(UINT IndexNum) const noexcept;							//�C���f�b�N�X�`��
	void DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept;		//�C���X�^���V���O�`��
	void EndFrame() const;													//�t���[���I���˕`��J�n
	void SetDrawMode(DRAW_MODE Mode) const noexcept;						//�`�惂�[�h�ݒ�

	void SetViewMtx(DirectX::XMFLOAT4X4 mtxView) noexcept			//�r���[�s��ւ̃A�N�Z�X
	{
		m_mtxView = mtxView;
	}
	DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
	{
		return m_mtxView;
	}
	void SetProjectionMtx(DirectX::XMFLOAT4X4 mtxProj) noexcept		//���e�s��ւ̃A�N�Z�X
	{
		m_mtxProjection = mtxProj;
	}
	DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
	{
		return m_mtxProjection;
	}

#ifdef IMGUI

	void SetImGuiMode(bool bEnable) noexcept						//ImGui�`��ON/OFF
	{
		m_bDrawImGui = bEnable;
	}

	bool IsImGuiEnabled() const noexcept							//ImGui�`���Ԋm�F
	{
		return m_bDrawImGui;
	}

#endif // IMGUI

private:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter;				//�A�_�v�^�[
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;					//�f�o�C�X
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;			//�X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;			//�R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTView;		//�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;		//�[�x�E�X�e���V���r���[

	DirectX::XMFLOAT4X4 m_mtxView;			//�r���[�s��i�J�����j
	DirectX::XMFLOAT4X4 m_mtxProjection;	//���e�s��

#ifdef IMGUI

	bool m_bDrawImGui;						//ImGUI�`�搧��

#endif // IMGUI

	//�v���g�^�C�v�錾
	void InitDXGI();						//DXGI������

	//�����w��
	friend class BINDER;
};
