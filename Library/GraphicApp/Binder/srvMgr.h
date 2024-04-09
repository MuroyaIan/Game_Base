/**
 * @file cbMgr.h
 * @brief �萔�o�b�t�@�}�l�[�W��
 * @author ���J�C�A��
 * @date 2023/01/13
 * @���� 2023/01/13�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �\���̐錾 =====
struct SRV_PTR												//SRV�̃|�C���^���
{
	//�ϐ��錾
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrVS;
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrPS;		//�|�C���^�z��

	SRV_PTR() noexcept : m_aSrvPtrVS(0), m_aSrvPtrPS(0)
	{}

	~SRV_PTR() noexcept
	{}
};

//===== �N���X��` =====
class SRV_MGR : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit SRV_MGR(SRV_PTR& srvPtrRef) noexcept;
	~SRV_MGR() noexcept override;

	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����

private:

	//�ϐ��錾
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrVS;
	std::vector<ID3D11ShaderResourceView*> m_aSrvPtrPS;		//SRV�̃|�C���^�z��
	UINT m_SrvSizeVS;
	UINT m_SrvSizePS;										//SRV�̃T�C�Y
};
