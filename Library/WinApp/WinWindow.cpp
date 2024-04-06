
//===== �C���N���[�h�� =====
#include <WinApp/WinWindow.h>
#include <resource.h>
#include <Startup.h>

#ifdef IMGUI
#
#	include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#
#endif // IMGUI

//===== �ǉ����C�u���� =====
#pragma comment(lib, "imm32")

//===== �N���X���� =====
WIN_WINDOW::WIN_WINDOW(LPCWSTR WindowName, int nWndWidth, int nWndHeight, int nWndPosX, int nWndPosY) : IF_WINDOW(),
	m_hAppInst(GetModuleHandle(nullptr)), m_PosX(nWndPosX), m_PosY(nWndPosY), m_Width(nWndWidth), m_Height(nWndHeight), m_hWindow(nullptr),
	m_bDrawCursor(true), m_RawBuffer(0), m_useImgui(false)
{
	//�E�B���h�E�N���X�o�^
	WNDCLASSEX WindowClass{};
	WindowClass.cbSize = static_cast<UINT>(sizeof(WNDCLASSEX));
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = WIN_WINDOW::WndProc_Init;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = m_hAppInst;
	WindowClass.hIcon = LoadIcon(m_hAppInst, MAKEINTRESOURCE(IDI_ICON_BASE));			//�A�C�R��
	WindowClass.hCursor = LoadCursor(m_hAppInst, MAKEINTRESOURCE(IDC_CURSOR_BASE));		//�J�[�\��
	WindowClass.hbrBackground = nullptr;
	WindowClass.lpszMenuName = nullptr;
	WindowClass.lpszClassName = CLASS_NAME;												//�N���X��
	WindowClass.hIconSm = LoadIcon(m_hAppInst, MAKEINTRESOURCE(IDI_ICON_BASE));			//���A�C�R��
	if (!RegisterClassEx(&WindowClass))
		throw ERROR_DEFAULT();

	//�T�C�Y�Z�o
	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	RECT rcWnd = { 0L, 0L, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };	//Client�̈�T�C�Y�ݒ�
	if (!AdjustWindowRectEx(&rcWnd, dwStyle, false, dwExStyle))							//Window�T�C�Y�Z�o
		throw ERROR_DEFAULT();
	int nWidth = rcWnd.right - rcWnd.left;
	int nHeight = rcWnd.bottom - rcWnd.top;												//���E������ݒ�

	//Instance�쐬
	m_hWindow = CreateWindowEx(
		dwExStyle, CLASS_NAME, WindowName, dwStyle,
		m_PosX, m_PosY, nWidth, nHeight,
		nullptr, nullptr, m_hAppInst, this);
	if (!m_hWindow)
		throw ERROR_DEFAULT();

	//Window�\��
	ShowWindow(m_hWindow, SW_SHOW);
	UpdateWindow(m_hWindow);

	//IME������
	ImmAssociateContext(m_hWindow, nullptr);

	//RawInput�f�o�C�X�o�^
	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 0x01;		//�}�E�X���ʗp
	rid.usUsage = 0x02;			//�}�E�X���ʗp
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
		throw ERROR_EX2("RawInput���������s");

#ifdef IMGUI

	//IMGUI������
	if (!ImGui_ImplWin32_Init(m_hWindow))
		throw ERROR_EX2("IMGUI���������s");

#endif // IMGUI

}

WIN_WINDOW::~WIN_WINDOW() noexcept(false)
{

#ifdef IMGUI

	//IMGUI�I��
	ImGui_ImplWin32_Shutdown();

#endif // IMGUI

	//Instance�j��
	if(!DestroyWindow(m_hWindow))
		throw ERROR_DEFAULT();

	//�E�B���h�E�N���X�o�^����
	if(!UnregisterClass(CLASS_NAME, m_hAppInst))
		throw ERROR_DEFAULT();
}

//�g�����X�t�H�[��
void WIN_WINDOW::Transform(int nWndPosX, int nWndPosY, int nWndWidth, int nWndHeight)
{
	//�T�C�Y�擾
	RECT rcClient;								//�N���C�A���g�̈�
	if (!GetClientRect(m_hWindow, &rcClient))
		throw ERROR_DEFAULT();
	rcClient.right -= rcClient.left;
	rcClient.bottom -= rcClient.top;
	RECT rcWindow;								//��N���C�A���g�̈�
	if (!GetWindowRect(m_hWindow, &rcWindow))
		throw ERROR_DEFAULT();
	rcWindow.right -= rcWindow.left;
	rcWindow.bottom -= rcWindow.top;

	//�T�C�Y�X�V
	int nWidth = 0;
	int nHeight = 0;
	if (nWndWidth <= 0)
		nWidth = rcWindow.right;
	else {
		nWidth = rcWindow.right - rcClient.right + nWndWidth;
		m_Width = nWndWidth;
	}
	if (nWndHeight <= 0)
		nHeight = rcWindow.bottom;
	else {
		nHeight = rcWindow.bottom - rcClient.bottom + nWndHeight;
		m_Height = nWndHeight;
	}

	//Window�ړ�
	if (!SetWindowPos(m_hWindow, HWND_TOP, nWndPosX, nWndPosY, nWidth, nHeight, SWP_SHOWWINDOW))
		throw ERROR_DEFAULT();
}

