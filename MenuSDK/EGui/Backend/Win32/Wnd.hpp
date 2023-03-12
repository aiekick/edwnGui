#pragma once
#include "../../EGui.hpp"

struct EWindow {
	bool CreateGuiWindow(std::string title, Vec2 wPos = { 0 ,0 }, Vec2 wSize = { 0, 0 });
	bool IsWindowParent();
	Vec2 GetWindowPos();
	Vec2 GetWindowSize();

	void CreateGraphicsWindow(std::string title, Vec2 wPos = { 0 ,0 }, Vec2 wSize = { 0, 0 }) {
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

	Vec2 Size = { 1280, 800 };
	Vec2 Pos = { 100, 100 };
};

extern EWindow wnd;