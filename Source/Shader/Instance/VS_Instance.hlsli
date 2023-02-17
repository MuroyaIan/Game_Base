/**
 * @file VS_Instance.hlsli
 * @brief ���\�[�X�Q�Ɓi�C���X�^���V���O�p�j
 * @author ���J�C�A��
 * @date 2023/02/17
 * @���� 2023/02/17�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <../Light.hlsli>

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_CAMERA : register(b0)
{
	matrix mtxView;		//�r���[�s��
	matrix mtxProj;		//���e�s��
};

//�萔�o�b�t�@�i�������j
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;		//���s����
	LIGHT_POINT PointLight[16];				//�_����
	float4 AmbientLight;					//����
}

#ifdef MODEL_ANIM

	//�萔�o�b�t�@�i�����j
	cbuffer CB_BONE : register(b2)
	{
		matrix mtxBone[250];	//���s��
	};

	//�萔�o�b�t�@�i���[�J���s��j
	cbuffer CB_LOCAL : register(b3)
	{
		matrix mtxSkin;		//���Ȃ����b�V���p���[�J���s��
	};

#endif // MODEL_ANIM
