#include "../EGui.hpp"

static std::unordered_map<int, bool> DraggingState;
static std::unordered_map<int, bool> TriedDragOutside;
static std::unordered_map<int, Vec2> Difference;
Vec2 EGuiMain::Dragging(int id, Vec2 pos, Vec2 size, bool CanDragOffscreen, bool Child, Vec2 SnapPos) {
	if (!wnd.IsWindowParent() || !Input.IsMouseHoveringRect(Vec2(0, 0), Input.GetWindowSize()))
		return pos;

	if (InResizingArea())
		return pos;

	Vec2 NewPos = pos;

	for (auto const& [thisid, isDragged] : DraggingState) {
		if (isDragged && thisid != id) {
			return pos;
			break;
		}
	}

	if (!DraggingState[id] && Input.IsKeyDown(VK_LBUTTON) && !Input.IsMouseHoveringRect(pos, size))
		TriedDragOutside[id] = true;
	else if (!DraggingState[id] && !Input.IsKeyDown(VK_LBUTTON))
		TriedDragOutside[id] = false;

	//Check if we should start DraggingState.
	if (!DraggingState[id] && !TriedDragOutside[id] && Input.IsKeyDown(VK_LBUTTON) && Input.IsMouseHoveringRect(pos, size)) {
		DraggingState[id] = true;
		Difference[id] = Input.GetMousePos() - NewPos;
	}

	//Check for if we are not DraggingState anymore.
	if (DraggingState[id] && !Input.IsKeyDown(VK_LBUTTON))
		DraggingState[id] = false;

	if (DraggingState[id])
		NewPos = Input.GetMousePos() - Difference[id];

	if (SnapPos.x != -1 && NewPos.x > SnapPos.x - 10 && NewPos.x < SnapPos.x + 10 && NewPos.y > SnapPos.y - 10 && NewPos.y < SnapPos.y + 10)
		NewPos = SnapPos;

	//This restricts our element from going out of window bounds
	if (!CanDragOffscreen)
		NewPos = { std::clamp(NewPos.x, 0.f, Input.GetWindowSize().x - size.x), std::clamp(NewPos.y, 0.f, Input.GetWindowSize().y - size.y) };

	return NewPos;
}

bool EGuiMain::IsDragging(int id)
{
	return DraggingState[id];
}