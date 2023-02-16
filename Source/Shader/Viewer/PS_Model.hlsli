/**
 * @file VS_Model.hlsli
 * @brief ���\�[�X�Q�Ɓi�r�����[�p�j
 * @author ���J�C�A��
 * @date 2023/01/23
 * @���� 2023/01/23�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <../Light.hlsli>
#include <../Phong.hlsli>

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexMap[4] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal, 3:Displacement

//�萔�o�b�t�@�i�}�e���A���j
cbuffer CB_MATERIAL : register(b0)
{
	MATERIAL_DATA matData;
}

//�萔�o�b�t�@�i���C�g���j
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;		//���s����
	LIGHT_POINT PointLight[16];				//�_����
	float4 AmbientLight;					//����
}
