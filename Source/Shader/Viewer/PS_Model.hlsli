/**
 * @file VS_Model.hlsli
 * @brief ���\�[�X�Q�Ɓi�r�����[�p�j
 * @author ���J�C�A��
 * @date 2023/01/23
 * @���� 2023/01/23�F�t�@�C���쐬
 */

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexMap[4] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal, 3:Displacement

//�萔�o�b�t�@�i�}�e���A���j
cbuffer CB_MATERIAL : register(b0)
{
	float4 cbAmbient;			//����
	float4 cbDiffuse;			//�g�U���ˌ�
	float4 cbEmissive;			//���ˌ�
	float4 cbTransparent;		//���ߓx
	float4 cbSpecular;			//���ʔ��ˌ�
	float cbShininess;			//����
	float cbDisp_MinLayerNum;
	float cbDisp_MaxLayerNum;	//�����}�b�v�̃T���v�����O�͈�
	float cbDisp_DepthScale;	//�����}�b�v�̐[���W��
}

//�O���[�o���萔�i���̕��s�����j
static const float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };		//���s�����̌�
static const float4 GlobalAmbient = { 1.0f, 1.0f, 1.0f, 0.2f };		//������
