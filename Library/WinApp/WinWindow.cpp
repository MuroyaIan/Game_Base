
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
CT_IW_WIN::CT_IW_WIN(const LPCWSTR& windowName, const int& nWndWidth, const int& nWndHeight, const int& nWndPosX, const int& nWndPosY) : CT_IF_WINDOW(),
	m_AppInst(GetModuleHandle(nullptr)), m_PosX(nWndPosX), m_PosY(nWndPosY), m_Width(nWndWidth), m_Height(nWndHeight), m_WinHandle(nullptr),
	m_bDrawCursor(true), m_RawBuffer(0), m_bUseImgui(false)
{
	//�E�B���h�E�N���X�o�^
	WNDCLASSEX l_WindowClass{};
	l_WindowClass.cbSize = static_cast<UINT>(sizeof(WNDCLASSEX));
	l_WindowClass.style = CS_OWNDC;
	l_WindowClass.lpfnWndProc = WndProc_Init;
	l_WindowClass.cbClsExtra = 0;
	l_WindowClass.cbWndExtra = 0;
	l_WindowClass.hInstance = m_AppInst;
	l_WindowClass.hIcon = LoadIcon(m_AppInst, MAKEINTRESOURCE(IDI_ICON_BASE));			//�A�C�R��
	l_WindowClass.hCursor = LoadCursor(m_AppInst, MAKEINTRESOURCE(IDC_CURSOR_BASE));		//�J�[�\��
	l_WindowClass.hbrBackground = nullptr;
	l_WindowClass.lpszMenuName = nullptr;
	l_WindowClass.lpszClassName = c_Class_Name;												//�N���X��
	l_WindowClass.hIconSm = LoadIcon(m_AppInst, MAKEINTRESOURCE(IDI_ICON_BASE));			//���A�C�R��
	if (!RegisterClassEx(&l_WindowClass))
		throw ERROR_DEFAULT();

	//�T�C�Y�Z�o
	DWORD l_ExStyle = 0;
	DWORD l_Style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	RECT l_WinRect = { 0L, 0L, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };	//Client�̈�T�C�Y�ݒ�
	if (!AdjustWindowRectEx(&l_WinRect, l_Style, false, l_ExStyle))							//Window�T�C�Y�Z�o
		throw ERROR_DEFAULT();
	int l_Width = l_WinRect.right - l_WinRect.left;
	int l_Height = l_WinRect.bottom - l_WinRect.top;												//���E������ݒ�

	//Instance�쐬
	m_WinHandle = CreateWindowEx(
		l_ExStyle, c_Class_Name, windowName, l_Style,
		m_PosX, m_PosY, l_Width, l_Height,
		nullptr, nullptr, m_AppInst, this);
	if (!m_WinHandle)
		throw ERROR_DEFAULT();

	//Window�\��
	ShowWindow(m_WinHandle, SW_SHOW);
	UpdateWindow(m_WinHandle);

	//IME������
	ImmAssociateContext(m_WinHandle, nullptr);

	//RawInput�f�o�C�X�o�^
	RAWINPUTDEVICE l_Rid{};
	l_Rid.usUsagePage = 0x01;		//�}�E�X���ʗp
	l_Rid.usUsage = 0x02;			//�}�E�X���ʗp
	l_Rid.dwFlags = 0;
	l_Rid.hwndTarget = nullptr;
	if (!RegisterRawInputDevices(&l_Rid, 1, sizeof(l_Rid)))
		throw ERROR_EX2("RawInput : Fail to init");

#ifdef IMGUI

	//IMGUI������
	if (!ImGui_ImplWin32_Init(m_WinHandle))
		throw ERROR_EX2("IMGUI : Fail to init");

#endif // IMGUI

}

CT_IW_WIN::~CT_IW_WIN() noexcept(false)
{

#ifdef IMGUI

	//IMGUI�I��
	ImGui_ImplWin32_Shutdown();

#endif // IMGUI

	//Instance�j��
	if(!DestroyWindow(m_WinHandle))
		throw ERROR_DEFAULT();

	//�E�B���h�E�N���X�o�^����
	if(!UnregisterClass(c_Class_Name, m_AppInst))
		throw ERROR_DEFAULT();
}

