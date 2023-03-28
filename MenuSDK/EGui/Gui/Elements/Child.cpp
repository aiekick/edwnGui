#include "../../EGui.hpp"

struct ChildData {
	bool hovered = false;
	Vec2 Size = {};
	Vec2 OgNextDrawPos = {};
	float scroll_abs = 0.f;
	float scroll_weight = 0.f;
	Vec2 previous_pos = {0, 0};
};

std::map<const char*, ChildData> child_data;
bool EGuiMain::Child(const char* title, Vec2 size) {
	CurrentChildName = title;

	child_data[CurrentChildName].previous_pos = NextDrawPos;
	ChildPos = NextDrawPos;
	ChildSize = size;

	auto hovered = Input.IsMouseHoveringRect(NextDrawPos, size);
	child_data[CurrentChildName].hovered = hovered;

	if (hovered)
		SetWindowDragability(false);

	renderer.BorderedRectangle(NextDrawPos, size, EGuiColors.ChildBgColor, EGuiColors.ElementBorderColor, EGuiStyle.ChildRounding);
	renderer.FilledRectangle(NextDrawPos + Vec2(1, 1), { size.x - 2, 19 }, EGuiColors.ChildHeaderColor, EGuiStyle.ChildRounding, CORNER_TOP);

	renderer.Gradient(NextDrawPos + Vec2(1, 20), Vec2((size.x - 2) / 2, 1), EGuiColors.MenuTheme, EGuiColors.MenuTheme.OverrideAlpha(0), false);
	renderer.Gradient(NextDrawPos + Vec2(1 + ((size.x - 2) / 2), 20), Vec2((size.x - 2) / 2, 1), EGuiColors.MenuTheme.OverrideAlpha(0), EGuiColors.MenuTheme, false);
	renderer.Gradient(NextDrawPos + Vec2(1, 21), Vec2((size.x - 2), 10), {0, 0, 0, 100}, { 0, 0, 0, 0 }, true);

	renderer.Text(Fonts.Primary, title, NextDrawPos + Vec2{ 5, 4 }, EGuiColors.TextColor, LEFT);

	Vec2 ChildAreaPos = NextDrawPos + Vec2(0, 21);
	Vec2 ChildAreaSize = size - Vec2(0, 21);
	renderer.PushClip(ChildAreaPos, ChildAreaSize);

	ChildArea = { ChildAreaPos.x, ChildAreaPos.y, ChildAreaSize.x, ChildAreaSize.y };

	child_data[CurrentChildName].Size = size - Vec2(0, 21);
	child_data[CurrentChildName].OgNextDrawPos = NextDrawPos + Vec2(0, 25);
	SetNextDrawPosEx({ 5, 25 + child_data[CurrentChildName].scroll_weight});

	return true;
}

bool EGuiMain::EndChild() {
	if (child_data[CurrentChildName].hovered) {
		if (NextDrawPos.y - (child_data[CurrentChildName].OgNextDrawPos.y + child_data[CurrentChildName].Size.y) > 0)
			child_data[CurrentChildName].scroll_abs = Math.Clamp(child_data[CurrentChildName].scroll_abs + Input.GetMouseWheelDelta(), -(NextDrawPos.y - (child_data[CurrentChildName].OgNextDrawPos.y + child_data[CurrentChildName].Size.y)), 0.f);
		
		child_data[CurrentChildName].scroll_weight = Animations.smoothLerp(child_data[CurrentChildName].scroll_weight, child_data[CurrentChildName].scroll_abs, timing.getDeltaTime() * 8);
	}

	ChildArea.clear();

	SetNextDrawPos(child_data[CurrentChildName].previous_pos);
	renderer.PopClip();

	return true;
}