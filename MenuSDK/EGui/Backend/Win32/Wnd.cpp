#include "Wnd.hpp"
EWindow wnd;

#if 0
//Put this were your wndproc function is and call it with the wndproc args. this is required for input.
extern LRESULT WINAPI edwnGui_ImplWin32_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT WINAPI edwnGui_ImplWin32_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static bool DeviceLost = false;
	float mouse_wheel_delta = 0.f;
	bool set_delta = false;
	switch (msg) {
	case WM_SETCURSOR:
		if (EGui.SettingCursor) {
			SetCursor(EGui.CurrentCursor);
			EGui.SettingCursor = false;
		}
		else
			SetCursor(EGui.CursorArrow);
		break;
	case WM_MOUSEMOVE:
		const POINTS p = MAKEPOINTS(lParam);
		Input.UpdateMousePos(Vec2(p.x, p.y));
		break;
	case WM_MOUSEWHEEL:
		mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 8 : -8;
		Input.UpdateMouseWheelDelta(mouse_wheel_delta);
		break;
	case WM_EXITSIZEMOVE:
		if (EGui.Device != NULL && wParam != SIZE_MINIMIZED)
			Graphics.OnDeviceLost(lParam);
		break;
	case WM_SIZE:
		if (DeviceLost) {
			DeviceLost = false;

			Graphics.OnDeviceLost(lParam);
		}
		if (wParam == SIZE_MAXIMIZED && EGui.Device != NULL) {
			DeviceLost = true;

			Graphics.OnDeviceLost(lParam);
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

bool EWindow::DispatchMessages() {
	MSG msg;
	std::memset(&msg, 0, sizeof(MSG));
	if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return false;
	}

	return true;
}

Vec2 EWindow::GetWindowPos() {
	RECT rect = { NULL };
	if (GetWindowRect(EGui.hwnd, &rect))
		return Vec2(rect.left, rect.top);

	return Vec2(0, 0);
}

Vec2 EWindow::GetWindowSize() {
	RECT rect = { NULL };
	if (GetClientRect(EGui.hwnd, &rect))
		return Vec2(rect.right - rect.left - 1, rect.bottom - rect.top - 1);

	return Vec2(0, 0);
}

bool EWindow::CreateGuiWindow(std::string title, Vec2 wPos, Vec2 wSize)
{
	if (wPos == Vec2(0, 0)) wPos = Pos;
	if (wSize == Vec2(0, 0)) wSize = Size;

	wc = { sizeof(wc), CS_CLASSDC, edwnGui_ImplWin32_WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, title.c_str(), NULL };
	RegisterClassEx(&wc);
	EGui.hwnd = CreateWindowA(wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, Pos.x, Pos.y, Size.x, Size.y, NULL, NULL, wc.hInstance, NULL);

	this->hwnd = EGui.hwnd;

	return true;
}

bool EWindow::IsWindowParent() {
	if (EGui.hwnd == GetForegroundWindow())
		return true;

	return false;
}

bool EWindow::IsWindowed(HWND hWnd) {
	RECT a, b;
	GetWindowRect(hWnd, &a);
	GetWindowRect(GetDesktopWindow(), &b);
	return !(a.left == b.left &&
		a.top == b.top &&
		a.right == b.right &&
		a.bottom == b.bottom);
}