//�g�����X�t�H�[��
void CT_IW_WIN::Transform(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth, const int& nWndHeight)
{
	//�T�C�Y�擾
	RECT l_ClientRect;								//�N���C�A���g�̈�
	if (!GetClientRect(m_WinHandle, &l_ClientRect))
		throw ERROR_DEFAULT();
	l_ClientRect.right -= l_ClientRect.left;
	l_ClientRect.bottom -= l_ClientRect.top;
	RECT l_WindowRect;								//��N���C�A���g�̈�
	if (!GetWindowRect(m_WinHandle, &l_WindowRect))
		throw ERROR_DEFAULT();
	l_WindowRect.right -= l_WindowRect.left;
	l_WindowRect.bottom -= l_WindowRect.top;

	//�T�C�Y�X�V
	int l_Width, l_Height;
	if (nWndWidth <= 0)
		l_Width = l_WindowRect.right;
	else {
		l_Width = l_WindowRect.right - l_ClientRect.right + nWndWidth;
		m_Width = nWndWidth;
	}
	if (nWndHeight <= 0)
		l_Height = l_WindowRect.bottom;
	else {
		l_Height = l_WindowRect.bottom - l_ClientRect.bottom + nWndHeight;
		m_Height = nWndHeight;
	}

	//Window�ړ�
	if (!SetWindowPos(m_WinHandle, HWND_TOP, nWndPosX, nWndPosY, l_Width, l_Height, SWP_SHOWWINDOW))
		throw ERROR_DEFAULT();
}

//�^�C�g���o��
void CT_IW_WIN::TitlePrint(const std::string& text) const
{
	const std::string& l_TitleName = text;
	if (!SetWindowTextA(m_WinHandle, l_TitleName.c_str()))
		throw ERROR_DEFAULT();
}

//�^�C�g���o�́i�}�E�X���W�j
void CT_IW_WIN::TitlePrint_MousePos() const
{
	//���b�Z�[�W�{�b�N�X�\���o�O����
	auto [x, y] = m_Mouse.GetPos();
	std::ostringstream l_Oss;
	l_Oss << "MousePos:(" << x << ", " << y << ")";
	TitlePrint(l_Oss.str());
}

//�^�C�g���o�́i�z�C�[���l�j
void CT_IW_WIN::TitlePrint_WheelVal()
{
	while (!m_Mouse.IsEmpty()) {

		//�ϐ��錾
		const CT_MOUSE_EVENTS l_Event = m_Mouse.ReadBuffer();
		static int l_Cnt = 0;
		std::ostringstream l_Oss;

		//�X�V����
		switch (l_Event.GetType()) {
			case ET_MOUSE_STATUS::me_WheelUp:
				l_Cnt++;
				l_Oss << "Wheel Val : " << l_Cnt;
				TitlePrint(l_Oss.str());
				break;
			case ET_MOUSE_STATUS::me_WheelDown:
				l_Cnt--;
				l_Oss << "Wheel Val : " << l_Cnt;
				TitlePrint(l_Oss.str());
				break;
			default:
				l_Oss << "Wheel Val : " << l_Cnt;
				TitlePrint(l_Oss.str());
				break;
		}
	}
}

//�}�E�X�g�p�E�s�g�p
void CT_IW_WIN::EnableCursor() noexcept
{
	m_bDrawCursor = true;
	ShowCursor();
	UnlockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

void CT_IW_WIN::DisableCursor() noexcept
{
	m_bDrawCursor = false;
	HideCursor();
	LockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

//�}�E�X�g�p��Ԋm�F
bool CT_IW_WIN::IsUsingCursor() const noexcept
{
	return m_bDrawCursor;
}

//WndProc������
LRESULT CALLBACK CT_IW_WIN::WndProc_Init(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) noexcept
{
	//Instance�쐬��
	if (uMsg == WM_NCCREATE)
	{
		//Instance�|�C���^���擾
		const CREATESTRUCT* const l_pCreateInfo = reinterpret_cast<CREATESTRUCT*>(lParam);
		const auto l_pGameWnd = static_cast<CT_IW_WIN*>(l_pCreateInfo->lpCreateParams);

		//Instance�|�C���^��WinAPI�̃��[�U�f�[�^�Ƃ��ēo�^�iWinAPI�̓N���X�̃����o�֐���F���ł��Ȃ��ׁj
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(l_pGameWnd));

		//�������Ȍ��WndProc�Ăяo���֐���ݒ�
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CT_IW_WIN::WndProc_Call));

		//WndProc�{����
		return l_pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}

	//WM_NCCREATE�ȑO�̏�����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//WndProc�Ăяo��
