#pragma once
#define DIRECTINPUT_VERSION 0x800
#define EGUI_VERSION "1.0 open beta"

// Standard library includes
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>
#include <map>
#include <chrono>
#include <cmath>

// Windows-specific includes
#include <Windows.h>
#include <tchar.h>

// DirectX includes
#include <d3d9.h>
#include <d3dx9.h>

// DirectInput include
#include <dInput.h>

using namespace std;

// EGui source files
#include "Backend/Render/Classes/Color.hpp"
#include "Backend/Render/Classes/Vector.hpp"
#include "Backend/Render/Classes/Vector2D.hpp"
#include "Backend/Render/Classes/Rect.hpp"
#include "Backend/Render/Classes/Font.hpp"
#include "Backend/Render/Classes/Clip.hpp"
#include "Backend/Render/Textures/Textures.hpp"
#include "Backend/Utility/Math.hpp"
#include "Backend/Input/Input.hpp"
#include "Backend/Win32/wnd.hpp"
#include "Backend/Animation/Timing.hpp"
#include "Backend/Animation/Animation.hpp"
#include "Backend/Graphics/DirectX.hpp"
#include "Backend/Render/renderer.hpp"

// Pragma comments
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define PI 3.14159265359

struct guiStyle {
	float Alpha = 255.f;
	float DisabledAlpha = 150.f;
	float Padding = 5.f;
	float FrameRounding = 4.f;
	float ChildRounding = 4.f;
	float ElementRounding = 4.f;
};

struct guiColors {
	Color Accent = Color(255, 140, 140, 255);
	Color FrameBackColor = Color(25, 25, 25, 255);
	Color FrameHeaderColor = Color(35, 35, 35, 255);
	Color FrameBorderColor = Color(0, 0, 0, 255);
	Color ChildBgColor = Color(30, 30, 30, 255);
	Color ChildHeaderColor = Color(35, 35, 35, 255);
	Color TextColor = Color(250, 250, 250, 255);
	Color TextColorHovered = Color(255, 255, 255, 255);
	Color TextColorDisabled = Color(240, 240, 240, 255);
	Color ElementBackColor = Color(15, 15, 15, 255);
	Color ElementBorderColor = Color(0, 0, 0, 255);
	Color ElementColorHovered = Color(0, 0, 0, 0);
	Color ElementColorOn = Accent;
};

struct EGuiMain {
	// EGui -------------------------------------------------------------------------------------------------------
	string GetVersion() { return EGUI_VERSION; }

	// Window -----------------------------------------------------------------------------------------------------
	HWND hwnd = NULL;
	void SetWindowHandle(HWND hWnd) { hwnd = hWnd; }

	// Direct3D ---------------------------------------------------------------------------------------------------
	IDirect3D9* D3D = NULL;
	D3DPRESENT_PARAMETERS Paramaters = { };
	IDirect3DDevice9* Device = NULL;
	LPD3DXSPRITE Sprite = NULL;
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer = NULL;
	void SetDevice(IDirect3DDevice9* device) { Device = device; }
	IDirect3DDevice9* GetDevice() { return Device; }

	// EGui -------------------------------------------------------------------------------------------------------
	void Begin();
	void End();
	void DebugProfiler();
	void DemoWindow();

	/* for window hijacking */
	void Update();

	// Render -----------------------------------------------------------------------------------------------------
	void PreRender();
	void Render();

	// Resizing ---------------------------------------------------------------------------------------------------
	Vec2 Resizing(int id, Vec2 size);
	void SetResizingArea(int id, Vec2 pos, Vec2 size);
	Rect GetResizingArea(int id);
	bool InResizingArea(int id);
	bool IsResizing(int id);

	// Dragging ---------------------------------------------------------------------------------------------------
	Vec2 Dragging(int id, Vec2 pos, Vec2 size, bool CanDragOffscreen = true, bool Child = false, Vec2 SnapPos = { -1, -1 });
	bool IsDragging(int id);

