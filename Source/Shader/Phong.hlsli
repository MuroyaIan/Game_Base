/**
 * @file Phong.hlsli
 * @brief Phong���f����`
 * @author ���J�C�A��
 * @date 2023/02/16
 * @���� 2023/02/16�F�t�@�C���쐬
 */

//�\���̐錾
struct MATERIAL_DATA
{
	float4 Ambient;				//����
	float4 Diffuse;				//�g�U���ˌ�
	float4 Emissive;			//���ˌ�
	float4 Transparent;			//���ߓx
	float4 Specular;			//���ʔ��ˌ�
	float Shininess;			//����
	float Disp_MinLayerNum;
	float Disp_MaxLayerNum;		//�����}�b�v�̃T���v�����O�͈�
	float Disp_DepthScale;		//�����}�b�v�̐[���W��
};
