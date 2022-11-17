/**
 * @file Rand.h
 * @brief 乱数処理
 * @author 室谷イアン
 * @date 2022/07/04
 * @履歴 2022/07/04：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <random>					//乱数生成

//===== 定数・マクロ定義 =====
using RAND = std::mt19937_64;
using RandRange_Int = std::uniform_int_distribution<int>;
using RandRange_Float = std::uniform_real_distribution<float>;	//一様分布器

//===== クラス定義 =====

//***** 乱数生成クラス *****
class RAND_MAKER
{
public:

	//プロトタイプ宣言
	static void InitMaker() noexcept;									//生成器初期化
	static int MakeRand_Int(int Min = 0, int Max = 9);					//整数生成
	static float MakeRand_Float(float Min = 0.0f, float Max = 9.9f);	//小数生成

private:

	//変数宣言
	static RAND m_Rand;		//乱数生成器

	//プロトタイプ宣言
	RAND_MAKER() noexcept;
	~RAND_MAKER() noexcept;
};