	// Cursor -----------------------------------------------------------------------------------------------------
	bool SettingCursor = false;
	void SetCursorStyle(HCURSOR style) { SettingCursor = true; CurrentCursor = style; }
	HCURSOR CurrentCursor = NULL;
	HCURSOR CursorArrow = LoadCursor(NULL, IDC_ARROW);
	HCURSOR CursorBeam = LoadCursor(NULL, IDC_IBEAM);
	HCURSOR CursorWait = LoadCursor(NULL, IDC_WAIT);
	HCURSOR CursorCross = LoadCursor(NULL, IDC_CROSS);
	HCURSOR CursorUpArrow = LoadCursor(NULL, IDC_UPARROW);
	HCURSOR CursorSize = LoadCursor(NULL, IDC_SIZE);
	HCURSOR CursorIcon = LoadCursor(NULL, IDC_ICON);
	HCURSOR CursorSizeNWSE = LoadCursor(NULL, IDC_SIZENWSE);
	HCURSOR CursorSizeNESW = LoadCursor(NULL, IDC_SIZENESW);
	HCURSOR CursorSizeEWE = LoadCursor(NULL, IDC_SIZEWE);
	HCURSOR CursorSizeENS = LoadCursor(NULL, IDC_SIZENS);
	HCURSOR CursorSizeALL = LoadCursor(NULL, IDC_SIZEALL);
	HCURSOR CursorNo = LoadCursor(NULL, IDC_NO);
	HCURSOR CursorHand = LoadCursor(NULL, IDC_HAND);
	HCURSOR CursorAppStarting = LoadCursor(NULL, IDC_APPSTARTING);
	HCURSOR CursorHelp = LoadCursor(NULL, IDC_HELP);
	HCURSOR CursorPin = LoadCursor(NULL, IDC_PIN);
	HCURSOR CursorPerson = LoadCursor(NULL, IDC_PERSON);

	// NextDrawPos ------------------------------------------------------------------------------------------------
	Vec2 NextDrawPos, PreviousDrawPos = { 0, 0 };
	inline Vec2 GetNextDrawPos() { return NextDrawPos; }
	void SetNextDrawPos(Vec2 pos) { PreviousDrawPos = NextDrawPos; NextDrawPos = pos; }
	void SetNextDrawPosEx(Vec2 pos) { PreviousDrawPos = NextDrawPos; NextDrawPos = NextDrawPos + pos; }

	// Window settings --------------------------------------------------------------------------------------------
	int WindowId = -1;
	static std::map<int, Vec2> MenuPos;
	static std::map<int, Vec2> MenuSize;

	// Window -----------------------------------------------------------------------------------------------------
	std::map<int, bool> CanSetWindowPos, CanSetWindowSize; //WindowId is not correct but it will work lmfao.
	Vec2 NextWindowPos, NextWindowSize, NextWindowSmallSize = Vec2(0, 0);
	[[nodiscard]]Vec2 GetWindowPos() const { return MenuPos[WindowId]; }
	[[nodiscard]]Vec2 GetWindowSize() const { return MenuSize[WindowId]; }
	void SetNextWindowPos(Vec2 pos) { if (!CanSetWindowPos[GetWindowId()]) NextWindowPos = pos; CanSetWindowPos[GetWindowId()] = true; }
	void SetNextWindowPosEx(Vec2 pos) { if (!CanSetWindowPos[GetWindowId()]) NextWindowPos = NextWindowPos + pos; CanSetWindowPos[GetWindowId()] = true; }
	void SetNextWindowSize(Vec2 Smallest_Size, Vec2 Size) { if (!CanSetWindowSize[GetWindowId()]) { NextWindowSmallSize = Smallest_Size;  NextWindowSize = Size; } CanSetWindowSize[GetWindowId()] = true; }
	bool Window(int id, const char* title, bool draggable = true);
	bool EndWindow();

	// Window ID --------------------------------------------------------------------------------------------------
	bool CanDragWindow = false;
	Rect DraggingBounds = Rect(0, 0, 0, 0);
	void SetWindowDraggingBounds(Rect Bounds) { DraggingBounds = Bounds; }
	void SetWindowDraggingBounds(Vec2 Pos, Vec2 Size) { DraggingBounds = Rect(Pos.x, Pos.y, Size.x, Size.y); }
	Rect GetDraggingBounds() { return DraggingBounds; }
	void SetWindowDragability(bool state) { CanDragWindow = state; }
	bool GetWindowDragability() { return CanDragWindow; }
	int GetWindowId() { return WindowId; }
	void SetWindowId(int id) { WindowId = id; }

	// Children ---------------------------------------------------------------------------------------------------
	const char* CurrentChildName = "";
	Rect ChildArea = { 0, 0, 0, 0 };
	Vec2 ChildPos, ChildSize = { 0, 0 };
	Vec2 GetChildPos() { return ChildPos; }
	Vec2 GetChildSize() { return ChildSize; }
	Rect GetChildArea() { return ChildArea; }
	const char* GetCurrentChildName() { return CurrentChildName; }
	bool Child(const char* title, Vec2 size);
	bool EndChild();

