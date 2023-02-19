#pragma once
#define DIRECTINPUT_VERSION 0x800
#define EGUI_VERSION "pre-1.0"
#define EGUI_API

// External dependencies
#include <d3d9.h>
#include <d3dx9.h>
#include <d2d1.h>
#include <D3dx9tex.h>
#include <Windows.h>
#include <dinput.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>
#include <dwmapi.h>
#include <map>
#include <dwrite.h>

// EGui source files
#include "Backend/Render/Types/Color.hh"
#include "Backend/Render/Types/Vector.hh"
#include "Backend/Render/Types/Vector2D.hh"
#include "Backend/Render/Types/Vertex.hh"
#include "Backend/Render/Types/Rect.hh"
#include "Backend/Render/Types/Easing.hh"
#include "Backend/Render/Textures/Textures.hh"
#include "Backend/Utility/Utility.hh"
#include "Backend/Input/Input.hh"
#include "Backend/Window/Window.hh"
#include "Backend/Animation/Timing.hh"
#include "Backend/Animation/Animation.h"
#include "Backend/Graphics/Direct2D/Direct2D.h"
#include "Backend/Graphics/DirectX.hh"
#include "Backend/Render/Renderer.hh"

// Pragma comments
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d2d1")
#pragma comment (lib, "dwmapi")
#pragma comment (lib, "dwrite.lib")

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
	Color TextColor = Color(250, 250, 250, 255);
	Color TextColorHovered = Color(255, 255, 255, 255);
	Color TextColorDisabled = Color(240, 240, 240, 255);
	Color ElementBackColor = Color(12, 26, 46, 255);
	Color ElementBorderColor = Color(0, 0, 0, 255);
	Color ElementColorHovered = Color(0, 0, 0, 0); //UNUSED - USE IF YOU WANT.
	Color ElementColorOn = MenuTheme;
};

struct EGuiDebugWindow {
	float Alpha = 255;
	bool Line;
	bool Rectangle;
	bool FilledRectangle;
	bool BorderedRectangle;
	bool Gradient;
	bool Triangle;
	bool Circle;
	bool FilledCircle;
	bool BorderedCircle;
	bool Text;
	bool Sprite;
};

#define NullRect  Rect(0, 0, 0, 0)
#define NullVec2  Vec2(0, 0)
#define NullVec   Vec(0, 0, 0)
#define NullColor Color(0, 0, 0, 0)

struct EGuiMain {
	// EGui -------------------------------------------------------------------------------------------------------
	EGUI_API string GetVersion() { return EGUI_VERSION; }

	// Window -----------------------------------------------------------------------------------------------------
	EGUI_API bool Windowed = false;
	EGUI_API HWND hwnd = NULL;
	EGUI_API bool NoWindowHeader = false;
	EGUI_API void SetWindowHandle(HWND hWnd) { hwnd = hWnd; }
	EGUI_API void SetWindowed(bool state) { Windowed = state; }

	// Direct3D ---------------------------------------------------------------------------------------------------
	EGUI_API IDirect3D9* D3D = NULL;
	EGUI_API D3DPRESENT_PARAMETERS d3dparams = { 0 };
	EGUI_API IDirect3DDevice9* Device = NULL;
	EGUI_API LPD3DXSPRITE Sprite = NULL;
	EGUI_API LPDIRECT3DVERTEXBUFFER9 VertexBuffer = NULL;
	EGUI_API void SetDevice(IDirect3DDevice9* device) { Device = device; }
	EGUI_API IDirect3DDevice9* GetDevice() { return Device; }

	// Direct2D ---------------------------------------------------------------------------------------------------
	EGUI_API ID2D1Factory* m_pD2DFactory;
	EGUI_API ID2D1HwndRenderTarget* m_pRenderTarget;
	EGUI_API ID2D1SolidColorBrush* m_pSolidColorBrush;
	EGUI_API IDWriteFactory* m_pDWriteFactory;
	EGUI_API IDWriteTextFormat* m_pTextFormat;

	// EGui -------------------------------------------------------------------------------------------------------
	EGUI_API void Begin();
	EGUI_API void End();
	EGUI_API void DemoWindow();

	// Dragging ---------------------------------------------------------------------------------------------------
	EGUI_API Vec2 Dragging(int id, Vec2 pos, Vec2 size, bool CanDragOffscreen = true, bool Child = false, Vec2 SnapPos = { 0, 0 });
	EGUI_API bool IsDragging(int id);

	// Window settings --------------------------------------------------------------------------------------------
	EGUI_API Vec2 MenuPos = { 550, 300 };
	EGUI_API Vec2 MenuSize = { 650, 665 };

	// NextDrawPos ------------------------------------------------------------------------------------------------
	EGUI_API Vec2 NextDrawPos = { 0, 0 };
	EGUI_API Vec2 PreviousDrawPos = { 0, 0 };
	EGUI_API Vec2 GetNextDrawPos() { return NextDrawPos; }
	EGUI_API void SetNextDrawPos(Vec2 pos) { PreviousDrawPos = NextDrawPos; NextDrawPos = pos; }
	EGUI_API void SetNextDrawPosEx(Vec2 pos) { PreviousDrawPos = NextDrawPos; NextDrawPos = NextDrawPos + pos; }

