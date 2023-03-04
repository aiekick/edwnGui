#include "Wnd.hpp"
EWindow wnd;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static bool Changed = false;
	switch (msg) {
	case WM_SETCURSOR:
		if (EGui.SettingCursor) {
			SetCursor(EGui.CurrentCursor);
			EGui.SettingCursor = false;
		}
		else
			SetCursor(EGui.cursorArrow);

		break;
	case WM_EXITSIZEMOVE:
		if (EGui.Device != NULL && wParam != SIZE_MINIMIZED)
			graphics.Update(lParam);
		break;
	case WM_SIZE:
		if (Changed) {
			Changed = false;

			graphics.Update(lParam);
		}
		if (wParam == SIZE_MAXIMIZED && EGui.Device != NULL) {
			Changed = true;

			graphics.Update(lParam);
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

bool EWindow::CreateGuiWindow(std::string title, bool Invisible, Vec2 wPos, Vec2 wSize)
{
	if (wPos == Vec2(0, 0)) wPos = Pos;
	if (wSize == Vec2(0, 0)) wSize = Size;

	wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, title.c_str(), NULL };
	RegisterClassEx(&wc);
	EGui.hwnd = CreateWindowA(wc.lpszClassName, title.c_str(), Invisible ? WS_POPUP : WS_OVERLAPPEDWINDOW, Pos.x, Pos.y, Size.x, Size.y, NULL, NULL, wc.hInstance, NULL);

	//Requires dmwwrite
	//MARGINS margins = { -1 };
	//DwmExtendFrameIntoClientArea(hwnd, &margins);

	this->hwnd = EGui.hwnd;

	if (Invisible)
		EGui.NoWindowHeader = true;

	EGui.SetWindowed(!Invisible);

	return true;
}

bool EWindow::IsWindowParent() {
	if (EGui.hwnd == GetForegroundWindow())
		return true;

	return false;
}