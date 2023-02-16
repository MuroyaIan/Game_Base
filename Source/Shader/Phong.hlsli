/**
 * @file Phong.hlsli
 * @brief Phongfθ`
 * @author ΊJCA
 * @date 2023/02/16
 * @π 2023/02/16Ft@Cμ¬
 */

//\’ΜιΎ
struct MATERIAL_DATA
{
	float4 Ambient;				//Β«υ
	float4 Diffuse;				//gU½Λυ
	float4 Emissive;			//­Λυ
	float4 Transparent;			//§ίx
	float4 Specular;			//ΎΚ½Λυ
	float Shininess;			//υς
	float Disp_MinLayerNum;
	float Disp_MaxLayerNum;		//·}bvΜTvOΝΝ
	float Disp_DepthScale;		//·}bvΜ[³W
};
