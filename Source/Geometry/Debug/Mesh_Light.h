/**
 * @file Mesh_Light.h
 * @brief ���C�g�p���b�V��
 * @author ���J�C�A��
 * @date 2022/08/12
 * @���� 2022/08/12�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <App.h>

//===== �N���X��` =====

//***** ���C�g�p���b�V�� *****
class MESH_LIGHT : public DRAWER_EX<MESH_LIGHT>
{
public:

	//�v���g�^�C�v�錾
	MESH_LIGHT(GFX_PACK& Gfx);
	~MESH_LIGHT() noexcept override;
	void Update() noexcept override;															//�X�V����
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//�����ݏ���
	int AddInstance() override;																	//�C���X�^���X�ǉ�

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//�ό`�s��擾
	{
		return m_aMtxWorld[InstanceIndex];
	}

	UINT GetPolygonNum() const noexcept override												//�|���S�����擾
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

	void SetPos(DirectX::XMFLOAT3 Pos, int InstanceIndex = 0) noexcept							//���W�ݒ�
	{
		m_aMtxData[InstanceIndex].m_Pos = Pos;
	}

	void SetScale(DirectX::XMFLOAT3 Scale, int InstanceIndex = 0) noexcept						//�T�C�Y�ݒ�
	{
		m_aMtxData[InstanceIndex].m_Scale = Scale;
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;								//�`��f�[�^�Q��
	int m_InstanceNum;								//�C���X�^���X��
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//���[���h�s��

	struct MTX_DATA
	{
		DirectX::XMFLOAT3 m_Pos;	//���[���h���W
		DirectX::XMFLOAT3 m_Scale;	//�T�C�Y

		MTX_DATA() : m_Pos(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f)
		{}
	};
	std::vector<MTX_DATA> m_aMtxData;				//�s��v�Z�p���
};
