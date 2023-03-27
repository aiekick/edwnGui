#include "../../EGui.hpp"

static std::unordered_map<int, float> tab_alpha;

bool EGuiMain::Tab(const char* title, bool selected, Vec2 size) {
	SetItemIdentifier(GetItemIdentifier() + 1);
	bool changed = false;

	if (Input.IsMouseHoveringRect(NextDrawPos, size))
		SetWindowDragability(false);

	if (Input.ButtonBehaviour(NextDrawPos, size, PRESS)) {
		CurrentSubTab = 0;
		changed = true;
	}

	float delta_time = timing.getDeltaTime();
	
	if (selected)
		tab_alpha[GetItemIdentifier()] = clamp(Animations.lerp(tab_alpha[GetItemIdentifier()], 255.f, delta_time * 8), 0.f, 255.f);
	else
		tab_alpha[GetItemIdentifier()] = clamp(Animations.lerp(tab_alpha[GetItemIdentifier()], 0.f, delta_time * 8), 0.f, 255.f);

	renderer.FilledRectangle(NextDrawPos, size, Color(255, 255, 255, tab_alpha[GetItemIdentifier()] / 25.5));

	renderer.PushAlpha(tab_alpha[GetItemIdentifier()]);
	renderer.FilledRectangle(NextDrawPos + Vec2(0, size.y), { size.x, 2 }, EGuiColors.MenuTheme);
	renderer.PopAlpha();

	renderer.Text(Fonts.TitleFont, title, { NextDrawPos.x + size.x / 2, NextDrawPos.y + size.y / 2 }, selected ? EGuiColors.TextColor : EGuiColors.TextColorDisabled, CENTER_XY);

	SetNextDrawPosEx({ size.x, 0 });

	return changed;
}