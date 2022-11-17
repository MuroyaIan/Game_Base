
//===== �C���N���[�h�� =====
#include <Startup.h>

#ifdef APP_WIN64

#include <App_Win64.h>

#endif // APP_WIN64

//#ifdef DIRECTX_11
//#
//#elif defined(DIRECTX_12)
//#
//#endif // DIRECTX_11

//***** �G���g���[�|�C���g *****
#ifdef APP_WIN64

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
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//���������[�N���o

		//�A�v���P�[�V�������s
		return APP_64{}.Run();
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

#endif // APP_WIN64
