
//===== インクルード部 =====
#include <Tool/TextIO.h>

//===== プロトタイプ宣言 =====

//文字列分割
std::vector<std::string> gText::Split(const std::string& Text, char Sep)
{
	//変数宣言
	std::vector<std::string> aStr;  //分割後文字列配列
	std::stringstream ss(Text);     //分割前文字列
	std::string strBuff;            //分割用文字列

	//分割処理
	while (std::getline(ss, strBuff, Sep))
		aStr.push_back(strBuff);
	return aStr;
}
