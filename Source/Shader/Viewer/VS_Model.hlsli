/**
 * @file VS_Model.hlsli
 * @brief ���\�[�X�Q�Ɓi�r�����[�p�j
 * @author ���J�C�A��
 * @date 2023/01/23
 * @���� 2023/01/23�F�t�@�C���쐬
 */

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_MTX_LWVP : register(b0)
{
	matrix mtxLocal;	//���[�J���s��i���Ȃ����b�V���̕ϊ��j
	matrix mtxWorld;	//���[���h�s��
	matrix mtxView;		//�r���[�s��
	matrix mtxProj;		//���e�s��
};

//�萔�o�b�t�@�i�����j
cbuffer CB_BONE : register(b1)
{
	matrix mtxBone[250];	//���s��
};

//�萔�o�b�t�@�i��������j
cbuffer CB_LIGHT_CTRL : register(b2)
{
	float4 cbLightOffset;	//�����̕ω���
}

//�O���[�o���萔�i���̕��s�����j
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };	//���s�����̌���
