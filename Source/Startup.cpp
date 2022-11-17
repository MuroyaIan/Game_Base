
//===== �C���N���[�h�� =====
#include <Startup.h>
#include <App.h>

//***** �G���g���[�|�C���g *****
#ifdef _WIN64

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,			//Instance�n���h��
	_In_opt_ HINSTANCE hPrevInstance,	//��{NULL
	_In_ LPSTR lpCmdLine,				//�R�}���h���C��
	_In_ int nCmdShow)
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
		int wParam = APP{}.Run();

#ifdef _DEBUG

		{
			//�G���[����
			Microsoft::WRL::ComPtr<IDXGIDebug1> pDebugDxgi;
			if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&pDebugDxgi))))
				pDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		}

#endif // _DEBUG

		//�I������
		return wParam;
	}
	catch (const WIN_EO& e)
	{
		//Window�G���[
		MessageBoxA(nullptr, e.what(), e.GetType().c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		//�W���G���[
		MessageBoxA(nullptr, e.what(), "�W���I��O�i�G���[�j", MB_OK | MB_ICONEXCLAMATION);
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
