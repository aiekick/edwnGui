#include "../EGui.hpp"

struct ResizingInfo {
	Rect ResizingArea;
	bool Resizing;
	bool OutOfBounds;
	Vec2 Difference;
};

static std::map <int, ResizingInfo> resizing_info;

Vec2 EGuiMain::Resizing(int id, Vec2 size) {
	if (!wnd.IsWindowParent() || !Input.IsMouseHoveringRect(Vec2(0, 0), wnd.GetWindowSize()))
		return size;

	Vec2 NewSize = size;

	for (auto const& [thisid, IsResize] : resizing_info) {
		if (IsResize.Resizing && thisid != id) {
			return size;
			break;
		}
	}

	auto ResizeArea = GetResizingArea(id);

	if (!resizing_info[id].Resizing && Input.IsKeyDown(VK_LBUTTON) && !Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h)))
		resizing_info[id].OutOfBounds = true;
	else if (!resizing_info[id].Resizing && !Input.IsKeyDown(VK_LBUTTON))
		resizing_info[id].OutOfBounds = false;

	//Set cursor style
	if (Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h)) || resizing_info[id].Resizing)
		SetCursorStyle(CursorSizeNWSE);

	//Check if we should start DraggingState.
	if (!resizing_info[id].Resizing && !resizing_info[id].OutOfBounds && Input.IsKeyDown(VK_LBUTTON) && Input.IsMouseHoveringRect(Vec2(ResizeArea.x, ResizeArea.y), Vec2(ResizeArea.w, ResizeArea.h))) {
		resizing_info[id].Resizing = true;
		resizing_info[id].Difference = Input.GetMousePos() - NewSize;
	}

	//Check for if we are not DraggingState anymore.
	if (resizing_info[id].Resizing && !Input.IsKeyDown(VK_LBUTTON))
		resizing_info[id].Resizing = false;

	if (resizing_info[id].Resizing) {
		NewSize = Input.GetMousePos() - MenuPos[id];
	}

	NewSize.x = Math.Clamp(NewSize.x, NextWindowSmallSize.x, (float)INT_MAX);
	NewSize.y = Math.Clamp(NewSize.y, NextWindowSmallSize.y, (float)INT_MAX);

	return NewSize;
}

void EGuiMain::SetResizingArea(int id, Vec2 pos, Vec2 size) {
	resizing_info[id].ResizingArea = {pos.x, pos.y, size.x, size.y};
}

Rect EGuiMain::GetResizingArea(int id) {
	return resizing_info[id].ResizingArea;
}

bool EGuiMain::InResizingArea(int id) {
	return Input.IsMouseHoveringRect( { resizing_info[id].ResizingArea.x, resizing_info[id].ResizingArea.y}, {resizing_info[id].ResizingArea.w, resizing_info[id].ResizingArea.h});
}

bool EGuiMain::IsResizing(int id) {
	return resizing_info[id].Resizing;
}