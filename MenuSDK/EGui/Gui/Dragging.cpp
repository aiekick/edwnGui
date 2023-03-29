#include "../EGui.hpp"

struct DraggingInfo {
	bool Dragging;
	bool OutOfBounds;
	Vec2 Difference;
};

static std::map<int, DraggingInfo> dragging_info;

Vec2 EGuiMain::Dragging(int id, Vec2 pos, Vec2 size, bool CanDragOffscreen, bool Child, Vec2 SnapPos) {
	if (!wnd.IsWindowParent() || !Input.IsMouseHoveringRect(Vec2(0, 0), wnd.GetWindowSize()) || IsResizing(GetWindowId()))
		return pos;

	Vec2 NewPos = pos;

	for (auto const& [thisid, isDragged] : dragging_info) {
		if (isDragged.Dragging && thisid != id) {
			return pos;
			break;
		}
	}

	if (!GetWindowDragability() || (!dragging_info[id].Dragging && Input.IsKeyDown(VK_LBUTTON) && !Input.IsMouseHoveringRect(pos, size)))
		dragging_info[id].OutOfBounds = true;
	else if (!dragging_info[id].Dragging && !Input.IsKeyDown(VK_LBUTTON))
		dragging_info[id].OutOfBounds = false;

	if (!dragging_info[id].Dragging && !dragging_info[id].OutOfBounds && Input.IsKeyDown(VK_LBUTTON) && Input.IsMouseHoveringRect(pos, size)) {
		dragging_info[id].Dragging = true;
		dragging_info[id].Difference = Input.GetMousePos() - NewPos;
	}

	if (dragging_info[id].Dragging && !Input.IsKeyDown(VK_LBUTTON))
		dragging_info[id].Dragging = false;

	if (dragging_info[id].Dragging) {
		NewPos = Input.GetMousePos() - dragging_info[id].Difference;
		SetCursorStyle(CursorSizeALL);
	}

	if (SnapPos.x != -1 && NewPos.x > SnapPos.x - 10 && NewPos.x < SnapPos.x + 10 && NewPos.y > SnapPos.y - 10 && NewPos.y < SnapPos.y + 10)
		NewPos = SnapPos;

	if (!CanDragOffscreen)
		NewPos = { Math.Clamp(NewPos.x, 0.f, wnd.GetWindowSize().x - size.x), Math.Clamp(NewPos.y, 0.f, wnd.GetWindowSize().y - size.y) };

	return NewPos;
}

bool EGuiMain::IsDragging(int id)
{
	return dragging_info[id].Dragging;
}