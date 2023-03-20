//1.00
/* EGui is software provided to you by Edwin777 (main developer)
* Everyone helped, your feedback was extremely helpful and will be extremely helpful within the coming years.
* I plan to maintain this code and fix security/code issues over time. this message was wrote march 11th, 2023.
* EGui was a dream of mine for a long time and it finally became a reality once I got back into school.
* I feel the support and the hate really helped me get this far into the project, Thank you to all who supported.
* To all who hated, I respect your opinion as I am a beginner to coding but you saying its impossible was a really big motivator for me.
* Thanks to all for the motivation, I hope yall can build off of my work and make something better. -Edwin777
*/

#pragma once
#define DIRECTINPUT_VERSION 0x800
#define EGUI_VERSION "pre pre pre alpha lol"

// External dependencies
#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>
#include <Windows.h>
#include <dInput.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>
#include <map>
#include <dwrite.h>
#include <chrono>
#include <cmath>

// EGui source files
#include "Backend/Render/Classes/Color.hpp"
#include "Backend/Render/Classes/Vector.hpp"
#include "Backend/Render/Classes/Vector2D.hpp"
#include "Backend/Render/Classes/Vertex.hpp"
#include "Backend/Render/Classes/Rect.hpp"
#include "Backend/Render/Classes/Easings.hpp"
#include "Backend/Render/Textures/Textures.hpp"
#include "Backend/Utility/Math.hpp"
#include "Backend/Input/Input.hpp"
#include "Backend/Win32/wnd.hpp"
#include "Backend/Animation/Timing.hpp"
#include "Backend/Animation/Animation.hpp"
#include "Backend/Camera/Camera.hpp"
#include "Backend/Graphics/DirectX.hpp"
#include "Backend/Render/renderer.hpp"
#include "Backend/Render/Wrapper.hpp"

// Pragma comments
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define PI 3.14159265359
#define PI_2 1.570796326795
#define PI_SQR 9.869604401090658

struct guiStyle {
	float Alpha = 255.f;
	float DisabledAlpha = 150.f;
	float Padding = 5.f;
	float FrameRounding = 4.f;
	float ChildRounding = 4.f;
	float ElementRounding = 4.f;
};

struct guiColors {
	Color MenuTheme = Color(45, 118, 69, 255);
	Color FrameBackColor = Color(15, 25, 41, 255);
	Color FrameHeaderColor = Color(25, 39, 58, 255);
	Color FrameBorderColor = Color(0, 0, 0, 255);
	Color ChildBgColor = Color(25, 39, 58, 255);
	Color ChildHeaderColor = Color(15, 29, 48, 255);
	Color TextColor = Color(250, 250, 250, 255);
	Color TextColorHovered = Color(255, 255, 255, 255);
	Color TextColorDisabled = Color(240, 240, 240, 255);
	Color ElementBackColor = Color(12, 26, 46, 255);
	Color ElementBorderColor = Color(0, 0, 0, 255);
	Color ElementColorHovered = Color(0, 0, 0, 0);
	Color ElementColorOn = MenuTheme;
};

struct Object {
	float Friction = 0.5f;
	float BounceFactor = 0.7f;
	float Weight = 100.f;
	float Size = 20;
	Vec2 Position = { 0, 0 };
	Vec2 Velocity = { -1000, 0 };
	Color Color = { 255, 0, 0, 255 };
};

struct EGuiMain {
	// EGui -------------------------------------------------------------------------------------------------------
	string GetVersion() { return EGUI_VERSION; }

	// Window -----------------------------------------------------------------------------------------------------
	HWND hwnd = NULL;
	void SetWindowHandle(HWND hWnd) { hwnd = hWnd; }

	// Direct3D ---------------------------------------------------------------------------------------------------
	IDirect3D9* D3D = NULL;
	D3DPRESENT_PARAMETERS d3dparams = { 0 };
	IDirect3DDevice9* Device = NULL;
	LPD3DXSPRITE Sprite = NULL;
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer = NULL;
	void SetDevice(IDirect3DDevice9* device) { Device = device; }
	IDirect3DDevice9* GetDevice() { return Device; }

	// EGui -------------------------------------------------------------------------------------------------------
	void Begin();
	void End();
	void DemoPhysics(Object* object);
	void DemoWindow();
	void DemoShapes();

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
	void SetDraggingArea(int id, Vec2 pos, Vec2 size);
	Rect GetDraggingArea(int id);
	bool InDraggingArea(int id);
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
	Vec2 NextDrawPos = { 0, 0 };
	Vec2 PreviousDrawPos = { 0, 0 };
	inline Vec2 GetNextDrawPos() { return NextDrawPos; }
	void SetNextDrawPos(Vec2 pos) { PreviousDrawPos = NextDrawPos; NextDrawPos = pos; }
	void SetNextDrawPosEx(Vec2 pos) { PreviousDrawPos = NextDrawPos; NextDrawPos = NextDrawPos + pos; }

	// Window settings --------------------------------------------------------------------------------------------
	int WindowId = -1;
	static std::unordered_map<int, Vec2> MenuPos;
	static std::unordered_map<int, Vec2> MenuSize;

	// Window -----------------------------------------------------------------------------------------------------
	std::unordered_map<int, bool> CanSetWindowPos, CanSetWindowSize; //WindowId is not correct but it will work lmfao.
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
	Vec2 ChildPos = { 0, 0 };
	Vec2 ChildSize = { 0, 0 };
	Vec2 GetChildPos() { return ChildPos; }
	Vec2 GetChildSize() { return ChildSize; }
	bool Child(const char* title, Vec2 size);
	bool EndChild();

	// Tabs -------------------------------------------------------------------------------------------------------
	bool Tab(const char* title, bool selected, Vec2 size = Vec2(100, 29));
	bool SubTab(const char* title, bool selected, Vec2 size = Vec2(100, 28));
	int CurrentTab = 0;
	int CurrentSubTab = 0;

	// Elements ---------------------------------------------------------------------------------------------------
	bool Checkbox(const char* title, bool* state);
	//bool Combobox(const char* title, int* selected, const char* items[]);
	bool Combobox(const char* title, int* selected, std::vector<std::string> options);
	bool MultiCombobox(const char* title, std::vector<bool>& selected, std::vector<std::string> options);
	bool Listbox(const char* title, int* selected, const char* items[]);
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

	bool InputAreaDisabled();
	Rect DisableInputArea = {0, 0, 0, 0};

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