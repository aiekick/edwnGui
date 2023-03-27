#include "../../EGui.hpp"

static std::map<int, float> subtab_alpha;

bool EGuiMain::SubTab(const char* title, bool selected, Vec2 size) {
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

	renderer.FilledRectangle(NextDrawPos - Vec2(0, 1), size, Color(EGuiColors.MenuTheme.r(), EGuiColors.MenuTheme.g(), EGuiColors.MenuTheme.b(), subtab_alpha[GetItemIdentifier()]), true);
	renderer.FilledRectangle(NextDrawPos - Vec2(0, 1), { 1, size.y }, Color(EGuiColors.MenuTheme.r(), EGuiColors.MenuTheme.g(), EGuiColors.MenuTheme.b(), subtab_alpha[GetItemIdentifier()] * 5.1));

	renderer.Text(Fonts.Primary, title, { NextDrawPos.x + size.x / 2, NextDrawPos.y + size.y / 2 }, EGuiColors.TextColor, CENTER_XY);

	SetNextDrawPosEx({ 0, size.y });

	return changed;
}