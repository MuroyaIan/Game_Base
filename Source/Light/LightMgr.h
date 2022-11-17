/**
 * @file LightMgr.h
 * @brief ���C�g�}�l�[�W��
 * @author ���J�C�A��
 * @date 2022/08/20
 * @���� 2022/08/20�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Light/Light.h>

//===== �萔�E�}�N����` =====
constexpr int LIGHT_NUM = 16;		//�_�����̍ő�`�搔

//===== �O���錾 =====
template<typename C>
class PIXEL_CBUFFER;

//===== �N���X��` =====

//***** ���C�g�}�l�[�W�� *****
class LIGHT_MGR
{
public:

	//�o�b�t�@�p�b�N
	struct LIGHT_PACK
	{
		LIGHT::LIGHT_DIRECTIONAL DirectionalLight;	//���s����
		LIGHT::LIGHT_POINT PointLight[LIGHT_NUM];	//�_����
		DirectX::XMFLOAT4 AmbientLight;				//����

		LIGHT_PACK() noexcept : DirectionalLight(), PointLight(), AmbientLight(1.0f, 1.0f, 1.0f, 0.2f)
		{}
		~LIGHT_PACK() noexcept
		{}
	};

	//�v���g�^�C�v�錾
	LIGHT_MGR(APP_64& App) noexcept;
	~LIGHT_MGR() noexcept;
	void Draw() noexcept;									//�`�揈��
	void AddPointLight(LIGHT::LIGHT_POINT Data) noexcept;	//�|�C���g���C�g�o�^

	LIGHT_PACK& GetData() noexcept							//���C�g���Q��
	{
		return m_LightData;
	}

	int GetLightNum() noexcept								//�|�C���g���C�g�g�p���擾
	{
		return m_UsedData_backup;
	}

private:

	//�ϐ��錾
	GFX_PACK& m_Gfx;										//�`��f�[�^�Q��
	LIGHT_PACK m_LightData;									//���C�g���
	int m_UsedData;											//�|�C���g���C�g�g�p��
	int m_UsedData_backup;									//�O���擾�p
	std::unique_ptr<PIXEL_CBUFFER<LIGHT_PACK>> m_pcBuff;	//�萔�o�b�t�@
};
