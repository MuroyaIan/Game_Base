//===== インクルード部 =====
#include <Startup.h>
#include <App.h>

//***** エントリーポイント *****
#ifdef _WIN64

/**
 * エントリーポイント(Win)
 *
 * \param hInstance
 * \param hPrevInstance
 * \param lpCmdLine
 * \param nCmdShow
 * \return int
 */
int CALLBACK WinMain(_In_ const HINSTANCE hInstance,        //Instanceハンドル
					_In_opt_ const HINSTANCE hPrevInstance, //基本NULL
					_In_ const LPSTR lpCmdLine,             //コマンドライン
					_In_ const int nCmdShow
)
{
	try
	{
		//前処理
		(void)hInstance;
		(void)hPrevInstance;
		(void)lpCmdLine;
		(void)nCmdShow;

#ifdef _DEBUG

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //メモリリーク検出

#endif // _DEBUG

		//アプリケーション実行
		const int l_WParam = APP{}.Run();

#ifdef _DEBUG

		{
			//エラー処理
			ComPtr<IDXGIDebug1> l_PDebugDxgi;
			if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&l_PDebugDxgi))))
				l_PDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		}

#endif // _DEBUG

		//終了処理
		return l_WParam;
	}
	catch (const CT_EO_WIN& l_Error)
	{
		//Windowエラー
		MessageBoxA(nullptr, l_Error.what(), l_Error.GetType().c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& l_Error)
	{
		//標準エラー
		MessageBoxA(nullptr, l_Error.what(), "標準的例外（エラー）", MB_OK | MB_ICONEXCLAMATION);
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