	// Window -----------------------------------------------------------------------------------------------------
	EGUI_API [[nodiscard]]Vec2 GetWindowPos() const { return MenuPos; }
	EGUI_API [[nodiscard]]Vec2 GetWindowSize() const { return MenuSize; }
	EGUI_API void SetNextWindowPos(Vec2 pos) { MenuPos = pos; }
	EGUI_API void SetNextWindowPosEx(Vec2 pos) { MenuPos = MenuPos + pos; }
	EGUI_API void SetNextWindowSize(Vec2 Size) { MenuSize = Size; }
	EGUI_API void SetNextWindowConstraints(Rect rect) { MenuPos = Vec2(rect.x, rect.y); MenuSize = Vec2(rect.w, rect.h); }
	EGUI_API bool Window(int id, const char* title, bool draggable = true);
	EGUI_API bool EndWindow();

	// Window ID --------------------------------------------------------------------------------------------------
	EGUI_API int WindowId = 0;
	EGUI_API bool CanDragWindow = false;
	EGUI_API Rect DraggingBounds = Rect(0, 0, 0, 0);
	EGUI_API void SetWindowDraggingBounds(Rect Bounds) { DraggingBounds = Bounds; }
	EGUI_API void SetWindowDraggingBounds(Vec2 Pos, Vec2 Size) { DraggingBounds = Rect(Pos.x, Pos.y, Size.x, Size.y); }
	EGUI_API Rect GetDraggingBounds() { return DraggingBounds; }
	EGUI_API void SetWindowDragability(bool state) { CanDragWindow = state; }
	EGUI_API bool GetWindowDragability() { return CanDragWindow; }
	EGUI_API int GetWindowId() { return WindowId; }
	EGUI_API void SetWindowId(int id) { WindowId = id; }

	// Children ---------------------------------------------------------------------------------------------------
	EGUI_API Vec2 ChildPos = { 0, 0 };
	EGUI_API Vec2 ChildSize = { 0, 0 };
	EGUI_API Vec2 GetChildPos() { return ChildPos; }
	EGUI_API Vec2 GetChildSize() { return ChildSize; }
	EGUI_API bool Child(const char* title, Vec2 size);
	EGUI_API bool EndChild();

	// Tabs -------------------------------------------------------------------------------------------------------
	EGUI_API bool Tab(const char* title, bool selected, Vec2 size = Vec2(100, 30));
	EGUI_API bool SubTab(const char* title, bool selected, Vec2 size = Vec2(100, 28));
	EGUI_API int CurrentTab = 0;
	EGUI_API int CurrentSubTab = 0;

	// Elements ---------------------------------------------------------------------------------------------------
	EGUI_API bool Checkbox(const char* title, bool* state);
	EGUI_API bool Combobox(const char* title, int* selected, const char* items[]);
	EGUI_API bool MultiCombobox(const char* title, std::vector<bool>& selected, const char* items[]);
	EGUI_API bool Listbox(const char* title, int* selected, const char* items[]);
	EGUI_API bool Slider(const char* title, int min, int max, int* currentValue, const char* format = "");
	EGUI_API bool Slider(const char* title, int min, int max, float* currentValue, const char* format = "");
	EGUI_API bool Button(const char* title, Vec2 Size = { 0, 0 });
	EGUI_API bool Textbox(const char* title, std::string &str);
	EGUI_API bool Keybind(const char* id, int& key);
	EGUI_API bool ColorPicker(const char* title, Color* selected);
	EGUI_API bool InputAreaDisabled();
	EGUI_API Rect DisableInputArea = NullRect;

	// Identifier -------------------------------------------------------------------------------------------------
	EGUI_API int GetItemIdentifier() { return ItemIdentifier; }
	EGUI_API void SetItemIdentifier(int id) { ItemIdentifier = id; }
	EGUI_API int ItemIdentifier = 0;

	// Keybind & Textbox related ----------------------------------------------------------------------------------
	EGUI_API struct key_code_info {
		int vk_key;

		char regular;
		char upper;
	};

	EGUI_API std::string_view keys_list[256]{
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

	EGUI_API inline static key_code_info special_characters[22] = {
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


	// Cursor ---------------------------------------------------------------------------------------------------------
	EGUI_API using CursorStyle = enum struct ESCursorStyle : int {
		NONE = 0,
		ARROW,
		HAND,
		IBEAM,
		PIPETTE,
		MOVE
	};

	EGUI_API void SetCursor(CursorStyle cursor);
	EGUI_API int g_CursorStyle;
};

extern guiStyle EGuiStyle;
extern guiColors EGuiColors;
extern EGuiDebugWindow DebugWindow;
extern EGuiMain EGui;