//�^�C�g���o��
void WIN_WINDOW::TitlePrint(const std::string& Text) const
{
	std::string TitleName = Text;
	if (!SetWindowTextA(m_hWindow, TitleName.c_str()))
		throw ERROR_DEFAULT();
}

//�^�C�g���o�́i�}�E�X���W�j
void WIN_WINDOW::TitlePrint_MousePos() const
{
	//���b�Z�[�W�{�b�N�X�\���o�O����
	auto [x, y] = m_Mouse.GetPos();
	std::ostringstream oss;
	oss << "MousePos:(" << x << ", " << y << ")";
	TitlePrint(oss.str());
}

//�^�C�g���o�́i�z�C�[���l�j
void WIN_WINDOW::TitlePrint_WheelVal()
{
	while (!m_Mouse.IsEmpty()) {

		//�ϐ��錾
		const CT_MOUSE_EVENTS Event = m_Mouse.ReadBuffer();
		static int nCnt = 0;
		std::ostringstream oss;

		//�X�V����
		switch (Event.GetType()) {
			case ET_MOUSE_STATUS::me_WheelUp:
				nCnt++;
				oss << "�z�C�[���l�F" << nCnt;
				TitlePrint(oss.str());
				break;
			case ET_MOUSE_STATUS::me_WheelDown:
				nCnt--;
				oss << "�z�C�[���l�F" << nCnt;
				TitlePrint(oss.str());
				break;
			default:
				oss << "�z�C�[���l�F" << nCnt;
				TitlePrint(oss.str());
				break;
		}
	}
}

//�}�E�X�g�p�E�s�g�p
void WIN_WINDOW::EnableCursor() noexcept
{
	m_bDrawCursor = true;
	ShowCursor();
	UnlockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

void WIN_WINDOW::DisableCursor() noexcept
{
	m_bDrawCursor = false;
	HideCursor();
	LockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

//�}�E�X�g�p��Ԋm�F
bool WIN_WINDOW::IsUsingCursor() const noexcept
{
	return m_bDrawCursor;
}

//WndProc������
LRESULT CALLBACK WIN_WINDOW::WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Instance�쐬��
	if (uMsg == WM_NCCREATE)
	{
		//Instance�|�C���^���擾
		const CREATESTRUCT* const pCreateInfo = reinterpret_cast<CREATESTRUCT*>(lParam);
		WIN_WINDOW* const pGameWnd = static_cast<WIN_WINDOW*>(pCreateInfo->lpCreateParams);

		//Instance�|�C���^��WinAPI�̃��[�U�f�[�^�Ƃ��ēo�^�iWinAPI�̓N���X�̃����o�֐���F���ł��Ȃ��ׁj
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pGameWnd));

		//�������Ȍ��WndProc�Ăяo���֐���ݒ�
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WIN_WINDOW::WndProc_Call));

		//WndProc�{����
		return pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}

	//WM_NCCREATE�ȑO�̏�����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//WndProc�Ăяo��
LRESULT CALLBACK WIN_WINDOW::WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Instance�|�C���^���擾
	WIN_WINDOW* const pGameWnd = reinterpret_cast<WIN_WINDOW*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//WndProc�{����
	return pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

