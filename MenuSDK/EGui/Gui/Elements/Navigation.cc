#include "../../EGui.hh"

static std::unordered_map<int, float> tab_alpha;
static std::unordered_map<int, float> subtab_alpha;

EGUI_API bool EGuiMain::Tab(const char* title, bool selected, Vec2 size) {
	SetItemIdentifier(GetItemIdentifier() + 1);
	bool changed = false;

	if (Input.IsMouseHoveringRect(NextDrawPos, size))
		SetWindowDragability(false);

	if (Input.ButtonBehaviour(NextDrawPos, size, PRESS)) {
		CurrentSubTab = 0;
		changed = true;
	}

	if (selected) {
		drawList.FilledRectangle(NextDrawPos, size, Color(255, 255, 255, 10));
		drawList.FilledRectangle(NextDrawPos + Vec2(0, size.y), { size.x, 1 }, EGuiColors.MenuTheme);
	}

	drawList.Text(title, { NextDrawPos.x + size.x / 2, NextDrawPos.y + size.y / 2 }, CENTER_XY, drawList.TitleFont, false, selected ? EGuiColors.TextColor : EGuiColors.TextColorDisabled);

	SetNextDrawPosEx({ size.x, 0 });

	return changed;
}

EGUI_API bool EGuiMain::SubTab(const char* title, bool selected, Vec2 size) {
	SetItemIdentifier(GetItemIdentifier() + 1);
	bool changed = false;

	if (Input.IsMouseHoveringRect(NextDrawPos, size))
		SetWindowDragability(false);

	if (Input.ButtonBehaviour(NextDrawPos, size, PRESS))
		changed = true;

	if (selected)
		subtab_alpha[GetItemIdentifier()] = std::clamp(subtab_alpha[GetItemIdentifier()] + 2.5f, 0.f, 50.f);
	else
		subtab_alpha[GetItemIdentifier()] = std::clamp(subtab_alpha[GetItemIdentifier()] - 2.5f, 0.f, 50.f);

	drawList.FilledRectangle(NextDrawPos - Vec2(0, 1), size, Color(EGuiColors.MenuTheme.r(), EGuiColors.MenuTheme.g(), EGuiColors.MenuTheme.b(), subtab_alpha[GetItemIdentifier()]), true);
	drawList.FilledRectangle(NextDrawPos - Vec2(0, 1), { 1, size.y }, Color(EGuiColors.MenuTheme.r(), EGuiColors.MenuTheme.g(), EGuiColors.MenuTheme.b(), subtab_alpha[GetItemIdentifier()] * 5.1));

	drawList.Text(title, { NextDrawPos.x + size.x / 2, NextDrawPos.y + size.y / 2 }, CENTER_XY, drawList.TabIcon, false, EGuiColors.TextColor);

	SetNextDrawPosEx({ 0, size.y });

	return changed;
}

static auto PreviousPos = Vec2(0, 0);
EGUI_API bool EGuiMain::Child(const char* title, Vec2 size) {
	PreviousPos = NextDrawPos;
	ChildPos = NextDrawPos;
	ChildSize = size;

	auto hovered = Input.IsMouseHoveringRect(NextDrawPos, size);

	if (hovered)
		SetWindowDragability(false);

	drawList.FilledRectangle(NextDrawPos, size, EGuiColors.ChildBgColor, EGuiStyle.ChildRounding);
	drawList.Rectangle(NextDrawPos, size, EGuiColors.ElementBorderColor, EGuiStyle.ChildRounding);

	drawList.Text(title, NextDrawPos + Vec2{ 5, 4}, LEFT, drawList.Verdana, false, EGuiColors.TextColor);

	drawList.FilledRectangle(NextDrawPos + Vec2{ 1, 20 }, { size.x - 2, 1 }, EGuiColors.MenuTheme);

	SetNextDrawPosEx({ 5, 25 });

	return true;
}

EGUI_API bool EGuiMain::EndChild() {
	SetNextDrawPos(PreviousPos);
	return true;
}