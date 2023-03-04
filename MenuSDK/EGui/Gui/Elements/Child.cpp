#include "../../EGui.hpp"

static auto PreviousPos = Vec2(0, 0);
bool EGuiMain::Child(const char* title, Vec2 size) {
	PreviousPos = NextDrawPos;
	ChildPos = NextDrawPos;
	ChildSize = size;

	auto hovered = Input.IsMouseHoveringRect(NextDrawPos, size);

	if (hovered)
		SetWindowDragability(false);

	renderer.FilledRectangle(NextDrawPos, size, EGuiColors.ChildBgColor, EGuiStyle.ChildRounding);
	renderer.Rectangle(NextDrawPos, size, EGuiColors.ElementBorderColor, EGuiStyle.ChildRounding);

	renderer.Text(title, NextDrawPos + Vec2{ 5, 4 }, LEFT, renderer.Verdana, false, EGuiColors.TextColor);

	renderer.FilledRectangle(NextDrawPos + Vec2{ 1, 20 }, { size.x - 2, 1 }, EGuiColors.MenuTheme);

	SetNextDrawPosEx({ 5, 25 });

	return true;
}

bool EGuiMain::EndChild() {
	SetNextDrawPos(PreviousPos);
	return true;
}