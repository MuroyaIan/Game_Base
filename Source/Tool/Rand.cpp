
//===== インクルード部 =====
#include <Tool/Rand.h>
#include <WinApp/WinEO.h>		//エラー出力

//===== 静的メンバ変数 =====
RAND RAND_MAKER::m_Rand{};		//乱数生成器

//===== クラス実装 =====

//***** 乱数生成クラス *****
RAND_MAKER::RAND_MAKER() noexcept
{
}
RAND_MAKER::~RAND_MAKER() noexcept
{
}

//生成器初期化
void RAND_MAKER::InitMaker() noexcept
{
	std::random_device Seed;	//シード生成
	m_Rand.seed(Seed());		//乱数のシード指定
}

//整数生成
int RAND_MAKER::MakeRand_Int(int Min, int Max)
{
	//***** 例外処理 *****
	if (Min > Max)
		throw ERROR_EX2("【警告】乱数の指定範囲が不適切！");

	//***** 生成処理 *****
	RandRange_Int RandDist(Min, Max);	//一様分布を指定
	return RandDist(m_Rand);			//乱数生成された整数を返す
}

//小数生成
float RAND_MAKER::MakeRand_Float(float Min, float Max)
{
	//***** 例外処理 *****
	if (Min > Max)
		throw ERROR_EX2("【警告】乱数の指定範囲が不適切！");

	//***** 生成処理 *****
	RandRange_Float RandDist(Min, Max);		//一様分布を指定
	return RandDist(m_Rand);				//乱数生成された整数を返す
}
