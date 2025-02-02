#pragma once
#include "../../EGui.hpp"

struct EWindow {
	bool CreateGuiWindow(std::string title, Vec2 wPos = { 0 ,0 }, Vec2 wSize = { 0, 0 });
	bool DispatchMessages();

	bool IsWindowParent();
	bool IsWindowed(HWND hWnd);
	Vec2 GetWindowPos();
	Vec2 GetWindowSize();

	void CreateGraphicsWindow(std::string title, Vec2 wSize = { 0 ,0 }, Vec2 wPos = { 0, 0 }) {
		CreateGuiWindow(title, wPos, wSize);

		::ShowWindow(hwnd, SW_SHOWDEFAULT);
		::UpdateWindow(hwnd);
	}

	void SetGraphicsWindowTitle(std::string title) {
		SetWindowTextA(hwnd, title.c_str());
	}

	void SetGraphicsWindowTitle(const char* title) {
		SetWindowTextA(hwnd, title);
	}

	HWND hwnd;
	vector<HWND> wnds;
	WNDCLASSEX wc;
};

extern EWindow wnd;