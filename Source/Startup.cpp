
//===== インクルード部 =====
#include <Startup.h>

#ifdef APP_WIN64

#include <App_Win64.h>

#endif // APP_WIN64

//#ifdef DIRECTX_11
//#
//#elif defined(DIRECTX_12)
//#
//#endif // DIRECTX_11

//***** エントリーポイント *****
#ifdef APP_WIN64

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,			//Instanceハンドル
	_In_opt_ HINSTANCE hPrevInstance,	//基本NULL
	_In_ LPSTR lpCmdLine,				//コマンドライン
	_In_ int nCmdShow)
{
	try
	{
		//前処理
		(void)hInstance;
		(void)hPrevInstance;
		(void)lpCmdLine;
		(void)nCmdShow;
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//メモリリーク検出

		//アプリケーション実行
		return APP_64{}.Run();
	}
	catch (const WIN_EO& e)
	{
		//Windowエラー
		MessageBoxA(nullptr, e.what(), e.GetType().c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		//標準エラー
		MessageBoxA(nullptr, e.what(), "標準的例外（エラー）", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		//未定義エラー
		MessageBoxA(nullptr, "詳細不明です。", "不明のエラー", MB_OK | MB_ICONEXCLAMATION);
	}

	//異常終了
	return -1;
}

#endif // APP_WIN64
