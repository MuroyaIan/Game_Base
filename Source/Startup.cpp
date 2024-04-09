
//===== �C���N���[�h�� =====
#include <Startup.h>
#include <App.h>

//***** �G���g���[�|�C���g *****
#ifdef _WIN64

/**
 * �G���g���[�|�C���g(Win)
 *
 * \param hInstance
 * \param hPrevInstance
 * \param lpCmdLine
 * \param nCmdShow
 * \return int
 */
int CALLBACK WinMain(
	_In_ const HINSTANCE hInstance,         //Instance�n���h��
	_In_opt_ const HINSTANCE hPrevInstance, //��{NULL
	_In_ const LPSTR lpCmdLine,             //�R�}���h���C��
	_In_ const int nCmdShow)
{
	try
	{
		//�O����
		(void)hInstance;
		(void)hPrevInstance;
		(void)lpCmdLine;
		(void)nCmdShow;

#ifdef _DEBUG

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//���������[�N���o

#endif // _DEBUG

		//�A�v���P�[�V�������s
		const int l_WParam = APP{}.Run();

#ifdef _DEBUG

		{
			//�G���[����
			Microsoft::WRL::ComPtr<IDXGIDebug1> l_PDebugDxgi;
			if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&l_PDebugDxgi))))
				l_PDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		}

#endif // _DEBUG

		//�I������
		return l_WParam;
	}
	catch (const CT_EO_WIN& l_Error)
	{
		//Window�G���[
		MessageBoxA(nullptr, l_Error.what(), l_Error.GetType().c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& l_Error)
	{
		//�W���G���[
		MessageBoxA(nullptr, l_Error.what(), "�W���I��O�i�G���[�j", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		//����`�G���[
		MessageBoxA(nullptr, "�ڍוs���ł��B", "�s���̃G���[", MB_OK | MB_ICONEXCLAMATION);
	}

	//�ُ�I��
	return -1;
}

#endif // _WIN64