LRESULT CALLBACK CT_IW_WIN::WndProc_Call(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) noexcept
{
	//Instance�|�C���^���擾
	const auto l_pGameWnd = reinterpret_cast<CT_IW_WIN*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//WndProc�{����
	return l_pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

//WndProc�{����
LRESULT CT_IW_WIN::WndProc(const HWND& hWnd, const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam) noexcept
{

#ifdef IMGUI

	//IMGUI�p
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	const ImGuiIO& l_Imgui = ImGui::GetIO();

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

			if (l_Imgui.WantCaptureKeyboard) {									//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			switch (wParam) {
				case VK_ESCAPE:												//�uESC�v�˃E�B���h�E�I��
					PostMessage(hWnd, WM_CLOSE, 0, 0);
				default: ;
			}
			if (!(lParam & 0x40000000))
				m_Keyboard.KeyPressed(static_cast<unsigned char>(wParam));	//�L�[��������
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:

#ifdef IMGUI

			if (l_Imgui.WantCaptureKeyboard) {									//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Keyboard.KeyReleased(static_cast<unsigned char>(wParam));		//�L�[�𗣂���
			break;
		case WM_CHAR:

#ifdef IMGUI

			if (l_Imgui.WantCaptureKeyboard) {									//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
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

			if (l_Imgui.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const POINTS l_Points = MAKEPOINTS(lParam);
				if (l_Points.x >= 0 && l_Points.x < m_Width && l_Points.y >= 0 && l_Points.y < m_Height) {	//�E�B���h�E���̏ꍇ
					m_Mouse.MouseMove(l_Points.x, l_Points.y);									//���W�L�^
					if (!m_Mouse.IsInWindow()) {									//�}�E�X�L���v�`���[�I��
						SetCapture(hWnd);
						m_Mouse.EnterWindow();
					}
				}
				else {																//�E�B���h�E�O�̏ꍇ
					if (wParam & (MK_LBUTTON | MK_RBUTTON))							//�N���b�N���Ă�ꍇ
						m_Mouse.MouseMove(l_Points.x, l_Points.y);								//���W�L�^
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

			if (l_Imgui.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Mouse.LeftPressed();
			break;
		case WM_LBUTTONUP:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const POINTS l_Points = MAKEPOINTS(lParam);
				if (l_Points.x < 0 || l_Points.x >= m_Width || l_Points.y < 0 || l_Points.y >= m_Height) {	//�E�B���h�E�O�̏ꍇ
					ReleaseCapture();												//�}�E�X�L���v�`���[�I�t
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.LeftReleased();
			break;
		case WM_RBUTTONDOWN:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Mouse.RightPressed();
			break;
		case WM_RBUTTONUP:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const POINTS l_Points = MAKEPOINTS(lParam);
				if (l_Points.x < 0 || l_Points.x >= m_Width || l_Points.y < 0 || l_Points.y >= m_Height) {	//�E�B���h�E�O�̏ꍇ
					ReleaseCapture();												//�}�E�X�L���v�`���[�I�t
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.RightReleased();
			break;
		case WM_MOUSEWHEEL:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse) {												//IMGUI���͐ؑ�
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const int l_Delta = GET_WHEEL_DELTA_WPARAM(wParam);					//�z�C�[������ʎ擾(���b�Z�[�W����+-120)
				m_Mouse.WheelProc(l_Delta);
			}
			break;

		//RawInput����
		case WM_INPUT:
		{
			if (!m_Mouse.IsUsingRawInput())
				break;
			UINT l_Size{};

			//���͏��̃T�C�Y���擾
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &l_Size, sizeof(RAWINPUTHEADER)) == static_cast<UINT>(-1))
				break;	//�G���[���b�Z�[�W
			m_RawBuffer.resize(l_Size);

			//���͏��Ǎ�
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, m_RawBuffer.data(), &l_Size, sizeof(RAWINPUTHEADER)) != l_Size)
				break;	//�G���[���b�Z�[�W

			//�f�[�^���
			auto& l_RawInput = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
			if (l_RawInput.header.dwType == RIM_TYPEMOUSE && (l_RawInput.data.mouse.lLastX != 0 || l_RawInput.data.mouse.lLastY != 0))
				m_Mouse.GetRawDelta(l_RawInput.data.mouse.lLastX, l_RawInput.data.mouse.lLastY);

			break;
		}

		default:
			break;
	}

	//��{������T�|�[�g
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//�}�E�X��\���E�\��
void CT_IW_WIN::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void CT_IW_WIN::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

//�}�E�X���E�B���h�E���Ƀ��b�N�E���b�N����
void CT_IW_WIN::LockCursor() const noexcept
{
	RECT l_Rect;
	GetClientRect(m_WinHandle, &l_Rect);
	MapWindowPoints(m_WinHandle, nullptr, reinterpret_cast<POINT*>(&l_Rect), 2);	//�E�B���h�E�����W���X�N���[�����W�֕ϊ�
	ClipCursor(&l_Rect);															//�}�E�X���w��͈͓��Ƀ��b�N����
}

void CT_IW_WIN::UnlockCursor() const noexcept
{
	ClipCursor(nullptr);

	//�J�[�\������ʒ��S�֖߂�
	RECT l_Rect;
	GetClientRect(m_WinHandle, &l_Rect);
	MapWindowPoints(m_WinHandle, nullptr, reinterpret_cast<POINT*>(&l_Rect), 2);	//�E�B���h�E�����W���X�N���[�����W�֕ϊ�
	const int l_X = l_Rect.left + m_Width / 2;
	const int l_Y = l_Rect.top + m_Height / 2;
	SetCursorPos(l_X, l_Y);
}