//WndProc�{����
LRESULT WIN_WINDOW::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{

#ifdef IMGUI

	//IMGUI�p
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	const ImGuiIO& io = ImGui::GetIO();

#endif // IMGUI

	switch (uMsg) {
		case WM_CLOSE:
			if (MessageBox(hWnd, L"�E�B���h�E����܂����H", L"�I���m�F", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
				return 0;
			PostQuitMessage(0);				//WM_QUIT�𐶐�
			return 0;						//�f�X�g���N�^�Ăяo��

		//�E�B���h�E�A�N�e�B�u����
		case WM_ACTIVATE:

			//�}�E�X��ԍX�V
			if (!m_bDrawCursor) {
				if (wParam & WA_ACTIVE) {
					LockCursor();			//�A�N�e�B�u�˃}�E�X���b�N
					HideCursor();
				}
				else {
					UnlockCursor();			//��A�N�e�B�u�˃}�E�X���b�N����
					ShowCursor();
				}
			}
			break;

		//�L�[�{�[�h����
		case WM_KILLFOCUS:
			m_Keyboard.ClearState();		//�E�B���h�E��A�N�e�B�u�˓��̓��Z�b�g
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:													//�uWM_SYSKEY�v�ˁuALT�v�ƁuF10�v��Ή�

#ifdef IMGUI

			if (io.WantCaptureKeyboard) {									//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			switch (wParam) {
				case VK_ESCAPE:												//�uESC�v�˃E�B���h�E�I��
					PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			if (!(lParam & 0x40000000))
				m_Keyboard.KeyPressed(static_cast<unsigned char>(wParam));	//�L�[��������
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:

#ifdef IMGUI

			if (io.WantCaptureKeyboard) {									//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Keyboard.KeyReleased(static_cast<unsigned char>(wParam));		//�L�[�𗣂���
			break;
		case WM_CHAR:

#ifdef IMGUI

			if (io.WantCaptureKeyboard) {									//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Keyboard.CharInput(static_cast<unsigned char>(wParam));		//�e�L�X�g����͂���
			break;

		//�}�E�X����
		case WM_MOUSEMOVE:

			//�}�E�X��\���̏ꍇ
			if (!m_bDrawCursor) {
				if (!m_Mouse.IsInWindow()) {	//�E�B���h�E�ɓ��������񃋁[�v�˃}�E�X���E�B���h�E�O����̏ꍇ
					SetCapture(hWnd);
					m_Mouse.EnterWindow();		//�L���v�`���[�I���A�}�E�X���b�Z�[�W�X�V
					HideCursor();				//�}�E�X��\��
				}
				break;							//�}�E�X���W�������X�L�b�v
			}

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				POINTS pt = MAKEPOINTS(lParam);
				if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height) {	//�E�B���h�E���̏ꍇ
					m_Mouse.MouseMove(pt.x, pt.y);									//���W�L�^
					if (!m_Mouse.IsInWindow()) {									//�}�E�X�L���v�`���[�I��
						SetCapture(hWnd);
						m_Mouse.EnterWindow();
					}
				}
				else {																//�E�B���h�E�O�̏ꍇ
					if (wParam & (MK_LBUTTON | MK_RBUTTON))							//�N���b�N���Ă�ꍇ
						m_Mouse.MouseMove(pt.x, pt.y);								//���W�L�^
					else {															//�N���b�N���ĂȂ��ꍇ
						ReleaseCapture();											//�}�E�X�L���v�`���[�I�t
						m_Mouse.LeaveWindow();
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			SetForegroundWindow(hWnd);												//�E�B���h�E���A�N�e�B�u
			if (!m_bDrawCursor) {													//�}�E�X��ԍX�V
				LockCursor();
				HideCursor();
			}

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Mouse.LeftPressed();
			break;
		case WM_LBUTTONUP:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				POINTS pt = MAKEPOINTS(lParam);
				if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height) {	//�E�B���h�E�O�̏ꍇ
					ReleaseCapture();												//�}�E�X�L���v�`���[�I�t
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.LeftReleased();
			break;
		case WM_RBUTTONDOWN:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Mouse.RightPressed();
			break;
		case WM_RBUTTONUP:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				POINTS pt = MAKEPOINTS(lParam);
				if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height) {	//�E�B���h�E�O�̏ꍇ
					ReleaseCapture();												//�}�E�X�L���v�`���[�I�t
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.RightReleased();
			break;
		case WM_MOUSEWHEEL:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				const int nDelta = GET_WHEEL_DELTA_WPARAM(wParam);					//�z�C�[������ʎ擾(���b�Z�[�W����+-120)
				m_Mouse.WheelProc(nDelta);
			}
			break;

		//RawInput����
		case WM_INPUT:
		{
			if (!m_Mouse.IsUsingRawInput())
				break;
			UINT size{};

			//���͏��̃T�C�Y���擾
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
				break;	//�G���[���b�Z�[�W
			m_RawBuffer.resize(size);

			//���͏��Ǎ�
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, m_RawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
				break;	//�G���[���b�Z�[�W

			//�f�[�^���
			auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
				m_Mouse.GetRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);

			break;
		}

		default:
			break;
	}

	//��{������T�|�[�g
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//�}�E�X��\���E�\��
void WIN_WINDOW::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void WIN_WINDOW::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

//�}�E�X���E�B���h�E���Ƀ��b�N�E���b�N����
void WIN_WINDOW::LockCursor() noexcept
{
	RECT rect;
	GetClientRect(m_hWindow, &rect);
	MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);	//�E�B���h�E�����W���X�N���[�����W�֕ϊ�
	ClipCursor(&rect);															//�}�E�X���w��͈͓��Ƀ��b�N����
}

void WIN_WINDOW::UnlockCursor() noexcept
{
	ClipCursor(nullptr);

	//�J�[�\������ʒ��S�֖߂�
	RECT rect;
	GetClientRect(m_hWindow, &rect);
	MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);	//�E�B���h�E�����W���X�N���[�����W�֕ϊ�
	int x = rect.left + m_Width / 2;
	int y = rect.top + m_Height / 2;
	SetCursorPos(x, y);
}
