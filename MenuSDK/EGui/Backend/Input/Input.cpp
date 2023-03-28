#include "Input.hpp"

EInput Input;

//This could use a recode, try using ScreenToClient(hwnd, cursor_point), it should compensate for the window pos. too FUCKING tired, need sleep. its like 2pm and I been up for like 20 hours.
bool EInput::IsPointInRect(Vec2 point, Vec2 pos, Vec2 size) {
	return(point.x >= pos.x && point.x <= pos.x + size.x) && (point.y >= pos.y && point.y <= pos.y + size.y);
}

bool EInput::IsRectInRect(Vec2 pos, Vec2 size, Vec2 pos2, Vec2 size2) {
	bool visible = false;

	if (!visible && pos.x >= pos2.x)
		visible = true;

	if (!visible && pos.x + size.x <= pos2.x + size2.x)
		visible = true;

	if (!visible && pos.y >= pos2.y)
		visible = true;

	if (!visible && pos.y + size.y <= pos2.y + size2.y)
		visible = true;

	return visible;
}

bool EInput::IsMouseHoveringRect(Vec2 pos, Vec2 size) {
	if (!wnd.IsWindowParent())
		return false;

	Vec2 CursorPos = GetMousePos();
	return (CursorPos.x >= pos.x && CursorPos.x <= pos.x + size.x) && (CursorPos.y >= pos.y && CursorPos.y <= pos.y + size.y);
}

static std::map<int, bool> clicked;
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

static std::map<int, bool> keypressed;
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

void EInput::UpdateMousePos(const Vec2 point) {
	MousePos = point;
}

Vec2 EInput::GetMousePos() {
	return MousePos;
}

void EInput::UpdateMouseWheelDelta(float delta) {
	MouseWheelDelta = delta;
}

float EInput::GetMouseWheelDelta() {
	return MouseWheelDelta;
}