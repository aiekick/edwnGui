#include "../EGui.hpp"

static std::unordered_map<int, bool> ResizingState;
static std::unordered_map<int, bool> TriedResizeOutside;
static std::unordered_map<int, Vec2> Difference;
Vec2 EGuiMain::Resizing(int id, Vec2 pos, Vec2 size) {
	if (!wnd.IsWindowParent() || !Input.IsMouseHoveringRect(Vec2(0, 0), wnd.GetWindowSize()))
		return size;

	Vec2 NewSize = size;

	for (auto const& [thisid, IsResize] : ResizingState) {
		if (IsResize && thisid != id) {
			return size;
			break;
		}
	}

	auto ResizeArea = Rect(GetWindowPos().x + GetWindowSize().x - 10, GetWindowPos().y + GetWindowSize().y - 10, 10, 10);

	if (!ResizingState[id] && Input.IsKeyDown(VK_LBUTTON) && !Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h)))
		TriedResizeOutside[id] = true;
	else if (!ResizingState[id] && !Input.IsKeyDown(VK_LBUTTON))
		TriedResizeOutside[id] = false;

	//Set cursor style
	if (Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h)) || ResizingState[id])
		SetCursorStyle(cursorSizeNWSE);

	//Check if we should start DraggingState.
	if (!ResizingState[id] && !TriedResizeOutside[id] && Input.IsKeyDown(VK_LBUTTON) && Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h))) {
		ResizingState[id] = true;
		Difference[id] = Input.GetMousePos() - NewSize;
	}

	//Check for if we are not DraggingState anymore.
	if (ResizingState[id] && !Input.IsKeyDown(VK_LBUTTON))
		ResizingState[id] = false;

	if (ResizingState[id]) {
		NewSize = Input.GetMousePos() - MenuPos[id];
	}

	NewSize.x = std::clamp(NewSize.x, NextWindowSmallSize.x, (float)INT_MAX);
	NewSize.y = std::clamp(NewSize.y, NextWindowSmallSize.y, (float)INT_MAX);

	return NewSize;
}

bool EGuiMain::InResizingArea() {
	auto ResizeArea = Rect(GetWindowPos().x + GetWindowSize().x - 10, GetWindowPos().y + GetWindowSize().y - 10, 10, 10);
	return Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h));
}

bool EGuiMain::IsResizing(int id) {
	return ResizingState[id];
}