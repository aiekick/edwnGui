#include "Wnd.hpp"
EWindow wnd;

#if 0
//Put this were your wndproc function is and call it with the wndproc args. this is required for input.
extern LRESULT WINAPI EGui_ImplWin32_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT WINAPI EGui_ImplWin32_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static bool DeviceLost = false;

	switch (msg) {
	case WM_SETCURSOR:
		if (EGui.SettingCursor) {
			SetCursor(EGui.CurrentCursor);
			EGui.SettingCursor = false;
		}
		else
			SetCursor(EGui.CursorArrow);

		break;
	case WM_EXITSIZEMOVE:
		if (EGui.Device != NULL && wParam != SIZE_MINIMIZED)
			graphics.OnDeviceLost(lParam);
		break;
	case WM_SIZE:
		if (DeviceLost) {
			DeviceLost = false;

			graphics.OnDeviceLost(lParam);
		}
		if (wParam == SIZE_MAXIMIZED && EGui.Device != NULL) {
			DeviceLost = true;

			graphics.OnDeviceLost(lParam);
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

Vec2 EWindow::GetWindowPos() {
	RECT rect = { NULL };
	if (GetWindowRect(EGui.hwnd, &rect))
		return Vec2(rect.left, rect.top);

	return Vec2(0, 0);
}

Vec2 EWindow::GetWindowSize() {
	RECT rect = { NULL };
	if (GetClientRect(EGui.hwnd, &rect)) // -1 because the outline is drawn there lol and getclientrect does not account for it
		return Vec2(rect.right - rect.left - 1, rect.bottom - rect.top - 1);

	return Vec2(0, 0);
}

bool EWindow::CreateGuiWindow(std::string title, Vec2 wPos, Vec2 wSize)
{
	if (wPos == Vec2(0, 0)) wPos = Pos;
	if (wSize == Vec2(0, 0)) wSize = Size;

	wc = { sizeof(wc), CS_CLASSDC, EGui_ImplWin32_WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, title.c_str(), NULL };
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