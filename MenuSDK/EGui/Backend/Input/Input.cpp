#include "Input.hpp"

EInput Input;

//This could use a recode, try using ScreenToClient(hwnd, cursor_point), it should compensate for the window pos. too FUCKING tired, need sleep. its like 2pm and I been up for like 20 hours.
bool EInput::IsMouseHoveringRect(Vec2 pos, Vec2 size)
{
	if (!wnd.IsWindowParent())
		return false;

	Vec2 WindowPos = wnd.GetWindowPos();

	POINT p;
	if (GetCursorPos(&p))
	{
		//@todo: get our actual current cursor size incase of custom cursors. https://forums.codeguru.com/showthread.php?449040-get-the-system-current-cursor-size
		//The (x = -7, y = -30) is to compensate for our cursor size.
		bool Windowed = wnd.IsWindowed(EGui.hwnd);
		if ((p.x - (Windowed ? 7 : 0) - WindowPos.x >= pos.x && p.x - (Windowed ? 7 : 0) - WindowPos.x <= pos.x + size.x) && (p.y - (Windowed ? 30 : 0) - WindowPos.y >= pos.y && p.y - (Windowed ? 30 : 0) - WindowPos.y <= pos.y + size.y))
			return true;
	}

	return false;
}

static std::unordered_map<int, bool> clicked;
bool EInput::ButtonBehaviour(Vec2 pos, Vec2 size, int button_type, char key) {
	if (!wnd.IsWindowParent())
		return false;

	// Generate a unique ID for the button based on its position, size, and type. This is fucking retarded - edwn change it, will cause high memory usage.
	int id = pos.x + pos.y + button_type + key;

	bool pressed = false;
	bool hovered = IsMouseHoveringRect(pos, size);

	// Only process button press if the user is not currently dragging.
	if (!EGui.IsDragging(EGui.GetWindowId())) {
		if (hovered) {
			if (button_type == HOLD) {
				// Button press type 0: the button is pressed as long as the mouse is hovering over it.
				pressed = (GetKeyState(key) & 0x80) != 0;
			}
			else if (button_type == PRESS) {
				// Button press type 1: the button is only pressed when the mouse is first clicked while hovering over it.
				if ((GetKeyState(key) & 0x80) != 0 && !clicked[id])
					pressed = true;
			}
		}
	}

	// Update the clicked state of the button.
	if ((GetKeyState(key) & 0x80) != 0 && !clicked[id]) {
		clicked[id] = true;
	}
	else if ((GetKeyState(key) & 0x80) == 0 && clicked[id]) {
		clicked[id] = false;
	}

	//Fix for combo, multi combo, color pickers, popup, fixes clicking upon other elements behind them. for some reason its broken rn.
	if (pressed && EGui.InputAreaDisabled() && IsMouseHoveringRect(Vec2(EGui.DisableInputArea.x, EGui.DisableInputArea.y), Vec2(EGui.DisableInputArea.w, EGui.DisableInputArea.h)))
		return false;

	return pressed;
}

bool EInput::IsKeyDown(int key) {
	if (!wnd.IsWindowParent())
		return false;

	return (GetKeyState(key) & 0x80);
}

static std::unordered_map<int, bool> keypressed;
bool EInput::IsKeyPressed(int key)
{
	if (!wnd.IsWindowParent())
		return false;

	bool pressed = false;

	if ((GetKeyState(key) & 0x80) != 0 && !keypressed[key])
		pressed = true;

	if (GetKeyState(key) & 0x80 && !keypressed[key])
		keypressed[key] = true;
	else if (!(GetKeyState(key) & 0x80) && keypressed[key])
		keypressed[key] = false;

	return pressed;
}

Vec2 EInput::GetMousePos() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(EGui.hwnd, &p);

	return { (float)p.x, (float)p.y };
}