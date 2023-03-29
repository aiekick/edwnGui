#include "Input.hpp"
EInput Input;

bool EInput::IsPointInRect(Vec2 point, Vec2 pos, Vec2 size) {
	return(point.x >= pos.x && point.x <= pos.x + size.x) && (point.y >= pos.y && point.y <= pos.y + size.y);
}

bool EInput::IsRectInRect(Vec2 pos, Vec2 size, Vec2 pos2, Vec2 size2) {
	bool visible = false;

	std::vector<Vec2> points = {
		{pos.x, pos.y}, //top left
		{pos.x + size.x, pos.y}, //top right
		{pos.x, pos.y + size.y}, //bottom left
		{pos.x + size.x, pos.y + size.y} // bottom right
	};

	for (int i = 0; i < points.size(); i++) {
		if (IsPointInRect(points[i], pos2, size2))
			visible = true;
	}

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

	int id = pos.x + pos.y + button_type + key;

	bool pressed = false;
	bool hovered = IsMouseHoveringRect(pos, size);

	if (!EGui.IsDragging(EGui.GetWindowId())) {
		if (hovered) {
			if (button_type == HOLD)
				pressed = (GetKeyState(key) & 0x80) != 0;
			else if (button_type == PRESS) {
				if ((GetKeyState(key) & 0x80) != 0 && !clicked[id])
					pressed = true;
			}
		}
	}

	if ((GetKeyState(key) & 0x80) != 0 && !clicked[id])
		clicked[id] = true;
	else if ((GetKeyState(key) & 0x80) == 0 && clicked[id])
		clicked[id] = false;

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