
//===== インクルード部 =====
#include <Startup.h>
#include <App.h>

//***** エントリーポイント *****
#ifdef _WIN64

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

#ifdef _DEBUG

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//メモリリーク検出

#endif // _DEBUG

		//アプリケーション実行
		int wParam = APP{}.Run();

#ifdef _DEBUG

		{
			//エラー処理
			Microsoft::WRL::ComPtr<IDXGIDebug1> pDebugDxgi;
			if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&pDebugDxgi))))
				pDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		}

#endif // _DEBUG

		//終了処理
		return wParam;
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

#endif // _WIN64
