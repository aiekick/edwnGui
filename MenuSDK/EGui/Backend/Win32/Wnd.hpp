#pragma once
#include "../../EGui.hpp"

struct EWindow {
	bool CreateGuiWindow(std::string title, bool Invisible, Vec2 wPos = { 0 ,0 }, Vec2 wSize = { 0, 0 });
	bool IsWindowParent();

	void ExampleWindow(std::string title, bool Invisible, Vec2 wPos = { 0 ,0 }, Vec2 wSize = { 0, 0 }) {
		CreateGuiWindow(title, Invisible, wPos, wSize);

		::ShowWindow(hwnd, SW_SHOWDEFAULT);
		::UpdateWindow(hwnd);
	}

	HWND hwnd;
	vector<HWND> wnds;
	WNDCLASSEX wc;

	Vec2 Size = { 1280, 800 };
	Vec2 Pos = { 100, 100 };
};

extern EWindow wnd;