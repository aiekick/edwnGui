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

	renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2{ 5, 4 }, EGuiColors.TextColor, LEFT);

	renderer.Gradient(NextDrawPos + Vec2(1, 20), Vec2((size.x - 2) / 2, 1), EGuiColors.MenuTheme, EGuiColors.MenuTheme.OverrideAlpha(0), false);
	renderer.Gradient(NextDrawPos + Vec2(1 + ((size.x - 2) / 2), 20), Vec2((size.x - 2) / 2, 1), EGuiColors.MenuTheme.OverrideAlpha(0), EGuiColors.MenuTheme, false);

	renderer.PushClip(NextDrawPos, size);

	SetNextDrawPosEx({ 5, 25 });

	return true;
}

bool EGuiMain::EndChild() {
	renderer.PopClip();

	SetNextDrawPos(PreviousPos);
	return true;
}