	// Tabs -------------------------------------------------------------------------------------------------------
	bool Tab(const char* title, bool selected, Vec2 size = Vec2(100, 29));
	bool SubTab(const char* title, bool selected, Vec2 size = Vec2(100, 28));
	int CurrentTab = 0;
	int CurrentSubTab = 0;

	// Elements ---------------------------------------------------------------------------------------------------
	bool Checkbox(const char* title, bool* state);
	bool Combobox(const char* title, int* selected, std::vector<std::string> options);
	bool MultiCombobox(const char* title, std::vector<bool>& selected, std::vector<std::string> options);
	bool ListBox(const char* title, int* selected, std::vector<std::string> options);
	bool Slider(const char* title, int min, int max, int* currentValue, const char* format = "");
	bool Slider(const char* title, float min, float max, float* currentValue, const char* format = "");
	bool Button(const char* title, Vec2 Size = { 0, 0 });
	bool Textbox(const char* title, std::string &str);
	bool Keybind(const char* title, bool* key_state);
	bool ColorPicker(const char* title, Color* selected, bool alpha_bar = true);
	bool Popup(const char* text, Vec2 position, Color clr);
	bool Label(const char* str, Color custom_color = {255, 255, 255, 255});

	void RenderColorPickers();
	void RenderKeybinds();
	void RenderPopups();
	void RenderCombos();
	void RenderMultiCombos();

	// InputDisable -----------------------------------------------------------------------------------------------
	Rect DisableInputArea = { 0, 0, 0, 0 };
	bool InputAreaDisabled();
	void SetDisableInputArea(Vec2 pos, Vec2 size) { DisableInputArea = Rect(pos.x, pos.y, size.x, size.y); }

	// Identifier -------------------------------------------------------------------------------------------------
	int GetItemIdentifier() { return ItemIdentifier; }
	void SetItemIdentifier(int id) { ItemIdentifier = id; }
	int ItemIdentifier = 0;

	// Keybind & Textbox related ----------------------------------------------------------------------------------
	struct key_code_info {
		int vk_key;

		char regular;
		char upper;
	};

	std::string_view keys_list[256]{
				"error", "left mouse", "right mouse", "break", "middle mouse", "mouse 4", "mouse 5",
				"error", "backspace", "tab", "error", "error", "error", "enter", "error", "error", "shift",
				"ctrl", "alt", "pause", "caps lock", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "error", "error", "space", "pg up", "pg down", "end", "home", "left",
				"up", "right", "down", "error", "print", "error", "print screen", "insert", "delete", "error", "0", "1",
				"2", "3", "4", "5", "6", "7", "8", "9", "error", "error", "error", "error", "error", "error",
				"error", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
				"v", "w", "x", "y", "z", "left windows", "right windows", "error", "error", "error", "num 0", "num 1",
				"num 2", "num 3", "num 4", "num 5", "num 6", "num 7", "num 8", "num 9", "*", "+", "_", "-", ".", "/", "f1", "f2", "f3",
				"f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21",
				"f22", "f23", "f24", "error", "error", "error", "error", "error", "error", "error", "error",
				"num lock", "scroll lock", "error", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "error", "error", "error", "error", "shift", "shift", "ctrl",
				"ctrl", "menu", "menu", "error", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "next track", "previous track", "stop", "play/pause", "error", "error",
				"error", "error", "error", "error", ";", "+", ",", "-", ".", "/?", "~", "error", "error",
				"error", "error", "error", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "error", "error", "error", "[{", "\\|", "}]", "'\"", "error",
				"error", "error", "error", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "error", "error", "error", "error", "error", "error",
				"error", "error", "error", "error", "error", "error", "error", "error", "error",
				"error", "error"
	};

	inline static key_code_info special_characters[22] = {
			{48,  '0',  ')'},
			{49,  '1',  '!'},
			{50,  '2',  '@'},
			{51,  '3',  '#'},
			{52,  '4',  '$'},
			{53,  '5',  '%'},
			{54,  '6',  '^'},
			{55,  '7',  '&'},
			{56,  '8',  '*'},
			{57,  '9',  '('},
			{32,  ' ',  ' '},
			{192, '`',  '~'},
			{189, '-',  '_'},
			{187, '=',  '+'},
			{219, '[',  '{'},
			{220, '\\', '|'},
			{221, ']',  '}'},
			{186, ';',  ':'},
			{222, '\'', '"'},
			{188, ',',  '<'},
			{190, '.',  '>'},
			{191, '/',  '?'}
	};
};

extern guiStyle EGuiStyle;
extern guiColors EGuiColors;
extern EGuiMain EGui;